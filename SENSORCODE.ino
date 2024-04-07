#include <WiFi.h>
#include "PubSubClient.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "3.96.64.144";
const int mqtt_port = 1883;
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_heart_topic = "heartsensor";
const char* mqtt_bp_topic = "bp";


WiFiClient espClient;
PubSubClient client(espClient);

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int analogValue1 = analogRead(36);
  int analogValue2 = analogRead(39);
  int analogValue3 = analogRead(35); // Pin for the third potentiometer

  int heartRate = map(analogValue1, 0, 4095, 60, 180);
  float heartRateFloat = floatMap(analogValue1, 0, 4095, 60, 180);

  int bp = map(analogValue2, 0, 4095, 0, 180);
  float f_bp = floatMap(analogValue2, 0, 4095, 0, 180);

  

  Serial.print("Heart Rate: ");
  Serial.println(heartRate);
  Serial.print("blood pressure: ");
  Serial.println(bp);
 

  String heartMessage = String(heartRate);
  client.publish(mqtt_heart_topic, heartMessage.c_str());

  String oxygenMessage = String(bp);
  client.publish(mqtt_bp_topic, oxygenMessage.c_str());


  delay(10000);
}
