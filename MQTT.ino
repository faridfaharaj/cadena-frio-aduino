// Este código funciona con la siguiente configuración:
//    Board support API esp8266 (by ESP8266 Community) version 2.5.2

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

#define SENSOR_A0 A0
#define LED1                                                                   \
  D0 // R-Rojo  ... IMPORTANTE: Validar si el LED es de ánodo o de cátodo común
#define LED2 D1 // G-Verde
#define LED3 D2 // B-Azul

WiFiClient espClient;
PubSubClient client(espClient);

long tiempoAnterior = 0;
char msg[50];
int lecturaSensorA0 = 0;

void setup_wifi() {
  delay(100);
  Serial.println();
  Serial.print("macAddress: ");
  Serial.println(WiFi.macAddress()); // mac:="Medium Access Control Address"
  // Iniciar por conectar con la red WiFi
  Serial.print("Conectando WiFi --> ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
} // End setup_wifi()

void callback(char *topic, byte *payload, unsigned int length) {
  char *cstring = (char *)payload;
  cstring[length] = '\0'; // Adds a terminate to end of string based on length
                          // of current payload
  Serial.println(cstring);
  switch (cstring[1]) {
  case 'R':
    Serial.println("\tRojo");
    digitalWrite(LED1,
                 LOW); // El LED-Rojo se enciende con '0' por ser ánodo común
    digitalWrite(LED2,
                 HIGH); // El LED-Verde se apaga con '1' por ser ánodo común
    digitalWrite(LED3,
                 HIGH); // El LED-Azul se apaga con '1' por ser ánodo común
    break;
  case 'G':
    Serial.println("\tVerde");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    break;
  case 'B':
    Serial.println("\tAzul");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    break;
  case 'X':
    Serial.println("\tAPAGAR todo!");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    break;
  default: // Sin no fué ninguna de las anteriores entonces hacemos nada
    break;
  }
} // End callback(...)

void reconnect() {
  // Ciclarse hasta lograr reconexión con "broker"
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT ...");
    // Create a random client ID
    String clientId = "Client-";
    clientId += String(random(0xffff), HEX);
    // Intentar reconexión ...
    //  ... en caso de que el "broker" tenga clientID, username y password
    //  ... cambiar la siguiente línea por --> if
    //  (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado al 'broker' MQTT!!!");
      // Ya conectado al "borker" MQTT suscribirse al tópico
      client.subscribe(channelTopicSub.c_str());
    } else {
      Serial.print("Error de conexión, rc=");
      Serial.print(client.state());
      Serial.println(" ... reintentando en 6 seg.");
      // Esperar 6 segundos para el próximo intento de conexión
      delay(6000);
    }
  }
} // end reconnect()

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);

  if (!loadConfig()) {
    Serial.println("Config failed, halting");
    while (true)
      delay(1000);
  }

  setup_wifi();
  client.setServer(mqtt_server.c_str(), mqtt_port);
  client.setCallback(callback);
}

void loop() {
  int sensogA0;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  lecturaSensorA0 = analogRead(A0);
  Serial.print("sensorA0=");
  Serial.println(lecturaSensorA0);

  String msg = "";
  msg = msg + lecturaSensorA0;
  char message[58];
  msg.toCharArray(message, 58);
  Serial.print("menssage=");
  Serial.println(message);
  client.publish(channelTopicPub.c_str(), message);

  delay(1000); // Esperar 1000 milisegundos
} // End loop
