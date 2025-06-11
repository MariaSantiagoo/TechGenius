#include <ESP32Servo.h>
//#include <Servo.h>

//acenderledaodetectarpresenca
const int PIR = 5;
const int led = 2;

//motor rele
const int rele = 15;

int contador = 0; // avança para dentro de casa

Servo motor;  //variavel servo
const int servoMotor = 18;


int senhaD = 0;
const int SenhaR = 231;

//verificarvazamentodegas
const int MQ135 = 34;
const int bagulhotagarela = 12;

//sistema de trancar a porta

void reletrancar() {
  digitalWrite(rele, LOW);
  Serial.println("trancado");
}

void reledestrancar() {
  digitalWrite(rele, HIGH);
  Serial.println("destrancado");
}

//porta

void fecharporta(){
//motor.write(0);
}

void abrirporta(){
Serial.println("Diigite sua senha");
while ( ! Serial.available());
senhaD = Serial.parseInt(); 

if (senhaD == SenhaR){
Serial.println("Porta aberta");
reledestrancar();
//motor.write(125);
delay(3000);
fecharporta();
contador = 1;
} 
else{
Serial.println("Senha incorreta"); 
reletrancar();
fecharporta();
}
}

//led -> luz

void acenderledaodetectarpresenca() {
  int estadoPIR = digitalRead(PIR);

  if (estadoPIR == HIGH) {
    digitalWrite(led, HIGH);
    Serial.println("LUZ ligada");
  } else {
    digitalWrite(led, LOW);
    Serial.println("LUZ desligada");
  }
}

//vazamento de gas

void verificarvazamentodegas() {
  int estadoMQ135 = analogRead(MQ135);
  if (estadoMQ135 >= 200) {
    alarme_dois_tons();
  } else {
    noTone(bagulhotagarela);
  }
}

void alarme_dois_tons() {
  int freqAlta = 2000;
  int freqBaixa = 800;
  int duracaoTom = 250;

  tone(bagulhotagarela, freqAlta, duracaoTom);
  delay(duracaoTom);
  tone(bagulhotagarela, freqBaixa, duracaoTom);
  delay(duracaoTom);
}

void setup() {

pinMode(led, OUTPUT);
pinMode(PIR, OUTPUT);

motor.attach(servoMotor);
motor.write(0);

pinMode(rele, OUTPUT);
digitalWrite(rele, LOW);

pinMode(MQ135, INPUT);
pinMode(bagulhotagarela, OUTPUT);
pinMode(rele, OUTPUT);

Serial.begin(9600);
}

void loop() {
 do{
 
 fecharporta();
 abrirporta(); 

 }
 while (contador != 0);

acenderledaodetectarpresenca();

}
