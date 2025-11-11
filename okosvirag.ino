#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// eszkoz
const int soilPin = A0;
const int relayPin = 7;
#define DHT11PIN 4

// kijelzo
dht11 DHT11;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// beallitasok
const int dryThreshold = 20;
const unsigned long pumpTime = 1000UL; 

// allapot
bool pumpOn = false;
unsigned long pumpStart = 0;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Okosvirag ON");
  lcd.setCursor(0,1); lcd.print("Udvozol a virag!");
  delay(1500);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();

  //meres
  int sensorValue = analogRead(soilPin);
  int moisturePercent = map(sensorValue, 1023, 0, 0, 100);

  int chk = DHT11.read(DHT11PIN);
  float humidity = (float)DHT11.humidity;
  float temperature = (float)DHT11.temperature;

  Serial.print("Soil:"); Serial.print(moisturePercent); Serial.print("% ");
  Serial.print("T:"); Serial.print(temperature); Serial.print("C ");
  Serial.print("H:"); Serial.print(humidity); Serial.println("%");

  // pumpa kezdes off
  if (!pumpOn && moisturePercent < dryThreshold) {
    // pumpa on
    digitalWrite(relayPin, HIGH);
    pumpOn = true;
    pumpStart = now;
    Serial.println("Pumpa ON");
    
  }


  if (pumpOn) {
    if (now - pumpStart >= pumpTime) {
      digitalWrite(relayPin, LOW);
      pumpOn = false;
      Serial.println("Pumpa OFF");
      // kijelzo frissites
      delay(50);
    } else {
   
    }
  } else {
    //kijelzo ha pumpa off
    lcd.setCursor(0,0);
    lcd.print("T:");
    lcd.print(temperature,1);
    lcd.print((char)223);
    lcd.print("C ");

    lcd.setCursor(10,0);
    lcd.print("H:");
    lcd.print(humidity,0);
    lcd.print("% ");

    lcd.setCursor(0,1);
    lcd.print("Soil:");
    lcd.print(moisturePercent);
    lcd.print("%    "); 

    lcd.setCursor(11,1);
    lcd.print("     "); 
  }


  delay(2000); // kesleltetes a meresek kozott
}
