#include <M5StickCPlus2.h>

// ===============================

// XBee UART 設定

//   TX = GPIO0

//   RX = GPIO26

// ===============================

HardwareSerial XBeeSerial(2);

int messageCount = 0;

String receivedData = "";

unsigned long lastSendTime = 0;

void updateDisplay();

void sendTestMessage();

void setup()

{

  //--------------------------------

  // M5 初期化

  //--------------------------------

  auto cfg = M5.config();

  M5.begin(cfg);

  M5.Display.setRotation(1);

  M5.Display.fillScreen(BLACK);

  M5.Display.setTextColor(WHITE);

  M5.Display.setTextSize(1);

  //--------------------------------

  // PC シリアル開始（デバッグ）

  //--------------------------------

  Serial.begin(115200);

  Serial.println("==== XBee Communication Test Start ====");

  //--------------------------------

  // XBee UART 初期化

  // begin(baud, config, RX, TX)

  //--------------------------------

  XBeeSerial.begin(9600, SERIAL_8N1, 26, 0);

  //--------------------------------

  // 初期表示

  //--------------------------------

  updateDisplay();

}

void loop()

{

  M5.update();

  //--------------------------------

  // ボタンA：送信

  //--------------------------------

  if (M5.BtnA.wasPressed())

  {

    sendTestMessage();

  }

  //--------------------------------

  // ボタンB：画面リセット

  //--------------------------------

  if (M5.BtnB.wasPressed())

  {

    receivedData = "";

    messageCount = 0;

    updateDisplay();

  }

  //--------------------------------

  // XBee 受信処理

  //--------------------------------

  while (XBeeSerial.available())

  {

    char c = XBeeSerial.read();

    receivedData += c;

    // 改行で1メッセージとみなす

    if (c == '\n')

    {

      Serial.print("Received : ");

      Serial.print(receivedData);

      // 表示用に長さ制限

      if (receivedData.length() > 100)

      {

        receivedData =

          receivedData.substring(receivedData.length() - 100);

      }

      updateDisplay();

    }

  }

  //--------------------------------

  // 自動送信（10秒毎）

  //--------------------------------

  if (millis() - lastSendTime > 10000)

  {

    sendTestMessage();

  }

  delay(10);

}

//================================

// 送信処理

//================================

void sendTestMessage()

{

  messageCount++;

  String msg = "M5->XBee #" + String(messageCount) + "\n";

  XBeeSerial.print(msg);

  Serial.print("Sent : ");

  Serial.print(msg);

  lastSendTime = millis();

  updateDisplay();

}

//================================

// 画面更新処理

//================================

void updateDisplay()

{

  M5.Display.fillScreen(BLACK);

  M5.Display.setCursor(0, 0);

  //--------------------------------

  // タイトル

  //--------------------------------

  M5.Display.setTextColor(GREEN);

  M5.Display.println("XBee Test");

  M5.Display.println("----------------");

  //--------------------------------

  // 送信カウント表示

  //--------------------------------

  M5.Display.setTextColor(YELLOW);

  M5.Display.print("Sent : ");

  M5.Display.println(messageCount);

  M5.Display.println();

  //--------------------------------

  // 受信データ表示

  //--------------------------------

  M5.Display.setTextColor(CYAN);

  M5.Display.println("Received:");

  M5.Display.setTextColor(WHITE);

  int startPos = max(0, (int)receivedData.length() - 60);

  String displayData = receivedData.substring(startPos);

  M5.Display.println(displayData);

  //--------------------------------

  // ボタン説明

  //--------------------------------

  M5.Display.setCursor(0, 120);

  M5.Display.setTextColor(ORANGE);

  M5.Display.println("A:Send  B:Clear");

}
 