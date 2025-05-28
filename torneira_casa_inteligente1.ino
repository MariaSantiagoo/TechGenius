#include <Servo.h> 

int distancia = 0;
int ledBranco = 5; 

long distanciaUltrasonica(int TRIG, int ECHO)
{
  pinMode(TRIG, OUTPUT);  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);//porta digital 3
  pinMode(ECHO, INPUT);//porta digital 2

  return pulseIn(ECHO, HIGH);
}

Servo servo_4;

void setup()
{
  Serial.begin(9600);
  servo_4.attach(4);//O sinal(servo) conectado na porta 4
  pinMode(ledBranco, OUTPUT);					 
}

void loop()
{
  distancia = 0.01723 * distanciaUltrasonica(3, 2);
  if (distancia <= 35) {
    servo_4.write(90);
    digitalWrite(ledBranco, HIGH); //servo: biblioteca para apontar o eixo do motor para o angulo definido 
  }else {
    digitalWrite(ledBranco, LOW);				//servo é um atuador rotativo para controle de posição gira até a posição do comando co}
  }
    delay(500); 
    servo_4.write(0);


  }