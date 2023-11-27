#include <HTTPClient.h>
#include <Wifi.h>
#include "string.h"
#include <Ultrasonic.h>

const char* ssid     = "CARJU";     // login wifi
const char* password = "carju123"; // senha wifi

char *server = "script.google.com";  // Server URL
char *GScriptId = "1FJVn4beCrLZ4AtWsykpAtn8eYAozDuqI4MeZBuhf-8g"; //planilha

WiFiClientSecure client;

long tempoEnvio = 0;

const int sensor1 = 14;  // define o pino do sensor (pino sem pull up interno)
const int sensor2 = 16;  // define o pino do sensor
const int sensor3 = 17;  // define o pino do sensor
const int sensor4 = 18;  // define o pino do sensor
const int sensor9 = 23; // define o pino do sensor

const int pino_trigger = 4
const int pino_echo = 5

Ultrasonic ultrasonic(pino_trigger, pino_echo);

bool estadoSensor1;
bool estadoSensor2;
bool estadoSensor3;
bool estadoSensor4;
bool estadoSensor5;

bool liberaSensor1 = false;
bool liberaSensor2 = false;
bool liberaSensor3 = false;
bool liberaSensor4 = false;
bool liberaSensor5 = false;

bool controleSensor1 = true; //limitador para inicio de processo

bool liberaCalculos = false; //limitador para inicio de calculos

bool acionamentoSensor1 = false; //Limitador do tipo cascata do processo de leitura dos sensores
bool acionamentoSensor2 = false; //Limitador do tipo cascata do processo de leitura dos sensores
bool acionamentoSensor3 = false; //Limitador do tipo cascata do processo de leitura dos sensores
bool acionamentoSensor4 = false; //Limitador do tipo cascata do processo de leitura dos sensores
bool acionamentoSensor5 = false; //Limitador do tipo cascata do processo de leitura dos sensores

bool acionamentoSensor9 = false; //Limitador do tipo cascata do processo de leitura dos sensores

bool acionaEnvioDeDados = false; //Aciona o envio dos dados

bool resetSistema = false;

double inicioPrograma = millis(); //Inicia função de tempo para verificação de logica

double millisTempoSensor1 = millis(); //armazena tempo de leitura do sensor
double millisTempoSensor2 = millis(); //armazena tempo de leitura do sensor
double millisTempoSensor3 = millis(); //armazena tempo de leitura do sensor
double millisTempoSensor4 = millis(); //armazena tempo de leitura do sensor
double millisTempoSensor5 = millis(); //armazena tempo de leitura do sensor
double millisTempoSensor9 = millis(); //armazena tempo de leitura do sensor

double millisIniciaProcesso = millis(); //Inicia função millis para comparador de processo

double tempoSensor9_ms;

bool inicioProcesso = false; //Limitador de processo de leitura dos sensores
bool verificaProcesso = true; //limitador de primeiro ciclo (inicio proceeso)
bool zeraTempoInicioProcesso = true; //limitador para reset de millis de inicio processo

void iniciarprocesso(){

  bool estadoSensor9;  //recebe leitura do pino do sensor
  
  estadoSensor9 = digitalRead(sensor9); // Realiza leitura do pino 9 e inicia metodo ( responsavel por iniciar o processo - temporario, necessario alterar no projeto final)

  if (estadoSensor9 == 0 && verificaProcesso == true) { // se pino 9 enviou sinal e o processo pode ser iniciado

    if (zeraTempoInicioProcesso == true) { // se o tempo inicial do processo pode ser resetado

      millisIniciaProcesso = millis(); // Resetar tempo inicial do processo
      zeraTempoInicioProcesso = false; // Indica que o tempo inicial do processo não pode mais ser resetado

    }

    if ((millis() - millisIniciaProcesso) > 3000) { // se o tempo de inicio do processo for maior que 3 segundos

      Serial.println("Iniciando sistema de leitura.");

      inicioProcesso = true; // processo pode ser iniciado
      verificaProcesso = false; //Processo ja foi iniciado, metodo inicial pode ser desabilitado.

    }
  } else { // se o botão não foi pressionado por 3 segundo

    millisIniciaProcesso = millis(); // resetar tempo de contagem do botão de inicio do processo
  }
}

const int httpsPort = 443;

void enviodosdados(){
  
  HTTPClient http;

  tempoEnvio = millis() - tempoEnvio;
  String url = String("https://script.google.com") + "/macros/s/" + GScriptId + "/exec?" + 
  "value1=" + acionamentoSensor1 +
  "&value2=" + acionamentoSensor2 +
  "&value3=" + acionamentoSensor3 + 
  "&value4=" + acionamentoSensor4 + 
  "&value5=" + acionamentoSensor5 + 
  "&value6=" + tempoSensor1_ms + 
  "&value7=" + tempoSensor2_ms + 
  "&value8=" + tempoSensor3_ms + 
  "&value9=" + tempoSensor4_ms + 
  "&value10=" + tempoSensor5_ms + 
  "&value11=" + velocidadeMediaSensor1 +  
  "&value12=" + velocidadeMediaSensor2 + 
  "&value13=" + velocidadeMediaSensor3 + 
  "&value14=" + velocidadeMediaSensor4 + 
  "&value15=" + velocidadeMediaSensor5 + 
  "&value16=" + aceleracaoMediaSensor1 + 
  "&value17=" + aceleracaoMediaSensor2 + 
  "&value18=" + aceleracaoMediaSensor3 + 
  "&value19=" + aceleracaoMediaSensor4 +
  "&value20=" + aceleracaoMediaSensor5;
  
  tempoEnvio = millis();
  
  Serial.print("Making a request");
  http.begin(url.c_str()); //Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String payload;
  if (httpCode > 0) { //Check for the returning code
    payload = http.getString();

    Serial.println(httpCode);
    Serial.println(payload);
    //     testdrawstyles(payload);
    //if (httpCode == 200 or httpCode == 201) tempPing.Saida(0);
  }
  else {
    Serial.println("Error on HTTP request");
  }
  http.end();
}

double tempoSensor3_ms;
double tempoSensor4_ms;
double tempoSensor5_ms;

double velocidadeMediaSensor1;
double velocidadeMediaSensor2;
double velocidadeMediaSensor3;
double velocidadeMediaSensor4;
double velocidadeMediaSensor5;

double aceleracaoMediaSensor1;
double aceleracaoMediaSensor2;
double aceleracaoMediaSensor3;
double aceleracaoMediaSensor4;
double aceleracaoMediaSensor5;

void calculos(){
  double diferencaTempoPlaca = millisTempoSensor1 - tempoplacaligada;

  millisTempoSensor1 = millisTempoSensor1 - diferencaTempoPlaca;
  millisTempoSensor2 = millisTempoSensor2 - diferencaTempoPlaca;
  millisTempoSensor3 = millisTempoSensor3 - diferencaTempoPlaca;
  millisTempoSensor4 = millisTempoSensor4 - diferencaTempoPlaca;
  millisTempoSensor5 = millisTempoSensor5 - diferencaTempoPlaca;

  Serial.print("tempo no sensor 1 é de:");
  Serial.print(millisTempoSensor1);
  Serial.println(" milisegundos");

  Serial.print("tempo no sensor 2 é de:");
  Serial.print(millisTempoSensor2);
  Serial.println(" milisegundos");

  Serial.print("tempo no sensor 3 é de:");
  Serial.print(millisTempoSensor3);
  Serial.println(" milisegundos");

  Serial.print("tempo no sensor 4 é de:");
  Serial.print(millisTempoSensor4);
  Serial.println(" milisegundos");

  Serial.print("tempo no sensor 5 é de:");
  Serial.print(millisTempoSensor5);
  Serial.println(" milisegundos");
  
  Serial.println("-----------------");

  tempoSensor1_ms = millisTempoSensor1 / 1000;
  tempoSensor2_ms = millisTempoSensor2 / 1000;
  tempoSensor3_ms = millisTempoSensor3 / 1000;
  tempoSensor4_ms = millisTempoSensor4 / 1000;
  tempoSensor5_ms = millisTempoSensor5 / 1000;
      
  Serial.print("tempo no sensor 1 é de:");
  Serial.print(tempoSensor1_ms);
  Serial.println(" segundos");
  
  Serial.print("tempo no sensor 2 é de:");
  Serial.print(tempoSensor2_ms);
  Serial.println(" segundos");

  Serial.print("tempo no sensor 3 é de:");
  Serial.print(tempoSensor3_ms);
  Serial.println(" segundos");

  Serial.print("tempo no sensor 4 é de:");
  Serial.print(tempoSensor4_ms);
  Serial.println(" segundos");

  Serial.print("tempo no sensor 5 é de:");
  Serial.print(tempoSensor5_ms);
  Serial.println(" segundos");

  Serial.println("-----------------");
  
  velocidadeMediaSensor1 = 0;
  Serial.print("velocidade no sensor 1 é de:");
  Serial.print(velocidadeMediaSensor1);
  Serial.println(" metros por segundos");

  velocidadeMediaSensor2 = (distanciaSensor2 - distanciaSensor1) / (tempoSensor2_ms - tempoSensor1_ms);
  Serial.print("velocidade no sensor 2 é de:");
  Serial.print(velocidadeMediaSensor2);
  Serial.println(" metros por segundos");

  velocidadeMediaSensor3 = (distanciaSensor3 - distanciaSensor2) / (tempoSensor3_ms - tempoSensor2_ms);
  Serial.print("velocidade no sensor 3 é de:");
  Serial.print(velocidadeMediaSensor3);
  Serial.println(" metros por segundos");

  velocidadeMediaSensor4 = (distanciaSensor4 - distanciaSensor3) / (tempoSensor4_ms - tempoSensor3_ms);
  Serial.print("velocidade no sensor 4 é de:");
  Serial.print(velocidadeMediaSensor4);
  Serial.println(" metros por segundos");

  velocidadeMediaSensor5 = (distanciaSensor5 - distanciaSensor4) / (tempoSensor5_ms - tempoSensor4_ms);
  Serial.print("velocidade no sensor 5 é de:");
  Serial.print(velocidadeMediaSensor5);
  Serial.println(" metros por segundos");
  
  Serial.println("-----------------");
    
  aceleracaoMediaSensor1 = 0;
  Serial.print("Aceleração no sensor 1 é de:");
  Serial.print(aceleracaoMediaSensor1);
  Serial.println(" metros por segundos²");

  aceleracaoMediaSensor2 = (velocidadeMediaSensor2 - velocidadeMediaSensor1) / (tempoSensor2_ms - tempoSensor1_ms);
  Serial.print("Aceleração no sensor 2 é de:");
  Serial.print(aceleracaoMediaSensor2);
  Serial.println(" metros por segundos²");

  aceleracaoMediaSensor3 = (velocidadeMediaSensor3 - velocidadeMediaSensor2) / (tempoSensor3_ms - tempoSensor2_ms);
  Serial.print("Aceleração no sensor 3 é de:");
  Serial.print(aceleracaoMediaSensor3);
  Serial.println(" metros por segundos²");
  
  aceleracaoMediaSensor4 = (velocidadeMediaSensor4 - velocidadeMediaSensor3) / (tempoSensor4_ms - tempoSensor3_ms);
  Serial.print("Aceleração no sensor 4 é de:");
  Serial.print(aceleracaoMediaSensor4);
  Serial.println(" metros por segundos²");

  aceleracaoMediaSensor5 = (velocidadeMediaSensor5 - velocidadeMediaSensor4) / (tempoSensor5_ms - tempoSensor4_ms);
  Serial.print("Aceleração no sensor 5 é de:");
  Serial.print(aceleracaoMediaSensor5);
  Serial.println(" metros por segundos²");
}

void setup() {
  Serial.begin(115200); // Inicia comunicação serial
  inicioPrograma = millis(); // armazena tempo de placa ao iniciar o processo

  Serial.print("Conectando a rede Wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  
  pinMode(sensor1, INPUT_PULLUP); //Define os pinos de sensores como entradas de sinal digital
  pinMode(sensor2, INPUT_PULLUP); //Define os pinos de sensores como entradas de sinal digital
  pinMode(sensor3, INPUT_PULLUP); //Define os pinos de sensores como entradas de sinal digital
  pinMode(sensor4, INPUT_PULLUP); //Define os pinos de sensores como entradas de sinal digital
  pinMode(sensor9, INPUT_PULLUP); //Define os pinos de sensores como entradas de sinal digital
  pinMode(led, OUTPUT); //Degine pino de indicador visual como saida
  pinMode(pinoSinaleiroFim, OUTPUT); //Degine pino de indicador visual como saida

  digitalWrite(led, LOW);
  digitalWrite(pinoSinaleiroFim, LOW);
  
  delay(2000);//Espera um tempo para se conectar no WiFi

  liberaSensor1 = false;  //Limitador de processo de leitura dos sensores
  liberaSensor2 = false;  //Limitador de processo de leitura dos sensores
  liberaSensor3 = false;  //Limitador de processo de leitura dos sensores
  liberaSensor4 = false;  //Limitador de processo de leitura dos sensores
  liberaSensor5 = false;  //Limitador de processo de leitura dos sensores

  acionamentoSensor1 = false; //Limitador do tipo cascata do processo de leitura dos sensores
  acionamentoSensor2 = false; //Limitador do tipo cascata do processo de leitura dos sensores
  acionamentoSensor3 = false; //Limitador do tipo cascata do processo de leitura dos sensores
  acionamentoSensor4 = false; //Limitador do tipo cascata do processo de leitura dos sensores
  acionamentoSensor5 = false; //Limitador do tipo cascata do processo de leitura dos sensores
      
  inicioProcesso = false;
  verificaProcesso = true;
  zeraTempoInicioProcesso = true;
  controleSensor1 = true;

  Serial.println("Parametrização do sistema concluida");
  Serial.println(" ");
}

void loop() {

iniciarprocesso();

  if (inicioProcesso == true) { // se o processo esta pronto para iniciar
    
    digitalWrite(led, HIGH);  // indicador visual de inicio de processo
    
    if (controleSensor1 == true) { // se o sensor 1 esta liberado para atuar
      Serial.println("Sistema de leitura iniciado com sucesso!");
      liberaSensor1 = true; //liberar processo do sensor 1
      controleSensor1 = false; // se o sensor 1 esta liberado para atuar, desabilitar metodo de controle do sensor 1
    }

    if (liberaSensor1 == true) { // se o processo do sensor esta liberado, iniciar o processo

      estadoSensor1 = digitalRead(sensor1); // realizar leitura do sensor

      if (estadoSensor1 == 0) { // se o sensor envio sinal

        acionamentoSensor1 = true;  // variavel que será enviada para nuvem, sensor foi acionado
        millisTempoSensor1 = millis();  // armazenar tempo em que o sensor foi acionado
        liberaSensor2 = true; //liberar processo do proximo sensor

        Serial.println("Sensor 1");
        Serial.println(millisTempoSensor1);
        Serial.println("-----------------");

        liberaSensor1 = false; // encerrar processo do sensor

      }
    }

    if (liberaSensor2 == true) { // se o processo do sensor esta liberado, iniciar o processo

      estadoSensor2 = digitalRead(sensor2); // realizar leitura do sensor

      if (estadoSensor2 == 1) { // se o sensor envio sinal

        acionamentoSensor2 = true;  // variavel que será enviada para nuvem, sensor foi acionado
        millisTempoSensor2 = millis();  // armazenar tempo em que o sensor foi acionado
        liberaSensor3 = true; //liberar processo do proximo sensor

        Serial.println("Sensor 2");
        Serial.println(millisTempoSensor2);
        Serial.println("-----------------");

        liberaSensor2 = false;  // encerrar processo do sensor

      }
    }

    if (liberaSensor3 == true) { // se o processo do sensor esta liberado, iniciar o processo

      estadoSensor3 = digitalRead(sensor3); // realizar leitura do sensor

      if (estadoSensor3 == 0) { // se o sensor envio sinal

        acionamentoSensor3 = true;  // variavel que será enviada para nuvem, sensor foi acionado
        millisTempoSensor3 = millis();  // armazenar tempo em que o sensor foi acionado
        liberaSensor4 = true; //liberar processo do proximo sensor

        Serial.println("Sensor 3");
        Serial.println(millisTempoSensor3);
        Serial.println("-----------------");

        liberaSensor3 = false;  // encerrar processo do sensor

      }
    }

    if (liberaSensor4 == true) {

      estadoSensor4 = digitalRead(sensor4);

      if (estadoSensor4 == 0) {

        acionamentoSensor4 = true;
        millisTempoSensor4 = millis();

        Serial.println("Sensor 4");
        Serial.println(millisTempoSensor4);
        Serial.println("-----------------");

        liberaSensor4 = false;

      }
    }

       if (liberaSensor5 == true) {
        // Envia pulso para o disparar o sensor
        float cmMsec, inMsec;
        long microsec = ultrasonic.timing();
        cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
        Serial.println(cmMsec);

        if(cmMsec <= 17){
          estadoSensor5 = 0;
         }
         if (estadoSensor5 == 0) { 
          
          acionamentoSensor5 = true;
          millisTempoSensor5 = millis();
          liberaCalculos = true;
  
          Serial.println("Sensor 5");
          Serial.println(millisTempoSensor5);
          Serial.println("-----------------");
          digitalWrite(pinoSinaleiroFim, HIGH);  // indicador visual de CHEGADA
          liberaSensor5 = false;

      }
    }
     
    if (liberaCalculos == true) {

      calculos();
      
      liberaCalculos = false;
      acionaEnvioDeDados = true;
      inicioProcesso = false;
    }
  }
  if(acionaEnvioDeDados == true){

  enviodosdados();
  
  resetSistema = true;
  acionaEnvioDeDados = false;
    }
   
    if(resetSistema == true){

      liberaSensor1 = false;  //Limitador de processo de leitura dos sensores
      liberaSensor2 = false;  //Limitador de processo de leitura dos sensores
      liberaSensor3 = false;  //Limitador de processo de leitura dos sensores
      liberaSensor4 = false;  //Limitador de processo de leitura dos sensores
      liberaSensor5 = false;  //Limitador de processo de leitura dos sensores
      
      acionamentoSensor1 = false; //Limitador do tipo cascata do processo de leitura dos sensores
      acionamentoSensor2 = false; //Limitador do tipo cascata do processo de leitura dos sensores
      acionamentoSensor3 = false; //Limitador do tipo cascata do processo de leitura dos sensores
      acionamentoSensor4 = false; //Limitador do tipo cascata do processo de leitura dos sensores
      acionamentoSensor5 = false; //Limitador do tipo cascata do processo de leitura dos sensores

      inicioProcesso = false;
      verificaProcesso = true;
      zeraTempoInicioProcesso = true;
      controleSensor1 = true;

      digitalWrite(pinoSinaleiroFim, LOW);  // indicador visual de CHEGADA
      digitalWrite(led, LOW);  // indicador visual de inicio de processo
      Serial.println("Desligando sistema de leitura leitura.");
      
      resetSistema = false;
    } 
}
