#include <HijelHID_BLEKeyboard.h>

HijelHID_BLEKeyboard keyboard;

// Google Maps アプリを起動する関数
void openGoogleMaps() {
  if (!keyboard.isConnected()) {
    Serial.println("【エラー】iPadがBluetooth接続されていません。");
    return;
  }

  Serial.println("【実行】iPadで Google Maps を起動中...");

  // 1. Cmd + Space を送信（Spotlight検索を起動）
  keyboard.tap(KEY_SPACE, KEY_MOD_LGUI);
  delay(500); // Spotlightが開くまで待機

  // 2. "Google Maps" と入力
  keyboard.print("Google Maps");
  delay(300); // 入力反映を待つ

  // 3. Enterキーを押してアプリを起動
  keyboard.tap(KEY_RETURN);
  delay(100);

  Serial.println("【完了】Google Maps の起動コマンドを送信しました。");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== iPad Google Maps Controller ===");

  keyboard.begin();

  Serial.println("【準備完了】iPadのBluetoothで「HijelHID KB」に接続してください。");
  Serial.println("シリアル通信で 'map' または 'g' と入力すると Google Maps を起動します。");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equalsIgnoreCase("map") || input.equalsIgnoreCase("g") || input.length() > 0) {
      openGoogleMaps();
    }
  }
  delay(20);
}
