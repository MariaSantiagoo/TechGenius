
//bibliotecas
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <Servo.h>

//acenderledaodetectarpresenca
const int PIR = 2; 
const int led = 13;

//verificarvazamentodegas
const int MQ135 = A0;
const int bagulhotagarela = 12;   

Servo motor; //variavel servo

//motor rele
const int rele = 7;

void acenderledaodetectarpresenca(){ 
int estadoPIR = digitalRead(PIR);
 
if(estadoPIR == HIGH){
digitalWrite(led, HIGH);
Serial.println("LED ligado");
}
else{
digitalWrite(led, LOW);
Serial.println("LED desligado");
}
}
void verificarvazamentodegas(){
int estadoMQ135 = analogRead(MQ135);
if(estadoMQ135 >= 200){
  alarme_dois_tons();
}
else{
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

#define DHTPINO A1
#define DHTTYPE DHT11 

DHT dht(DHTPINO, DHT11);
LiquidCrystal_I2C LCD(0x27, 20, 4);

void verificartemperaturaeumidade(){
float temperatura = dht.readTemperature();
float umidade = dht.readHumidity();
//Serial.println("Temperatura: " + String(temperatura) + "C");
//Serial.println("Umidade: " + String(umidade) + "%"); 
  LCD.clear();
  LCD.setCursor(0, 0);
   LCD.print("Temperatura: " + String(temperatura) + "C"); 

   LCD.setCursor(0, 1);
   LCD.print("Umidade: " + String(umidade) + "%"); 
  delay(700);
}

void abrirporta(){
motor.write(125);
delay(3000);
}

void fecharporta(){
motor.write(0);
delay(3000);
}

void reletrancar(){
digitalWrite(rele, LOW);
Serial.println("trancado");
delay(3000);
}

void reledestrancar(){
digitalWrite(rele, HIGH);
Serial.println("destrancado");
delay(3000);
}

void setup()
{
   pinMode(led, OUTPUT);
   pinMode(PIR, OUTPUT); 
   pinMode(bagulhotagarela, OUTPUT);
   pinMode(MQ135, INPUT); 
   motor.attach(3); 
   motor.write(0); // angulo inicial 
   Serial.begin(9600);
   dht.begin(); //iniciar o dht 
   LCD.init(); //inicia o LDC 
   LCD.backlight(); //liga o led traseiro
   pinMode(rele, OUTPUT);

   digitalWrite(rele, LOW);

   Serial.println("Sensores sendo calibrados, aguente >:(");
   LCD.setCursor(0, 0);
   LCD.print("Iniciando"); 
   delay(5000);
   LCD.setCursor(0, 1);
   LCD.print("Obrigado"); 
   Serial.println("Sensores calibrados!");
   delay(1000);
}

void loop()
{
  //acenderledaodetectarpresenca();
  //verificarvazamentodegas();
  //verificartemperaturaeumidade();
 // abrirporta();
 // fecharporta();
  reletrancar();
  reledestrancar();
}
