/*
  ESP8266/Relay
  parisot.simon (-) gmail.com
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configuration
const char* ssid = "test_wifi";                         // wifi configuration
const char* password = "123esp8266";            
const char* mqtt_server = "m23.cloudmqtt.com";          // MQTT broker configuration
int mqtt_port = 15067;
const char* mqtt_user = "sqqvsrhi";
const char* mqtt_pwd = "6mvr3arW3WJ0";
const char* outTopic = "state";             
const char* inTopic = "command";
int relayPin = 5;                                       // pin that commands the relay

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Toggle the relay if an 1 was received as first character
  if ((char)payload[0] == '1') {
    Serial.println("Toggle the relay");
    digitalWrite(relayPin, !digitalRead(relayPin));
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(digitalRead(relayPin));
    if(digitalRead(relayPin)){
      client.publish(outTopic, "Relay HIGH");
    } else {
      client.publish(outTopic, "Relay LOW");
    }
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pwd)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "Hi, i'm connected");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(relayPin, OUTPUT);     // Initialize the pin which commands the relay
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
