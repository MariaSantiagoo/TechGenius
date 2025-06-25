#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
//#include <Servo.h>

// --- WiFi & MQTT ---
const char* ssid = "ProjetosIoT_Esp32";         //sua rede wifi
const char* password = "Sen@i134";     //senha da sua rede wifi
const char* mqtt_server = "172.16.39.118";  //endereço do broker público
const int mqtt_port = 1883;       //porta do broker público, geralmente 1883

//Tópicos
const char* topic_led = "Techgenius/sala/luz";
const char* topic_porta = "techgenius/sala/porta";

//----pinos---
const int rele = 25;
const int PIR = 4;
const int led = 2;

int contador = 0;  // avança para dentro de casa

Servo motor;  //variavel servo
const int servoMotor = 18;


// --- Objetos ---
WiFiClient espClient;
PubSubClient client(espClient);

// --- Funções WiFi e MQTT ---
void conectarWiFi() {  //verifica conexão wifi para somente depois iniciar o sistema
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconectarMQTT() {  //verifica e reconecta a conexão com o broker mqtt
  while (!client.connected()) {
    Serial.print("Reconectando MQTT...");
    String clientId = "ESP32Client-";
clientId += String(WiFi.macAddress());  
  if (client.connect( clientId.c_str() )) { //converte a string C++ em char modelo C
 Serial.println("Conectado!");
      
      client.subscribe(topic_porta);  //conecta ao topico da porta assim que estabelecer ligação com o broker
    } else {
      Serial.print("Falha: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

/**
  Função para tratamento das mensagens de callback/retorno do broker de cada tópico subscrito (led, porta, etc.)

  char* é uma cadeia de caracteres em C como um vetor onde cada caractter/letra está em uma posição, 
  diferente de uma String em C++ que pode ser lida completamente
*/
void tratarMensagem(char* topic, byte* payload, unsigned int length) {  //
  String mensagem = "";
  for (int i = 0; i < length; i++) {  //concatena todas os char* para se ter o texto completo em String
    mensagem += (char)payload[i];
  }

  Serial.printf("Mensagem recebida [%s]: %s\n", topic, mensagem.c_str());

  /*
    Verifica se o tópico recebido é o topico da porta
  é uma função da linguagem C que compara duas strings (topic e topic_porta)
  */
  //porta
  if (strcmp(topic, topic_porta) == 0) {  //tópico atual é o da porta?
    if (mensagem == "abrir") {
      reledestrancar();
      delay(500);
      abrirporta();
    } else if (mensagem == "fechar") {
      fecharporta();
      delay(500);
      reletrancar();
    }
  }
}

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

void fecharporta() {
  motor.write(0);
  Serial.println("Porta fechada");
}

void abrirporta() {
  motor.write(90);
  Serial.println("Porta aberta");
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

void setup() {
  Serial.begin(115200);


  pinMode(led, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(rele, OUTPUT);
  digitalWrite(rele, HIGH);

  motor.attach(servoMotor);

  digitalWrite(rele, LOW);

  motor.write(0);


  Serial.println("Carregando");
  delay(7000);
  Serial.println("Carregado com sucesso");


  conectarWiFi();                            //conecta no wifi
  client.setServer(mqtt_server, mqtt_port);  //conecta no broker server
  client.setCallback(tratarMensagem);        //trata as mensagens recebidas do broker

  Serial.println("Sistema iniciado!");

  delay(2000);

  //reledestrancar();
  //abrirporta();
  //delay(4000);
  //fecharporta();
  //delay(2000);
  //reletrancar();
}

void loop() {
  if (!client.connected()) reconectarMQTT();  //se não tem conexão com o broker, tenta reconectar
  client.loop();                              //mantém a conexão com o broker serve sempre aberta

  acenderledaodetectarpresenca();
//  reledestrancar();
// delay(1000);
//  abrirporta();
//  delay(1500);
//  fecharporta();
//  delay(1500);
//   reletrancar();
  delay(500);
}
