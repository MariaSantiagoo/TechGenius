# include<Servo.h>
Servo motor;
const int buzzer = 4;
const int ledVermelho = 4;
const int ledVerde = 2;
const int senhaCorreta = 111;
int senhaDigitada;

void setup()
{
  Serial.begin(9600);
  motor.attach(12);
  pinMode(ledVermelho, OUTPUT); 
  pinMode(ledVerde, OUTPUT);
  pinMode(buzzer, OUTPUT); //configura o buzzer da porta 4 como saida 
  motor.write(0);

}


void loop()
{
  //

  Serial.println("BEM-VINDO digite a senha!");
  while(!Serial.available());
  senhaDigitada = Serial.parseInt();

  if(senhaCorreta == senhaDigitada) {
    digitalWrite(ledVerde, HIGH);
    delay(700);
    digitalWrite(ledVerde, LOW);
    motor.write(90);
  }else {//errou
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledVermelho, HIGH);
    delay(700);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, LOW);


  }
  delay(3000);
  //fim do loop
}
