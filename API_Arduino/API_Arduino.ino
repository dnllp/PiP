/*

Práctica API con Arduino para Control de Sensores y Actuadores
Programación de Interfaces y Puertos
2025
Objetivo: Configurar Arduino como un servidor web que expone una API para leer datos de sensores y controlar actuadores a través de solicitudes HTTP.
*/ 

    #include <SPI.h>
    #include <Ethernet.h>
    #include "DHT.h"

    #define DHTPIN 2
    #define DHTTYPE DHT11
    DHT dht(DHTPIN, DHTTYPE);

    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    IPAddress ip(192, 168, 1, 177);
    EthernetServer server(80);

    const int ledPin = 8;

    void setup() {
      pinMode(ledPin, OUTPUT);
      digitalWrite(ledPin, LOW);
      Serial.begin(9600);
      Ethernet.begin(mac, ip);
      server.begin();
      dht.begin();
    }

    void loop() {
      EthernetClient client = server.available();
      if (client) {
        String request = client.readStringUntil('\r');
        client.flush();
        if (request.indexOf("/temperatura") != -1) {
          float temp = dht.readTemperature();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close");
          client.println();
          client.print("{\"temperatura\":");
          client.print(temp);
          client.println("}");
        } else if (request.indexOf("/led/on") != -1) {
          digitalWrite(ledPin, HIGH);
          client.println("HTTP/1.1 200 OK");
          client.println("Connection: close");
          client.println();
          client.println("LED ON");
        } else if (request.indexOf("/led/off") != -1) {
          digitalWrite(ledPin, LOW);
          client.println("HTTP/1.1 200 OK");
          client.println("Connection: close");
          client.println();
          client.println("LED OFF");
        } else {
          client.println("HTTP/1.1 404 Not Found");
          client.println("Connection: close");
          client.println();
        }
        client.stop();
      }
    }
