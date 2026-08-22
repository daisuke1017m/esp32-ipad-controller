# Apple Developer Bluetooth LE HID アクセサリー要件ガイドライン

iPadOS / iOS / macOS が Bluetooth LE HID (Human Interface Device) キーボードを正規のアクセサリーとして認識し、画面に**ペアリング要求ポップアップ**および **`(i)` アイコン**を表示させるための Apple 公式設計要件（Accessory Design Guidelines for Apple Devices / HOGP 仕様）のまとめです。

---

## 1. 必須要件 (Mandatory Requirements)

### ① GATT 特性の暗号化権限 (Encrypted Characteristic Permissions)
- **要求**: HID レポート特性 (Input/Output/Feature Report) の読み書き権限（Permissions）は **`READ_ENCRYPTED` および `WRITE_ENCRYPTED`** で保護されていなければならない。
- **理由**: 暗号化なしの平文アクセスが許可されている場合、iPadOS は「セキュアなキーボード」とみなさず、暗号化ハンドシェイクを行わずに接続をスキップする。

### ② ATT Insufficient Authentication (0x05) 応答
- **要求**: デバイスが未暗号化状態で接続された際、iPadOS からの HID サービス読み出し要求に対して **`Insufficient Authentication (0x05)`** または **`Insufficient Encryption (0x0F)`** を返さなければならない。
- **理由**: このエラー応答を受け取ることで、iPadOS カーネルの Security Manager が起動し、ユーザー画面に**ペアリング許可ダイヤログ**と **`(i)` マーク**を強制表示させる。

### ③ Security Mode 1 (Level 3 / Level 4)
- **要求**: LE Security Mode 1 (Encrypted connection with bonding) に準拠すること。
- **認証**: Standard Just Works または Passkey/Numeric Comparison を用いた暗号化ボンドキーの生成・保存を行う。

---

## 2. 推奨パラメータ (Recommended Parameters)

- **アドバタイズメント Appearance**: `0x03C1` (HID Keyboard)
- **Primary Service UUID**: `0x1812` (HID Service)
- **アドバタイズ間隔 (Advertising Interval)**: `20ms` ~ `30ms` (発見の高速化のため)

---

## 3. 本プロジェクトへの適用方針

既存の `ESP32_BLE_Keyboard` ライブラリ（`BleKeyboard.cpp`）内の GATT 特性宣言箇所において、従来の権限:
`ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE`

を、Apple ガイドライン準拠の権限:
`ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED`

へ修正し、iPadOS にペアリング・暗号化処理を正しく強制させる。
