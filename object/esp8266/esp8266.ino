// Control ESP8266 anywhere

// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "h6n3uu";

// WiFi parameters
const char* ssid = "test_wifi";
const char* password = "123esp8266";

// Functions
void callback(char* topic, byte* payload, unsigned int length);

void setup(void)
{
  // Start Serial
  Serial.begin(74880);
  Serial.println("");
  Serial.println("serial begin - ");

  // Set callback
  client.setCallback(callback);
  Serial.println("set callback - ");
  
  // Give name and ID to device
  rest.set_id(device_id);
  rest.set_name("relay_anywhere");
  Serial.println("set name and id - ");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("connecting... ");
  }
  Serial.println("wifi connected ! ");

  // Set output topic
  char* out_topic = rest.get_topic();

}

void loop() {

  // Connect to the cloud
  rest.loop(client);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.println("message received ! ");
  rest.handle_callback(client, topic, payload, length);

}
