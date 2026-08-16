#include <BleKeyboard.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// 15文字以内に収めると切られずに送信されます
BleKeyboard bleKeyboard("ESP32_Control", "ESP32", 100);
BLECharacteristic *pCharacteristic;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        Serial.print("[iPad -> ESP32] 受信: ");
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }
        Serial.println();
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE...");

  // 1. BLE HID（キーボード）の開始
  bleKeyboard.begin();

  // 2. カスタムGATTサービスの設定
  BLEServer *pServer = BLEDevice::getServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  // 3. アドバタイジングの明示的な再設定
  // HID接続中であってもカスタムGATTサービスを検索できるようにする
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  
  // アドバタイジングを強制開始
  BLEDevice::startAdvertising();

  Serial.println("ESP32準備完了。");
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'g' || c == 'G') {
      if (bleKeyboard.isConnected()) {
        Serial.println("[PC -> ESP32 -> iPad] キー 'g' を送信中...");
        bleKeyboard.print("g"); 
      } else {
        Serial.println("iPadとBLE接続されていません。");
      }
    }
  }
  delay(20);
}