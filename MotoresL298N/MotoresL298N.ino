    /*

Práctica Control de Motores DC con Puente H L298N
Programación de Interfaces y Puertos
2025
Objetivo: Conectar y controlar la dirección y velocidad de motores DC utilizando el puente H L298N y Arduino.
*/
    // Pines de control para el Motor A
    int motorA_IN1 = 8;
    int motorA_IN2 = 9;
    int motorA_ENA = 3; // Pin PWM

    // Pines de control para el Motor B
    int motorB_IN3 = 10;
    int motorB_IN4 = 11;
    int motorB_ENB = 5; // Pin PWM

    void setup() {
      // Configurar pines como salidas
      pinMode(motorA_IN1, OUTPUT);
      pinMode(motorA_IN2, OUTPUT);
      pinMode(motorA_ENA, OUTPUT);
      pinMode(motorB_IN3, OUTPUT);
      pinMode(motorB_IN4, OUTPUT);
      pinMode(motorB_ENB, OUTPUT);
    }

    void loop() {
      // Motor A hacia adelante, velocidad máxima
      digitalWrite(motorA_IN1, HIGH);
      digitalWrite(motorA_IN2, LOW);
      analogWrite(motorA_ENA, 255);

      // Motor B hacia atrás, velocidad media
      digitalWrite(motorB_IN3, LOW);
      digitalWrite(motorB_IN4, HIGH);
      analogWrite(motorB_ENB, 150);

      delay(2000); // Esperar 2 segundos

      // Motor A hacia atrás, velocidad baja
      digitalWrite(motorA_IN1, LOW);
      digitalWrite(motorA_IN2, HIGH);
      analogWrite(motorA_ENA, 100);

      // Motor B hacia adelante, velocidad máxima
      digitalWrite(motorB_IN3, HIGH);
      digitalWrite(motorB_IN4, LOW);
      analogWrite(motorB_ENB, 255);

      delay(2000); // Esperar 2 segundos

      // Detener ambos motores
      digitalWrite(motorA_IN1, LOW);
      digitalWrite(motorA_IN2, LOW);
      digitalWrite(motorB_IN3, LOW);
      digitalWrite(motorB_IN4, LOW);
      analogWrite(motorA_ENA, 0);
      analogWrite(motorB_ENB, 0);

      delay(2000); // Esperar 2 segundos
    }
