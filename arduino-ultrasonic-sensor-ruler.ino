#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;

// defines variables
long duration;
int distance;
int height;
int filtered_distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();

  drawRule(5, 25, "4m", 10, false, false);
  drawRule(2, 20, "1m", 31, false, true);
  drawRule(5, 10, "cm", 57, true, false);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;
  distance = 0.5 * distance + (1-0.5)*filtered_distance;
  filtered_distance = distance;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);

  clearRuleLines(10, 31, 57);
  drawRuleLine(400, 10);
  drawRuleLine(100, 31);
  drawRuleLine(10, 57);

  display.display();
  delay(10);
}

void drawRule(int scale, int marker, String unit, int rule_height, bool alt, bool double_val) {
  for (int i = 0; i <= 100; i = i + scale) {
    if (i % marker == 0) {
      height = 4;
      if (alt != true || alt == true && (i / marker) % 2 == 0) {
        display.setCursor(i - (i / marker * 2) / 10 * 3, (SCREEN_HEIGHT - (rule_height - 2)));
        if (double_val) {
          display.print(i / marker * 2);
        }
        else {
          display.print(i / marker);
        }
      }
    }
    else {
      height = 2;
    }
    display.drawLine(i + 2, (SCREEN_HEIGHT - rule_height) - 1, i + 2, (SCREEN_HEIGHT - rule_height) - height, WHITE);
    display.setCursor(115, SCREEN_HEIGHT - (rule_height + 5));
    display.print(unit);
  }
}

void drawRuleLine(int scale, int height) {
  int len = map(distance, 0, scale, 2, 102);
  display.drawLine(2, SCREEN_HEIGHT - (height + 7), len, SCREEN_HEIGHT - (height + 7), WHITE);
  display.drawLine(103, SCREEN_HEIGHT - (height + 7), 128, SCREEN_HEIGHT - (height + 7), BLACK);
}

void clearRuleLines(int h1, int h2, int h3) {
  display.drawLine(2, SCREEN_HEIGHT - (h1 + 7), 102, SCREEN_HEIGHT - (h1 + 7), BLACK);
  display.drawLine(2, SCREEN_HEIGHT - (h2 + 7), 102, SCREEN_HEIGHT - (h2 + 7), BLACK);
  display.drawLine(2, SCREEN_HEIGHT - (h3 + 7), 102, SCREEN_HEIGHT - (h3 + 7), BLACK);
}
