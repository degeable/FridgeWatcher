#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <arduino-timer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <driver/rtc_io.h>

// Replace with your network credentials
const char* ssid = "aaaaaaaaaaaaaa";
const char* password = "ef!S#sC3B%VGWBGzvBg!";
 Timer<1, millis, const char *> timer;
// NTP stuff
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String formattedDate;
String dayStamp;
String timeStamp;

// Set GPIO and constants
const int beeperPin = 21;
const int inputPin = 15;
const int timeVal = 70000; // in milliseconds + 
bool justWroteLog = false;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(beeperPin, OUTPUT);
  pinMode(inputPin, INPUT);
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // set hostname
  WiFi.setHostname("Schrank");
  // Print ESP32 Local IP Address and hostname
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getHostname());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600);
  timer.in(timeVal, beep, "Tür wurde zu lange aufgelassen!1!!");

  rtc_gpio_pulldown_en((gpio_num_t)inputPin);                  //Enables ESP32 pullup or pulldown resistor during deep sleep
  esp_sleep_enable_ext0_wakeup((gpio_num_t)inputPin, RISING);
}


void writeLogEntry(const char *msg) {
  File file = SPIFFS.open("/index.html", "a");
  file.print("<li>");
  file.print(timeClient.getFormattedDate());
  file.print(" - ");
  file.print(msg);
  file.println("</li>");
  file.close();

}
bool beep(const char *msg){
     Serial.println("beeping..");
     writeLogEntry(msg);
     digitalWrite(beeperPin, HIGH); 
     return false;
}

void checkDoor(){
  int b = digitalRead(inputPin);
  if ( b == HIGH)
    { 
      if (justWroteLog == false){
      Serial.println("wrote a log entry");
      writeLogEntry("Kühlschrank wurde geöffnet");
      justWroteLog = true;
      }
      while(digitalRead(inputPin)== HIGH){
      timer.tick();
      }
      //if we got here, the door was closed again
      timer.cancel();
      timer.in(timeVal, beep, "Tür wurde zu lange aufgelassen!1!!");
      delay(3000);
      justWroteLog = false;
      digitalWrite(beeperPin, LOW); 
    
    }
    

    
}

void loop(){

  Serial.println("were Awake");
  while(!timeClient.update()) {
      timeClient.forceUpdate();
    }
  checkDoor();
  esp_deep_sleep_start();  // Sleep for ever until switch pressed
  }
