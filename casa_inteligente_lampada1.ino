// C++ code
//
const int ledBranco = 13;
const int PIR = 2; 

void setup()
{
  Serial.begin(9600);
  pinMode(ledBranco, OUTPUT);
  pinMode(PIR,INPUT);

  Serial.println("Calibrando o sensor PIR");
  Serial.println("Por favor, não se mova na frente do sensor.");
  delay(3000);
  Serial.println("Sensor PIR calibrado e pronto!"); 
}

void loop()
{
  int estadoPIR = digitalRead(PIR);

  if(estadoPIR == HIGH){
    digitalWrite(ledBranco, HIGH);

    Serial.println("Movimento detectado! acender led");
    delay(5000); 
  }else {
    digitalWrite(ledBranco, LOW);
    Serial.println("Sem movimento. LED apagado.");
  }

}