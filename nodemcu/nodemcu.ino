#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

// SSID e Password do Wi-Fi a qual o NodeMCU deve se comunicar
const char ssid[] = "bssbrubs";
const char psw[] = "andebubu2401";

HTTPClient http;

// Pagina web que receberá as informações via POST
const char http_site[] = "http://192.168.43.8";
const int http_port = 80;
WiFiClient client;
IPAddress server(192, 168, 43, 8); //Endereço IP do servidor - http_site

// Constantes de definição
#define pinDHT D2
#define typeDHT DHT22
#define pinLED D1
#define pinLDR A0

// Variáveis globais
int lumin = 0;
int temp = 0;
int humid = 0;

int valorSaida = 0;

// DHT
DHT dht(pinDHT, typeDHT);

void setup() {
  delay(3000);
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);

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
  Serial.println(" ");

  if ( !client.connect(server, http_port) ) {
    Serial.println("Falha na conexao com o site ");

  } else {
    Serial.println("Conectado ao site ");

    String param = "?temp=" + String(temp) + "&humid=" + String(humid) + "&lumin=" + String(lumin); //Parâmetros com as leituras
    Serial.println(param);
    client.println("GET http://192.168.43.8:80/Sistema-NodeMCU-Logica-Fuzzy/fuzzy_in_out.php" + param + "HTTP/1.1");
    client.println("Host: ");
    client.println(http_site);
    client.println("Connection: close");
    client.println();
    client.println();

    // Informações de retorno do servidor para debug

    http.begin("http://192.168.43.8:80/Sistema-NodeMCU-Logica-Fuzzy/fuzzy_in_out.php");
    int httpCode = http.GET();                                                                  //Send the request

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
      valorSaida = atoi( payload.c_str() );
    }
    http.end();   //Close connection

    if (valorSaida > 36) {
      digitalWrite(pinLED, HIGH);
    } else {
      digitalWrite(pinLED, LOW);
    }
  }

  delay(5000);
}
