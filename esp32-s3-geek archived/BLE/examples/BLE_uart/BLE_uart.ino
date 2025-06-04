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
#define DEV_BL_PIN  7

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
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);

        Serial.println();
        Serial.println("*********");
      }
    }
};

void sendBluetoothData(uint8_t *data, size_t length) {
  if (deviceConnected) {
    pTxCharacteristic->setValue(data, length);
    pTxCharacteristic->notify();
  }
}

void setup() {
  analogWrite(DEV_BL_PIN,0);
  Serial.begin(115200);

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

void loop() {

  // Read all available data from Serial and send it via BLE
  if (Serial.available() > 0) {
    static char buffer[256];  // Buffer to store input data
    size_t bufferSize = 0;  // Current size of data in buffer

    while (Serial.available() > 0) {
      char input = Serial.read();
      buffer[bufferSize++] = input;  // Store input in buffer
      // Check if the buffer is full, or a newline character is received
      if (bufferSize >= sizeof(buffer) || input == '\n') {
        // Send the entire buffer via BLE
        sendBluetoothData((uint8_t*)buffer, bufferSize);
        Serial.println("*********");
        Serial.print("Send Value: ");
        Serial.print(buffer);
        Serial.println("*********");
      
        // Reset buffer and size for the next input
        bufferSize = 0;
        memset(buffer, 0, sizeof(buffer));
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
