#define BLYNK_TEMPLATE_ID           "TMPLqAUp7TUb"
#define BLYNK_DEVICE_NAME           "smart iot drop off and pick up parcel"
#define BLYNK_AUTH_TOKEN            "a6Cko-lYewNANpNpuROIWyiu3waeuDnN"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int state = 0;

int buzz = 2;
int button = 14;
int offbuzz = 13;
int irsensor = 34;
int vibsensor = 35;

int valuebutton = 0;
int valueirsensor = 0;
int valuevibsensor = 0;
int valueoffbuzz = 0;

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "HUAWEI P20";
char pass[] = "nafza9494";

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

void myTimerEvent()
{
  valueirsensor = digitalRead(irsensor);
  valuevibsensor = digitalRead(vibsensor);
  valueoffbuzz = digitalRead(offbuzz);

  if ( valuevibsensor == HIGH ){ //sensor npn
    digitalWrite(buzz, HIGH);
  }

  if (valueoffbuzz == HIGH){
    digitalWrite(buzz, LOW);
    }
  
  Blynk.virtualWrite(V0, valueirsensor);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(button, INPUT);
  pinMode(vibsensor, INPUT);
  pinMode (buzz, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  valuebutton = digitalRead(button); // 0 1 , 0 1023, 0 255 = LOW HIGH
  if (valuebutton == HIGH){
    notification();
  }
}

void notification()
{
  if (state == 0){
  Blynk.logEvent("notification", "barang sampai sila buka lock");
  state = 1;
  }
}
