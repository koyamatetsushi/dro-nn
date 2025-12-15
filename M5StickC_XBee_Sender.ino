#include <M5StickCPlus2.h>
 
//===========================
// XBee UART
//  TX = GPIO0
//  RX = GPIO26
//===========================
HardwareSerial XBeeSerial(2);
 
const int sendInterval = 100;   // ms
unsigned long lastSendTime = 0;
 
void setup()
{
  //--------------------------
  // M5 初期化
  //--------------------------
  auto cfg = M5.config();
  M5.begin(cfg);
 
  //--------------------------
  // LCD設定
  //--------------------------
  M5.Display.setRotation(1);
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(WHITE);
  M5.Display.setCursor(0,0);
  M5.Display.println("XBee Sensor TX");
 
  //--------------------------
  // UART
  //--------------------------
  Serial.begin(115200);
  XBeeSerial.begin(9600, SERIAL_8N1, 26, 0);
 
  //--------------------------
  // IMU 初期化
  //--------------------------
  if (!M5.Imu.begin())
  {
    M5.Display.setTextColor(RED);
    M5.Display.println("IMU ERROR");
    while (1);
  }
 
  Serial.println("Sensor TX Start");
}
 
void loop()
{
  M5.update();
 
  if (millis() - lastSendTime >= sendInterval)
  {
    sendSensorData();
    lastSendTime = millis();
  }
 
  delay(5);
}
 
void sendSensorData()
{
  //--------------------------
  // 光センサ
  //--------------------------
  int raw = analogRead(33);
  int bright = 4095 - raw;
  int light = map(bright, 0, 4095, 0, 5000);
 
 
  //--------------------------
  // ラベル付き送信データ作成
  //--------------------------
  String data =
    "BRIGHT=" + String(light);
 
  //--------------------------
  // XBee 送信
  //--------------------------
  XBeeSerial.print(data);
 
  //--------------------------
  // PCデバッグ表示
  //--------------------------
  Serial.print("TX -> ");
  Serial.print(data);
 
  //--------------------------
  // LCD 表示
  //--------------------------
  M5.Display.fillRect(0, 20, 240, 110, BLACK);
  M5.Display.setCursor(0, 20);
  M5.Display.printf("BRIGHT: %4d\n", light);
}
 
 