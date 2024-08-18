#include "M5Dial.h"
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()

long oldPosition = -999;
uint8_t CMDID = 0;

// Global copy of slave
esp_now_peer_info_t slave;
#define CHANNEL 1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// Scan for slaves in AP mode
void ScanForSlave() {
  int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL); // Scan only on one channel
  // reset on each scan
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (PRINTSCANRESULTS) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(SSID);
        Serial.print(" (");
        Serial.print(RSSI);
        Serial.print(")");
        Serial.println("");
      }
      delay(10);
      // Check if the current device starts with `Slave`
      if (SSID.indexOf("Slave") == 0) {
        // SSID of interest
        Serial.println("Found a Slave.");
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
        // Get BSSID => Mac Address of the Slave
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = CHANNEL; // pick a channel
        slave.encrypt = 0; // no encryption

        slaveFound = 1;
        // we are planning to have only one slave in this example;
        // Hence, break after we find one, to be a bit efficient
        break;
      }
    }
  }

  if (slaveFound) {
    Serial.println("Slave Found, processing..");
  } else {
    Serial.println("Slave Not Found, trying again.");
  }

  // clean up ram
  WiFi.scanDelete();
}

// Check if the slave is already paired with the master.
// If not, pair the slave with master
bool manageSlave() {
  if (slave.channel == CHANNEL) {
    if (DELETEBEFOREPAIR) {
      deletePeer();
    }

    Serial.print("Slave Status: ");
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(slave.peer_addr);
    if ( exists) {
      // Slave already paired.
      Serial.println("Already Paired");
      return true;
    } else {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(&slave);
      if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Slave found to process");
    return false;
  }
}

void deletePeer() {
  esp_err_t delStatus = esp_now_del_peer(slave.peer_addr);
  Serial.print("Slave Delete Status: ");
  if (delStatus == ESP_OK) {
    // Delete success
    Serial.println("Success");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW Not Init");
  } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

void sendData(uint8_t data) {
 
  const uint8_t *peer_addr = slave.peer_addr;
  Serial.print("Sending: "); Serial.println(data);
  esp_err_t result = esp_now_send(peer_addr, &data, sizeof(data));
  Serial.print("Send Status: ");
  if (result == ESP_OK) {
    Serial.println("Success");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void taskEncoder(void *pvParameters) {
  while(1) {
  M5Dial.update();
    long newPosition = M5Dial.Encoder.read();
    if (newPosition != oldPosition) {
        M5Dial.Speaker.tone(8000, 20);
        
        if (newPosition > oldPosition) {CMDID = 9;}
        if (newPosition < oldPosition) {CMDID = 10; }
        oldPosition = newPosition;
        Serial.println(newPosition);
        
        sendData(CMDID);

    }
    if (M5Dial.BtnA.wasPressed()) {
        M5Dial.Encoder.readAndReset();
    }
    if (M5Dial.BtnA.pressedFor(5000)) {
        M5Dial.Encoder.write(100);
    }
    vTaskDelay(100);
  }
}

void setup() {
  auto cfg = M5.config();
    M5Dial.begin(cfg, true, true);
    WiFi.mode(WIFI_STA);
    M5Dial.Display.setTextColor(GREEN);
    M5Dial.Display.setTextDatum(middle_center);
    M5Dial.Display.setTextFont(&fonts::Orbitron_Light_32);
    M5Dial.Display.setTextSize(1);

    M5Dial.Display.drawString("Robot Control", M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);

InitESPNow();
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  xTaskCreate(taskEncoder, "Encoder", 2048, NULL, 2, NULL);
}

void loop() {
 ScanForSlave();
  // If Slave is found, it would be populate in `slave` variable
  // We will check if `slave` is defined and then we proceed further
  if (slave.channel == CHANNEL) { // check if slave channel is defined
    // `slave` is defined
    // Add slave as peer if it has not been added already
    bool isPaired = manageSlave();
    if (isPaired) {
      // pair success or already paired
      // Send data to device
      /**
   * @brief Get the ID corresponding to the command word 
   * @return Return the obtained command word ID, returning 0 means no valid ID is obtained
   */
   //encoder();
  
if (M5Dial.Rfid.PICC_IsNewCardPresent() &&
        M5Dial.Rfid.PICC_ReadCardSerial()) {
        M5Dial.Display.clear();

        Serial.print(F("PICC type: "));
        uint8_t piccType = M5Dial.Rfid.PICC_GetType(M5Dial.Rfid.uid.sak);
        Serial.println(M5Dial.Rfid.PICC_GetTypeName(piccType));

        // Check is the PICC of Classic MIFARE type
        if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
            piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
            piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
            Serial.println(F("Your tag is not of type MIFARE Classic."));
            M5Dial.Display.drawString("not support", M5Dial.Display.width() / 2,
                                      M5Dial.Display.height() / 2);
            return;
        }
        String uid = "";
        for (byte i = 0; i < M5Dial.Rfid.uid.size;
             i++) {  // Output the stored UID data.  将存储的UID数据输出
            Serial.printf("%02X ", M5Dial.Rfid.uid.uidByte[i]);
            uid += String(M5Dial.Rfid.uid.uidByte[i], HEX);
        }
        M5Dial.Speaker.tone(8000, 20);
        
        
        Serial.println();
        Serial.println(uid);
        CMDID = 0;
        if (uid == "af4705") {CMDID = 5; M5Dial.Display.drawString("Stop", M5Dial.Display.width()/2, M5Dial.Display.height()/2-30);} //stop red card
        if (uid == "4e41357c") {CMDID = 8; M5Dial.Display.drawString("Reverse", M5Dial.Display.width()/2, M5Dial.Display.height()/2-30);} //reverse orange card
        if (uid == "96fc9541") {CMDID = 22; M5Dial.Display.drawString("Forward", M5Dial.Display.width()/2, M5Dial.Display.height()/2-30);} //forward  green card
        if (uid == "e4f7a375") {CMDID = 6; M5Dial.Display.drawString("Left", M5Dial.Display.width()/2, M5Dial.Display.height()/2-30);} //left blue card
        if (uid == "c4d7b56a") {CMDID = 7; M5Dial.Display.drawString("Right", M5Dial.Display.width()/2, M5Dial.Display.height()/2-30);} //right black card
        // CMDID 9 increase speed CMDID 10 decrease speed
        sendData(CMDID);
        }
      
    } else {
      // slave pair failed
      Serial.println("Slave pair failed!");
    }
  }
  else {
    // No slave found to process
  }

  sendData(0);
  //delay(3000);
}
