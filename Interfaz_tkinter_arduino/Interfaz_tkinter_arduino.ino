// Configuración básica para comunicación serial
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  // Leer sensores o generar datos de ejemplo
  int sensor1 = analogRead(A0);
  int sensor2 = analogRead(A1);
  int sensor3 = analogRead(A2);
  
  // Enviar datos en formato: D1=valor,D2=valor,D3=valor
  Serial.print("D1=");
  Serial.print(sensor1);
  Serial.print(",D2=");
  Serial.print(sensor2);
  Serial.print(",D3=");
  Serial.println(sensor3);
  
  // Procesar comandos recibidos
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.startsWith("LED1=")) {
      digitalWrite(LED_BUILTIN, command.substring(5) == "1" ? HIGH : LOW);
    } else if (command.startsWith("LED2=")) {
      digitalWrite(2, command.substring(5) == "1" ? HIGH : LOW);
    } else if (command.startsWith("LED3=")) {
      digitalWrite(3, command.substring(5) == "1" ? HIGH : LOW);
    }
  }
  
  delay(100); // Pequeña pausa entre lecturas
}