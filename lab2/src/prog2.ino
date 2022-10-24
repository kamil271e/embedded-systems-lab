	#include <OneWire.h>
			#include <DS18B20.h>
			#include <SPI.h>
			#include <MFRC522.h>
			#define SS_PIN 10
			#define RST_PIN 9
			MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
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
				mfrc522.PCD_Init();   // Initiate MFRC522
				sensors.begin();
				sensors.request(address);
			}
			
			void loop() {
				float vec[20];
				if (sensors.available()){
					for (int i = 0; i < 18; i++) {
						vec[i] = sensors.readTemperature(address);
					}
					qsort(vec, 18, sizeof(float), sort_desc);
					
					float sum = 0;
					for (int i = 1; i < 17; i++) {
						sum += vec[i];
					}
					sum /= 16;
					
					if ( ! mfrc522.PICC_IsNewCardPresent()) return;
					if ( ! mfrc522.PICC_ReadCardSerial()) return;
					
					String content= "";
					byte letter;
					for (byte i = 0; i < mfrc522.uid.size; i++){
						content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
						content.concat(String(mfrc522.uid.uidByte[i], HEX));
					}
					// adresy odczytane dzieki powyzszej petli:
					// C4 F7 2E D0 // brelok
					// 6D 3D AC 75 // karta
					content.toUpperCase();
					if (content.substring(1) == "BD 31 15 2B"){
						Serial.print(sum);
						Serial.println(F(" 'C"));
					}
					else{
						Serial.println("Brak dostepu");
					}
					sensors.request(address);
				}
			}