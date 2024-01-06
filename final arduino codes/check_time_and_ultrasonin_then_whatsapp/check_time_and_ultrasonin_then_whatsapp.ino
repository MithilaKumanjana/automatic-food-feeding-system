#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

const char* ssid = "<enter your ssid>";            // Add your WiFi ssid
const char* password = "<enter your password>";         // Add your WiFi password

String apiKey = "<get epi key>";                  // Add your Token number that bot has sent you on WhatsApp messenger
String phone_number = "<your phone number>";      // Add your WhatsApp app registered phone number (same number that bot sent you in URL)

const int triggerPin = 21;  // Ultrasonic sensor trigger pin
const int echoPin = 32;     // Ultrasonic sensor echo pin

String url;  // URL String will be used to store the final generated URL

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to the WiFi network");

  // Initialize and get the time from NTP server
  configTime(0, 0, "pool.ntp.org");
}

void loop() {
  // Get current time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // Check if it's 6:30 AM
  if (timeinfo.tm_hour == 6 && timeinfo.tm_min == 30) {
    // Read Ultrasonic distance
    float distance = getUltrasonicDistance();
    Serial.printf("Ultrasonic Distance: %.2f cm\n", distance);

    // Check if Ultrasonic distance is larger than 50 cm
    if (distance > 50.0) {
      Serial.println("Sending WhatsApp notification...");
      messageToWhatsApp("Refill the food basket");
      // Add a delay to prevent sending multiple notifications in a short time
      delay(30000);  // 30 seconds delay
    }
  }

  // Add a delay to check the conditions periodically
  delay(60000);  // Check every 1 minute
}

void messageToWhatsApp(String message) {
  // Adding all number, your API key, your message into one complete URL
  url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);

  postData();  // Calling postData to run the above-generated URL once so that you will receive a message.
}

void postData() {
  int httpCode;     // Variable used to get the response HTTP code after calling API
  HTTPClient http;  // Declare object of class HTTPClient
  http.begin(url);  // Begin the HTTPClient object with the generated URL
  httpCode = http.POST(url);  // Finally, Post the URL with this function, and it will store the HTTP code

  if (httpCode == 200) {  // Check if the response HTTP code is 200
    Serial.println("Sent WhatsApp notification.");
  } else {  // If the response HTTP code is not 200, there is some error
    Serial.println("Error sending WhatsApp notification.");
  }

  http.end();  // After calling API end the HTTP client object.
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  char code2;
  
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    
    yield();
  }
  
  return encodedString;
}

float getUltrasonicDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  float duration = pulseIn(echoPin, HIGH);
  return (duration * 0.0343) / 2;  // Calculate distance in cm
}
