#include <ESP32Servo.h>

//acenderledaodetectarpresenca
const int PIR = 5;
const int led = 2;

//portaautomatica
Servo motor;
const int Servomotor = 26;

void acenderledaodetectarpresenca() {
  int estadoPIR = digitalRead(PIR);

  if (estadoPIR == LOW) {
    digitalWrite(led, LOW);
    Serial.println("LUZ desligada");
  } else {
    digitalWrite(led, HIGH);
    Serial.println("LUZ ligada");
  }
}

void portaAutomatica(){
 motor.write(125);
 delay(5000);
 motor.write(0);
}


void setup() {
  pinMode(led, OUTPUT);
  pinMode(PIR, INPUT);

  motor.attach(Servomotor);
  motor.write(0);

  Serial.begin(9600); 

 portaAutomatica();
}

void loop() {

 
//  portaAutomatica();
acenderledaodetectarpresenca();

}
