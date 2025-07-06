// Simple BLE Relay Controller - Compatible with Waveshare protocol
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Relay control pins (based on Waveshare ESP32-S3-Relay-6CH board)
const int RELAY_PINS[6] = {1, 2, 41, 42, 45, 46};
const int BUZZER_PIN = 21;

// Relay states
bool relayStates[6] = {false, false, false, false, false, false};

// BLE
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Forward declarations
void toggleRelay(int relay);
void allOff();

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device connected");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Device disconnected");
      // Restart advertising
      delay(500);
      pServer->startAdvertising();
      Serial.println("Restarted advertising");
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String value = pCharacteristic->getValue();

      if (value.length() > 0) {
        uint8_t cmd = value[0];
        Serial.print("Received command: 0x");
        Serial.println(cmd, HEX);
        
        // Waveshare protocol
        switch(cmd) {
          case 0x31: toggleRelay(0); break;
          case 0x32: toggleRelay(1); break;
          case 0x33: toggleRelay(2); break;
          case 0x34: toggleRelay(3); break;
          case 0x35: toggleRelay(4); break;
          case 0x36: toggleRelay(5); break;
          case 0x38: allOff(); break;
        }
      }
    }
};

void toggleRelay(int relay) {
  if (relay >= 0 && relay < 6) {
    relayStates[relay] = !relayStates[relay];
    digitalWrite(RELAY_PINS[relay], relayStates[relay] ? HIGH : LOW);
    
    // Beep feedback
    tone(BUZZER_PIN, relayStates[relay] ? 1000 : 500, 50);
    
    Serial.print("Relay ");
    Serial.print(relay + 1);
    Serial.print(" is now ");
    Serial.println(relayStates[relay] ? "ON" : "OFF");
  }
}

void allOff() {
  for (int i = 0; i < 6; i++) {
    relayStates[i] = false;
    digitalWrite(RELAY_PINS[i], LOW);
  }
  tone(BUZZER_PIN, 500, 100);
  Serial.println("All relays OFF");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Relay Controller...");
  
  // Initialize relay pins
  for (int i = 0; i < 6; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], LOW);
  }
  
  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Create BLE Device
  BLEDevice::init("ESP32 S3 Relay 6CH");
  
  // Create BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Ready");
  
  // Start the service
  pService->start();
  
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  
  Serial.println("BLE device is ready!");
  Serial.println("Device name: ESP32 S3 Relay 6CH");
  
  // Startup beep
  tone(BUZZER_PIN, 262, 200); delay(250);
  tone(BUZZER_PIN, 330, 200); delay(250);
  tone(BUZZER_PIN, 392, 200); delay(250);
  tone(BUZZER_PIN, 523, 400);
}

void loop() {
  // Check for serial commands for testing
  if (Serial.available()) {
    char cmd = Serial.read();
    switch(cmd) {
      case '1': toggleRelay(0); break;
      case '2': toggleRelay(1); break;
      case '3': toggleRelay(2); break;
      case '4': toggleRelay(3); break;
      case '5': toggleRelay(4); break;
      case '6': toggleRelay(5); break;
      case '0': allOff(); break;
    }
  }
  
  delay(20);
}