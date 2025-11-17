// M5StickC Plus2の内蔵IMU（ジャイロセンサ）から角速度を取得し、
// LCDに表示すると同時に、PCへPythonで受信するためのカンマ区切りデータを送信するスケッチ。
 
#include <M5StickCPlus2.h>
 
// データを更新する間隔（ミリ秒）
const int dataUpdateInterval = 50; // 20Hz (1000ms / 50ms)
 
void setup() {
  // M5StickC Plus2の初期化
  M5.begin();
 
  // シリアル通信の開始
  Serial.begin(115200);
 
  // LCDの初期設定
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print("Gyro Data to Python");
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.print("X, Y, Z (deg/s):");
 
  // IMUの初期化
  if (!M5.Imu.begin()) {
    M5.Lcd.setCursor(10, 50);
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.print("IMU init failed!");
    while (1); // 初期化失敗時は停止
  }
}
 
void loop() {
  float gx = 0.0f;
  float gy = 0.0f;
  float gz = 0.0f;
 
  // ジャイロデータを取得
  M5.Imu.getGyro(&gx, &gy, &gz);
 
  // --- LCD表示 ---
  M5.Lcd.setTextColor(RED, BLACK);
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.printf("X: %+7.2f", gx);
 
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.printf("Y: %+7.2f", gy);
 
  M5.Lcd.setTextColor(BLUE, BLACK);
  M5.Lcd.setCursor(10, 100);
  M5.Lcd.printf("Z: %+7.2f", gz);
 
  // --- PC側Pythonへの送信 ---
  Serial.print(gx, 2);
  Serial.print(",");
  Serial.print(gy, 2);
  Serial.print(",");
  Serial.println(gz, 2);
 
  delay(dataUpdateInterval);
  M5.update();
}