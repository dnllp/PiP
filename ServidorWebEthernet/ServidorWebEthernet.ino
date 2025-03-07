/*

Práctica Servidor Web con Arduino y Ethernet para control de Led
Programación de Interfaces y Puertos
2025
Objetivo: Configurar Arduino como un servidor web, conectar a una red Ethernet y controlar un LED a través de una página web.
*/

    #include <SPI.h>
    #include <Ethernet.h>

    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Dirección MAC (puedes cambiarla)
    IPAddress ip(192, 168, 1, 177); // Dirección IP (ajusta según tu red)
    EthernetServer server(80); // Puerto 80 para HTTP

    const int ledPin = 8;

    void setup() {
      pinMode(ledPin, OUTPUT);
      digitalWrite(ledPin, LOW);
      Serial.begin(9600);

      Ethernet.begin(mac, ip);
      server.begin();

      Serial.print("Dirección IP del servidor: ");
      Serial.println(Ethernet.localIP());
    }

    void loop() {
      EthernetClient client = server.available();
      if (client) {
        Serial.println("Nuevo cliente");
        boolean currentLineIsBlank = true;
        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            Serial.write(c);
            if (c == '\n' && currentLineIsBlank) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              client.println("<html>");
              client.println("<head><title>Control LED</title></head>");
              client.println("<body>");
              client.println("<h1>Control de LED</h1>");
              client.println("<a href=\"/H\"><button>Encender</button></a>");
              client.println("<a href=\"/L\"><button>Apagar</button></a><br />");

              if (Serial.find("/H")) {
                digitalWrite(ledPin, HIGH);
                client.println("LED Encendido");
              }
              if (Serial.find("/L")) {
                digitalWrite(ledPin, LOW);
                client.println("LED Apagado");
              }

              client.println("</body></html>");
              break;
            }
            if (c == '\n') {
              currentLineIsBlank = true;
            } else if (c != '\r') {
              currentLineIsBlank = false;
            }
          }
        }
        client.stop();
        Serial.println("Cliente desconectado");
      }
    }
