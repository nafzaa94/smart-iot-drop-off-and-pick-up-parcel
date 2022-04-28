#include <WiFi.h>
#include <PubSubClient.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "HP";
const char* password = "nafza9494";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.43.228";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// LED Pin
const int lock = 26;
const int button = 14;
const int buzz = 2;
const int irsensor = 34;
const int vibsensor = 35;

int valuebutton = 0;
int valueirsensor = 0;
int valuevibsensor = 0;
int state = 0;
int state2 = 0;

void setup() {
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(lock, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(button, INPUT);
}

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

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/lockdoor") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(lock, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(lock, LOW);
      state2 = 1;
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/lockdoor");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  valuebutton = digitalRead(button);
  valueirsensor = digitalRead(irsensor);
  valuevibsensor = digitalRead(vibsensor);

  if(valuebutton == HIGH && state == 0){
    client.publish("esp32/notification", "notification");
    state = 1;
    }
  else {
    state = 0;
    }

 if (state2 == 1){
  if(valueirsensor == LOW){
    client.publish("esp32/irsensor", "barang ada");
    state2 = 0;
    }
  if (valueirsensor == HIGH){
    client.publish("esp32/irsensor", "barang tidak ada");
    state2 = 0;
    }
 }

  if (valuevibsensor == LOW){
    digitalWrite(buzz, HIGH);
    }
  else{
    digitalWrite(buzz, LOW);
    }

}
