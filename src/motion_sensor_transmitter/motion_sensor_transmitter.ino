#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <string.h>

#define IRA_PIN   2
#define CE_PIN    9
#define CSN_PIN   10
#define MISO_PIN  12
#define SCLK_PIN  13

#define MOSI_PIN  11


const byte ADDRESS[6] = "00001";

RF24 radio (CE_PIN, CSN_PIN);

void setup() {

  pinMode(IRA_PIN, INPUT);
  pinMode(CSN_PIN, OUTPUT);

  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(ADDRESS);
  radio.setRetries(0, 0);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  delay(2000);

}

void loop() {

  if (!radio.isChipConnected())
    Serial.println("Chip not connected");

  if (digitalRead(IRA_PIN) == HIGH)
    broadcastMessage();

}

void broadcastMessage() {
  const char message[] = "activate_sfw_mode";
  radio.write(message, sizeof(message));
  Serial.println(message);
  delay(3000);
}
