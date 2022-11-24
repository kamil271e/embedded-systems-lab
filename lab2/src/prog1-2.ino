#include <OneWire.h>
#include <DS18B20.h>
#include <SPI.h>
#include <MFRC522.h>

#define ONEWIRE_PIN 2

// Adres czujnika
byte address[8] = {0x28, 0xFF, 0xBC, 0x88, 0x90, 0x17, 0x5, 0x76};

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);

int sort_desc(const void *cmp1, const void *cmp2){
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  return a > b ? -1 : (a < b ? 1 : 0);
}

void setup() {
  while(!Serial);
  Serial.begin(9600);
  SPI.begin();      // Initiate  SPI bus
  sensors.begin();
  sensors.request(address);
}

void loop() {
  float vec[20];
  if (sensors.available())
  {
    for (int i = 0; i < 18; i++) {
      vec[i] = sensors.readTemperature(address);
    }
    qsort(vec, 18, sizeof(float), sort_desc);

    float sum = 0;
    for (int i = 1; i < 17; i++) {
      sum += vec[i];
    }
    sum /= 16;

    Serial.print(sum);
    Serial.println(F(" 'C"));
    sensors.request(address);
  }
}
