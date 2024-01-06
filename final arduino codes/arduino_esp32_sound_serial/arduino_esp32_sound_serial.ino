#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

void setup() {
  tmrpcm.speakerPin = 9;
  Serial.begin(115200);

  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }

  tmrpcm.setVolume(5);
   //tmrpcm.play("Teddy_me.wav");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "up") {
      Serial.println("Playing sound...");
      tmrpcm.play("Teddy_me.wav");
      delay(500); // Adjust delay as needed
    }
  }
}
