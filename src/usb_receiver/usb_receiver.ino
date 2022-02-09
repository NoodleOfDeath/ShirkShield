#include <Keyboard.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <string.h>

#define SWT_PIN   2
#define CE_PIN    9
#define CSN_PIN   10
#define MISO_PIN  14
#define SCLK_PIN  15
#define MOSI_PIN  16
#define RXLED     17

enum Platform { MACOSX, WINDOWS, LINUX };

const byte ADDRESS[6] = "00001";

RF24 radio (CE_PIN, CSN_PIN);

void setup() {

  pinMode(RXLED, OUTPUT);
  pinMode(SWT_PIN, INPUT);
  pinMode(CSN_PIN, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Fuck");
  radio.begin();
  radio.openReadingPipe(0, ADDRESS);
  radio.setRetries(0, 0);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  Keyboard.begin();
  
}

void loop() {

  if (!radio.isChipConnected())
    Serial.println("Chip not connected");

  if (radio.available()) {
    char message[32] = "";
    radio.read(&message, sizeof(message));
    if (strlen(message) > 0) {
      Serial.println(message);
      if (strncmp(message, "activate_sfw_mode", sizeof(message)) == 0) {
        radio.stopListening();
        activateSFWMode();
        delay(3000);
        radio.startListening();
      }
    }
  }
  
}

Platform getPlatform() {
  if (digitalRead(SWT_PIN) == HIGH) {
    Serial.println("Mac Mode");
    return MACOSX;
  }
  Serial.println("Windows mode");
  return WINDOWS;
}

void activateSFWMode() {

  Serial.println("Activating SFW Mode!");
  digitalWrite(RXLED, LOW);

  Platform platform = getPlatform();
  
  switch (platform) {

  case MACOSX:
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('Q');
    delay(500);
    Keyboard.releaseAll();
    Keyboard.write(KEY_RETURN);
    break;

  case WINDOWS:
  case LINUX:
  default:
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_DELETE);
    delay(500);
    Keyboard.releaseAll();
    Keyboard.write(KEY_RETURN);
    break;
    
  }
  
}

void deactivateSFWMode() {
  digitalWrite(RXLED, HIGH);
}
