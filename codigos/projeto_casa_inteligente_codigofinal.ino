#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
//#include <Servo.h>

// --- WiFi & MQTT ---
const char* ssid = "???";         //sua rede wifi
const char* password = "???";     //senha da sua rede wifi
const char* mqtt_server = "???";  //endereço do broker público
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
    if (client.connect("ESP32ClientTest")) {
      Serial.println("Conectado!");
      client.subscribe(topic_led);    //conecta ao topico do led assim que estabelecer ligação com o broker
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

  //led - luz da sala
  if (strcmp(topic, topic_led) == 0) {  //tópico atual é o do led?
    if (mensagem == "ligar") {
      digitalWrite(led, HIGH);
    } else if (mensagem == "desligar") {
      digitalWrite(led, LOW);
    }
  }

  /*
    Verifica se o tópico recebido é o topico da porta
  é uma função da linguagem C que compara duas strings (topic e topic_porta)
  */
  //porta
  if (strcmp(topic, topic_porta) == 0) {  //tópico atual é o da porta?
    if (mensagem == "abrir") {
      abrirporta();
      delay(500);
      reledestrancar();
    } else if (mensagem == "fechar") {
      reletrancar();
      delay(500);
      fecharporta();
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


  //conectarWiFi();                            //conecta no wifi
  //client.setServer(mqtt_server, mqtt_port);  //conecta no broker server
  //client.setCallback(tratarMensagem);        //trata as mensagens recebidas do broker

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
  // if (!client.connected()) reconectarMQTT();  //se não tem conexão com o broker, tenta reconectar
  // client.loop();                              //mantém a conexão com o broker serve sempre aberta

  acenderledaodetectarpresenca();
  //reledestrancar();
 // delay(1000);
 // abrirporta();
  //delay(1500);
 //fecharporta();
  //delay(1500);
  //reletrancar();
  //delay(1000);
}
