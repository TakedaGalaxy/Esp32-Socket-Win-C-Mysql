#include <WiFi.h>

//Definição do botão usado para enviar dados
#define BOTAO_PINO  15
#define BOTAO       !digitalRead(BOTAO_PINO)

//Definindo pino analogico para captura de dados
#define CAPTURA_POT analogRead(34)

//Definições da senha e identificador da rede Wifi
#define SSID        ""
#define PASSWD      ""

//Armazena o ip e porta da interface socket para bando de dados
const char * socketIp = "";
const uint16_t socketPorta = 0; 

//Objeto usado para realizar a conexão ao socket
WiFiClient client;

void setup() {

  //Inicializando Serial com baud rate de 115200
  Serial.begin(115200);

  //Iniciando a conexão da rede wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf("\n %d", WiFi.status());
    delay(100);
  }

  //Imprime o ip atribuido ao esp32 na rede
  Serial.print("\nIP: ");
  Serial.println(WiFi.localIP());

  //Configurando periferico
  pinMode(BOTAO_PINO, INPUT_PULLUP);
  
}

void loop() {

  //Realiza a conexão ao socket caso esteja desconectado
  if (!client.connected()) {

    Serial.printf("\nDesconectado");

    Serial.printf("\nReconectando");

    while (!client.connected()) {

      //Realiza a conexão ao socket
      if (!client.connect(socketIp,socketPorta)) {

        Serial.printf("\nFalha de conexao : %s:%d",socketIp,socketPorta);

        delay(1000);

      }

    }

    Serial.printf("\nConectado");

  }

  //Verifica se algum dado está pronto para ser lido na Serial
  if (Serial.available()) {

    while (Serial.available()) {

      char c = Serial.read();

      //Repassa o dado recebido da serial para o socket
      client.print(c);

      delay(10);

    }

  }

  //Verifica se algum dado do socket está pronto para ser lido
  if (client.available()) {

    Serial.printf("\nDados Recebidos\n");
    
    while (client.available()) {

      char c = client.read();

      //Repassa para serial o dado recebido pelo socket
      Serial.print(c);

      delay(10);

    }

    Serial.print('\n');
    
  }

  //Enviando dados do potenciometro para socket
  if(BOTAO){

    while(BOTAO);

    client.printf("ID:TESTE,VALOR:%d,TIPO:ANALOGICO",CAPTURA_POT);

    delay(200);
    
  }

}
