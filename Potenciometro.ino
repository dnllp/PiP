#include<Servo.h>
Servo tilin;
int led = 3;
void setup()
{
 Serial.begin(9600); //Comunicación serial con la computadora
 pinMode(led,OUTPUT);
 tilin.attach(7); //Conectamos el servo en el pin 7
 tilin.write(90); //Posición inicial del servo
 delay(500);
}

void loop()
{
  int pot; int intensidad; int pos;
  pot = analogRead(A0); //ADC de 0v - 5v a 0 - 1023, 10 bits
  Serial.println(pot);
  delay(50);
  intensidad = map(pot,0,1023,0,255);
  analogWrite(led,intensidad);
  //analogWrite(led,pot/4);
  pos = map(pot,0,1023,180,0);
  tilin.write(pos);
 
}
