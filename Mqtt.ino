#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Ricky";
const char* password = "1092387456";
const char* mqtt_server = "192.168.0.107";
const int buttonPin = 13;     
const int buttonPin2 = 14;     

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
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

  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW); 
  } 
  else {
    digitalWrite(BUILTIN_LED, HIGH);  
  }
}

void reconnect() {
	
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("jardin", "Enviando mensaje");
       client.subscribe("jardin");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
	
  pinMode(BUILTIN_LED, OUTPUT);  
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);     
  pinMode(buttonPin2, INPUT_PULLUP);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
 long now = millis();
    ++value;
delay(2000);
    
  if (digitalRead(buttonPin) == HIGH) {    
           snprintf (msg,50, "Derecha #%ld", value);
    }
  else if (digitalRead(buttonPin2) == HIGH){
           snprintf (msg, 50, "Izquierda #%ld", value);
          }
 else{
        snprintf (msg, 50, "Centro #%ld", value);
  }
Serial.print("Publish message:Hello ");
    Serial.println(msg);
    client.publish("jardin", msg);
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
}  
