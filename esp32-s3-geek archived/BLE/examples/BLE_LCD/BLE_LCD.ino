#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        Serial.println("***************");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);
        Serial.println();
        Serial.println("***************");
        const char *rxValue1 = rxValue.c_str();
        Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
        Paint_SetRotate(90);
        LCD_Clear(BLACK);
        Paint_DrawString_EN(0, 10, "***************", &Font24, BLACK, WHITE);
        Paint_DrawString_EN(0, 35, "Receive Value:", &Font24, BLACK, WHITE);
        Paint_DrawString_EN(0, 58, rxValue1, &Font20, BLACK, 0xfff0); 
        Paint_DrawString_EN(0, 100, "***************", &Font24, BLACK, WHITE); 
      }
    }
};

void sendBluetoothData(uint8_t *data, size_t length) {
  if (deviceConnected) {
    pTxCharacteristic->setValue(data, length);
    pTxCharacteristic->notify();
  }
}

void setup()
{
  Config_Init();
  LCD_Init();
  Serial.begin(115200);
  LCD_Clear(BLACK);
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  Paint_DrawImage(gImage_pic1, 0, 0, 135, 240);
  // Create the BLE Device
  BLEDevice::init("Waveshare_ESP32S3_GEEK");
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);  
  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());  
  // Start the service
  pService->start();
  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");  
}

void loop()
{
  // Read all available data from Serial and send it via BLE
  if (Serial.available() > 0) {
    String readBuff;
    char readBuff1[100];
    size_t bufferSize = 0;  // Current size of data in buffer

    while (Serial.available() > 0) {
      char input = Serial.read();
      readBuff += input;  // Store input in buffer
      bufferSize ++;
      // Check if the buffer is full, or a newline character is received
      if (input == '\n') {
        // Send the entire buffer via BLE
        int idx = readBuff.lastIndexOf("\r");
        readBuff = readBuff.substring(0,idx);
        sprintf(readBuff1,"%s",readBuff);
        sendBluetoothData((uint8_t*)readBuff1, (bufferSize - 2));
        Serial.println("***************");
        Serial.print("Send Value: ");
        Serial.print(readBuff1);
        Serial.println();
        Serial.println("***************");
        Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
        Paint_SetRotate(90);
        LCD_Clear(BLACK);
        Paint_DrawString_EN(0, 10, "***************", &Font24, BLACK, WHITE);
        Paint_DrawString_EN(0, 35, "Send Value:", &Font24, BLACK, WHITE);
        Paint_DrawString_EN(0, 58, readBuff1, &Font20, BLACK, 0xfff0); 
        Paint_DrawString_EN(0, 100, "***************", &Font24, BLACK, WHITE);     
        // Reset buffer and size for the next input
        bufferSize = 0;
        readBuff = "";
        memset(readBuff1, 0, sizeof(readBuff1));
      }
    }
  }

  // Your other low-power operations can be added here

  delay(10);  // Introduce a short delay for power efficiency

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }  
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
