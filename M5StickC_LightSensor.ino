#include <M5Unified.h>

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Brightness Level");
}

void loop() {
  // 本体に応じて変更
  // M5StickC/Plus → 33
  // M5Core2 → 35
  // M5Stack Basic/Gray → 36
  int raw = analogRead(33);  // 0〜4095

  // 値を反転（明るい→大きく、暗い→小さく）
  int brightValue = 4095 - raw;

  // スケーリング（0〜5000）
  int level = map(brightValue, 0, 4095, 0, 5000);

  // LCD 表示（前の値を消すために空白追加）
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.printf("Brightness: %4d   ", level);

  // Python 用シリアル出力（数字だけ）
  Serial.println(level);

  delay(100);
}
