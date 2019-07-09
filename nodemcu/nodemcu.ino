#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"

// SSID e Password do Wi-Fi a qual o NodeMCU deve se comunicar
const char ssid[] = "bssbrubs";
const char psw[] = "andebubu2401";

// Pagina web que receberá as informações via POST
const char http_site[] = "http://localhost/nodemcu";
const int http_port = 8080;
WiFiClient client;
IPAddress server(192, 168, 0, 23); //Endereço IP do servidor - http_site

// Constantes de definição
#define pinDHT 8
#define typeDHT DHT22
#define pinLED D1;
#define pinLDR A0;

// Variáveis globais
int lumin = 0;
int temp = 0;
int humid = 0;

// DHT
DHT dht(pinDHT, typeDHT);

void setup() {
  delay(3000);
  Serial.begin(9600);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, psw);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(100);
    Serial.print("Tentando conectar ao Wi-fi");
  }

  Serial.print("\nWI-FI conectado com sucesso: ");
  Serial.println(ssid);

  dht.begin();

}

void loop() {

  // Lendo umidade e convertendo valor pra int
  float h = dht.readHumidity();
  humid = (int) h;
  // Lendo temperatura em Celsius  e convertendo valor pra int
  float t = dht.readTemperature();
  temp = (int) t;

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Falha ao ler sensor DHT!");
    return;
  }

  // Calculando indice de calor
  float hic = dht.computeHeatIndex(t, h, false);

  // Lendo luminosidade
  lumin = analogRead(pinLDR);



  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Luminosidade: ");
  Serial.print(lumin);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C\t ");
  Serial.print("Indice de Calor: ");
  Serial.print(hic);
  Serial.print(" *C ");


  // Envio dos dados do sensor para o servidor via GET
  if ( !getPage((int)temp, (int)humid, (int)lumin) ) {
    Serial.println("GET request failed");
  }
  delay(5000);
}

// Executa o HTTP GET request na pagina web
bool getPage(int temp, int humid, int lumin) {

  if ( !client.connect(server, http_port) ) {
    Serial.println("Falha na conexao com o site ");
    return false;
  }

  String param = "?temp=" + String(temp) + "&humid=" + String(humid) + "&lumin=" + String(lumin); //Parâmetros com as leituras
  Serial.println(param);
  client.println("GET /nodemcu/fuzzy_in_out.php" + param + " HTTP/1.1");
  client.println("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();
  client.println();

  delay(5000);

  // Informações de retorno do servidor para debug
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print("quantidade de agua: ");
    Serial.print(line);
  }
  return true;
}
