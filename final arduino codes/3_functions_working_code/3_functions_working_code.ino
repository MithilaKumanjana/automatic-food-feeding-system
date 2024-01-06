#include <WiFi.h>
#include "time.h"
#include <ESP32Servo.h>

const char* ssid = "SAMSUNG J2";
const char* password = "Taboo@123";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 5 * 3600 + 30 * 60; // Adjust according to your timezone
const int daylightOffset_sec = 0;

// Servo control variables
Servo MagicServo;

// Flag to track if "up" has been sent
bool upSent = false;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  // Initialize and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Attach the servo motor to GPIO 26 on ESP32
  MagicServo.attach(26);
}

void moveServo(int degrees) {
  MagicServo.write(degrees);
  delay(2000); // Adjust the delay as needed
}

void loop() {
  // Get current time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // Check if it's 4:30 PM and "up" has not been sent yet
  if (timeinfo.tm_hour == 16 && timeinfo.tm_min == 30 && !upSent) {
    // Move the servo motor to the 90-degree position
    moveServo(90);
    Serial.println("servo works");
    Serial.println("up");
    // Move the servo motor to the 0-degree position
    delay(500);
    moveServo(0);
    // Set the flag to true to indicate that "up" has been sent
    upSent = true;
  }

  // Check if it's 4:35 PM and "up" has not been sent yet
  if (timeinfo.tm_hour == 16 && timeinfo.tm_min == 35 && !upSent) {
    moveServo(90);
    Serial.println("servo works");
    Serial.println("up");
    delay(500);
    moveServo(0);
    upSent = true;
  }

  // Check if it's 4:40 PM and "up" has not been sent yet
  if (timeinfo.tm_hour == 16 && timeinfo.tm_min == 40 && !upSent) {
    moveServo(90);
    Serial.println("servo works");
    Serial.println("up");
    delay(500);
    moveServo(0);
    upSent = true;
  }

  delay(1000); // Wait for 1 second before checking the time again
}
