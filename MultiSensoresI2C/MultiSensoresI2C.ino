/*

Práctica Lectura de Múltiples Sensores I2C (DHT22 y BMP280)
Programación de Interfaces y Puertos
2025
Objetivo: Aprender a conectar y leer datos de múltiples sensores I2C (DHT22 y BMP280) utilizando Arduino o ESP32.

*/    
    
    
    #include <Wire.h>
    #include "Adafruit_Sensor.h"
    #include "Adafruit_BMP280.h"
    #include "DHT.h"

    #define DHTPIN 2     // Pin digital conectado al DHT22
    #define DHTTYPE DHT22   // Tipo de sensor DHT

    DHT dht(DHTPIN, DHTTYPE);
    Adafruit_BMP280 bmp; // Objeto BMP280

    void setup() {
      Serial.begin(9600);
      Serial.println(F("Lectura de sensores I2C"));

      dht.begin();
      if (!bmp.begin()) {
        Serial.println(F("No se pudo encontrar el sensor BMP280"));
        while (1) delay(10);
      }
    }

    void loop() {
      delay(2000); // Espera entre lecturas

      // Lectura del DHT22
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      // Lectura del BMP280
      float temp = bmp.readTemperature();
      float pressure = bmp.readPressure();

      // Mostrar datos en el Monitor Serial
      Serial.print(F("Humedad: "));
      Serial.print(h);
      Serial.print(F("%  Temperatura (DHT22): "));
      Serial.print(t);
      Serial.println(F("°C"));

      Serial.print(F("Temperatura (BMP280): "));
      Serial.print(temp);
      Serial.print(F("°C  Presión: "));
      Serial.print(pressure / 100.0F);
      Serial.println(F("hPa"));
    }
