 #include <WiFi.h>
    #include <WebServer.h>
    #include <DHT.h>
    #include <ArduinoJson.h>

    const char* ssid = "TuSSID";
    const char* password = "TuContraseña";

    #define DHTPIN 4
    #define DHTTYPE DHT22
    DHT dht(DHTPIN, DHTTYPE);

    const int ledPin = 2;

    WebServer server(80);

    void handleRoot() {
      server.send(200, "text/plain", "API ESP32");
    }

    void handleTemperature() {
      float t = dht.readTemperature();
      float h = dht.readHumidity();

      if (isnan(t) || isnan(h)) {
        server.send(500, "text/plain", "Error al leer el sensor DHT");
        return;
      }

      StaticJsonDocument doc;
      doc["temperatura"] = t;
      doc["humedad"] = h;
      String response;
      serializeJson(doc, response);

      server.send(200, "application/json", response);
    }

    void handleLedOn() {
      digitalWrite(ledPin, HIGH);
      server.send(200, "text/plain", "LED ON");
    }

    void handleLedOff() {
      digitalWrite(ledPin, LOW);
      server.send(200, "text/plain", "LED OFF");
    }

    void setup() {
      Serial.begin(115200);
      pinMode(ledPin, OUTPUT);
      dht.begin();

      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
      }

      Serial.println("Conectado a WiFi");
      Serial.print("Dirección IP: ");
      Serial.println(WiFi.localIP());

      server.on("/", handleRoot);
      server.on("/temperatura", handleTemperature);
      server.on("/led/on", handleLedOn);
      server.on("/led/off", handleLedOff);
      server.begin();
    }

    void loop() {
      server.handleClient();
    }