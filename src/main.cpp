#include "POL_WiFiConnect.h"
#include "POL_Config.h"

POL_WiFiConnect myWifi;
float last = 0;
bool failed = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Started");

  myWifi.connect();
  // myMqtt.setup(mqtt_callback);

  Serial.println(myWifi.getIp());
}

void sendMessage(float value2)
{
  WiFiClient client;
  failed = false;

  const int httpPort = 80;
  // Caso a conexao nao seja estabelecida entre ESP82666 e IFTTT.
  if (!client.connect(ifttHost, httpPort))
  {
    Serial.println("Falha de Conexao");
    Serial.println("-");
    failed = true;
    return;
  }

  //Dispara eventName através da chave de comunicacao do IFTTT.
  String url = "/trigger/";
  url += ifttEventName;
  url += "/with/key/";
  url += ifttKey;
  url += "?value1=" + String(ifttPlace);
  url += "&value2=" + String(value2);

  Serial.print("URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + ifttHost + "\r\n" +
               "Conexao: Fechada\r\n\r\n");

  int timeout = millis() + 5000;

  while (client.available() == 0)
  {
    if (timeout - millis() < 0)
    {
      Serial.println(">>> Tempo Excedido !");
      client.stop();
      failed = true;
      return;
    }
  }
  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("Fechando Conexao");
  Serial.println();
  Serial.println();

  delay(1000);
}

void checkTemperature(int valorObtido)
{
  float celsius = (valorObtido * 330.0f) / 1023.0f;
  float current = celsius;

  Serial.print("Current: ");
  Serial.print(floorf(celsius));
  Serial.println("°C");

  Serial.print("Value: ");
  Serial.println(valorObtido);

  if (floorf(last) != floorf(current) || failed)
  {
    sendMessage(current);

    Serial.print("A temperatura mudou de ");
    Serial.print(last);
    Serial.print(" para ");
    Serial.println(current);

    Serial.print("Falhou: ");
    Serial.println(failed ? "yes" : "no");
  }

  last = current;
}

void loop()
{
  int valorObtido = analogRead(A0);

  checkTemperature(valorObtido);
  delay(30 * 1000);
}