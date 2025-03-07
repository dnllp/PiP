/*

Práctica Control de Led con Bluetooth y Arduino
Programación de Interfaces y Puertos
2025
Objetivo: Controlar un LED conectado a Arduino mediante Bluetooth, utilizando un módulo HC-05 o HC-06 y una aplicación móvil.
*/

int ledPin = 13; // Pin del LED
void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
}
void loop() {
  if (Serial.available() > 0) { //verifica sy hay datos disponibles en el puerto serial
    char data = Serial.read(); // Lee el dato recibido por Bluetooth
    if (data == '1') {
      digitalWrite(ledPin, HIGH); // Enciende el LED
      Serial.println("LED ON");
    } else if (data == '0') {
      digitalWrite(ledPin, LOW); // Apaga el LED
      Serial.println("LED OFF");
    }
  }
}
