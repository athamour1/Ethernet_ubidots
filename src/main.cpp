
#include <Arduino.h>
#include <SimpleDHT.h>
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>

char const * TOKEN = "token_here"; // Assign your Ubidots TOKEN
char const * VARIABLE_LABEL_1 = "temperature"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_2 = "humidity"; // Assign the unique variable label to send the data

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

Ubidots client(TOKEN);

int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

void setup() {
  Serial.begin(9600);
  Serial.print(F("Starting ethernet..."));
  if (!Ethernet.begin(mac)) {
    Serial.println(F("failed"));
  } else {
    Serial.println(Ethernet.localIP());
  }
  delay(2000);
  Serial.println(F("Ready"));
}

void loop() {

  Ethernet.maintain();

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" H");

  /* Sensors readings */
  float value_1 = temperature;
  float value_2 = humidity;
  /* Sending values to Ubidots */
  client.add(VARIABLE_LABEL_1, value_1);
  client.add(VARIABLE_LABEL_2, value_2);
  client.sendAll();

  delay(5000);
}