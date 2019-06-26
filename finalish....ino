
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include "IFTTTWebhook.h"

#define WIFI_SSID "dnmgiot2016"
#define  WIFI_PASSWORD "dnmgiot2016"

#define IFTTT_API_KEY "m0Q6egYCC3SaYtdM7Uo7M6VTV94laB-Ed9AnK0QsnV6"
#define IFTTT_EVENT_NAME "lpgleaking"
char auth[] = "af0ff591e008477aadb2528b6d63bdf0";
char ssid[] = "dnmgiot2016";
char pass[] = "dnmgiot2016";

int threshold = 700;
int switchState = digitalRead(4);


int numberOfValues = 4;
int tindex = 0;
IFTTTWebhook wh(IFTTT_API_KEY, IFTTT_EVENT_NAME);

boolean messageSent = false;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(5, OUTPUT);
  Serial.println("RUNNING");
  pinMode(4, INPUT);



  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }





}

int currentSensorValue = 0;
void loop() {
  //Serial.println(switchState);
  //Serial.println("loop reached");
  // put your main code here, to run repeatedly:
  Blynk.run();



  currentSensorValue = getSensorValue();
  Serial.println(currentSensorValue);
  if (currentSensorValue > threshold) {
    delay(4000);
    currentSensorValue = getSensorValue();


      if (currentSensorValue > threshold) {
        Serial.println("Triggering");
        if (messageSent == false) {
          wh.trigger();
          messageSent = true;
        }

        while (currentSensorValue > threshold) {

          digitalWrite(5, HIGH);
          delay(100);
          digitalWrite(5, LOW);
          delay(20);
          currentSensorValue = getSensorValue();
          //Serial.println(currentSensorValue);
          Blynk.run();
          switchState = digitalRead(4);
          messageSent = false;
          if (switchState == 1)
          {
            digitalWrite(5, LOW);
            break;

          }


        }
      }

    }


  }

  int getSensorValue()
  {
    unsigned long int sum = 0;
    int avg = 0;
    sum = 0;
    avg = 0;

    for (tindex = 0; tindex < numberOfValues; tindex++)
    {
      sum = sum + analogRead(A0);
      delay(50);
    }
    avg = sum >> 4;


    return avg;

  }

  BLYNK_WRITE(V0) {
    threshold = param.asInt();
    //Serial.println(threshold);
    Blynk.virtualWrite(V1, threshold);


  }
