/* pins used
A0-A5 - input voltage
D8 - inputPin
D6,D7 - outputPin (display on)
DISPLAY
D13 - TFT_SCL (Clock pin for TFT display)
D11 - TFT_SDA (Data pin for TFT display)
D10 - TFT_CS (Chip Select pin for TFT display)
D4 - TFT_RES (Reset pin for TFT display)
D9 - TFT_DC (Data/Command pin for TFT display)
*/

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 
#include <SPI.h>
#include <VoltageReference.h>

#define TFT_CS 10
#define TFT_RES 4
#define TFT_DC 9
const float VccBatMax = 4.2;
const float VccBatMin = 3.6;

const int inputPin = 8;
const int outputPinG = 7; //display on
const int outputPinP = 6; //display on
int lowCounter = 0;
unsigned long lastLowTime = 0;


VoltageReference vRef = VoltageReference();

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RES);

void setup() {
  Serial.begin(115200);
  vRef.begin(1115550);
  pinMode(inputPin, INPUT);
  pinMode(outputPinG, OUTPUT);
  pinMode(outputPinP, OUTPUT);
  tft.initR(INITR_MINI160x80);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  digitalWrite(outputPinG, LOW);
  digitalWrite(outputPinP, LOW);
}

void DrawBezievCurve (float p) {
  int x0 = 20; int y0 = 40;int x1 = 80; int y1 = (p/100) * 80; int x2 = 140; int y2 = 40;
  for (size_t i = 0; i < 160; i++)
  {
    float t = i / 160.0;
    int x = (1 - t) * (1 - t) * x0 + 2 * (1 - t) * t * x1 + t * t * x2;
    int y = (1 - t) * (1 - t) * y0 + 2 * (1 - t) * t * y1 + t * t * y2;
    tft.drawPixel(x, y, ST7735_WHITE);
  }
  
}
int turnOn () {
    static int lastState = HIGH; 
    int currentState = digitalRead(inputPin);
    static int stateChangeCounter = 0; 
    static unsigned long lastChangeTime = 0; 

    if (currentState != lastState) {
        stateChangeCounter++;
        lastChangeTime = millis(); 
    }

    if (millis() - lastChangeTime > 20000) { 
        stateChangeCounter = 0; 
    }

    lastState = currentState; 
    return stateChangeCounter / 2;
}
float readVcc() {
  float sum = 0;
  float R1 = 20000.0;
  float R2 = 3300.0;
  for (int i = 0; i < 10; i++) {
    int pinV = (float)analogRead(A0) / 1024 * vRef.readVcc();
    sum += pinV;
    delay(50);
  }  float averageVoltage = sum / 10;

  float realVoltage = sum / (R2 / (R1 + R2));
  return realVoltage / 10000;
}
void advanced (){
  float voltage5 = readVcc();
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(ST7735_WHITE);
  tft.print("Voltage5: ");
  tft.print(voltage5);
  tft.println(" V");
  tft.setCursor(10, 30);
  tft.print("Percentage: ");
  tft.print(min((voltage5 - VccBatMin*5) / (VccBatMax*5 - VccBatMin*5) * 100, 100));
  tft.println(" %");
}
void loop() {
  static bool ranOnce = false;
  static bool sleep = true;
  int oscillations = turnOn();
  Serial.println(oscillations);
  
  switch (oscillations) {
    case 3 ... 5:
    if (!ranOnce) {
      digitalWrite(outputPinG, HIGH); 
      digitalWrite(outputPinP, HIGH); 
      tft.fillScreen(ST7735_BLACK);
      DrawBezievCurve(min((readVcc() - VccBatMin*5) / (VccBatMax*5 - VccBatMin*5) * 100, 100));
      ranOnce = true;
      }
      break;
    case 6 ... 100:
      advanced();
      delay(3000);
      break;
    default:
      break;
  }
}