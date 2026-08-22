#include <HijelHID_BLEKeyboard.h>

HijelHID_BLEKeyboard keyboard;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // JustWorks パターン（ペアリング要求+iマーク）
  keyboard.begin();

  Serial.println("【準備完了】iPadの「設定 > Bluetooth」で接続してください。");
}

void loop() {
  if (keyboard.isConnected()) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '\r' || c == '\n') return;
      Serial.print("送信: ");
      Serial.println(c);
      keyboard.print(c);
    }
  }
  delay(20);
}
