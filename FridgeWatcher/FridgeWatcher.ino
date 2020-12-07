#include <arduino-timer.h>
#include <driver/rtc_io.h>

// Set GPIO and constants
const int beeperPin = 21;
const int LedPin = 18;
const int inputPin = 15;
const int timeVal = 40000; // in milliseconds 
auto timer = timer_create_default(); //1 Task timer

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(beeperPin, OUTPUT);
  pinMode(LedPin, OUTPUT);
  pinMode(inputPin, INPUT);

  //set time job
  timer.in(timeVal, beep);
  //use same pin to wake up from sleep
  rtc_gpio_pulldown_en((gpio_num_t)inputPin);                  //Enables ESP32 pullup or pulldown resistor during deep sleep
  esp_sleep_enable_ext0_wakeup((gpio_num_t)inputPin, RISING);
}

bool beep(void* p){
     Serial.println("beeping..");
     digitalWrite(beeperPin, HIGH); 
     return false;
}

void checkDoor(){
  //led on when door is open
  digitalWrite(LedPin, HIGH);
  while(digitalRead(inputPin)== HIGH){
    timer.tick();
   }
    //if we got here, the door was closed again
    timer.cancel();
    timer.in(timeVal, beep);
    digitalWrite(beeperPin, LOW);   
      digitalWrite(LedPin, LOW);

 }

void loop(){

  Serial.println("were Awake");
  checkDoor();
  Serial.println("going to sleep");
  esp_deep_sleep_start();  // Sleep for ever until switch pressed
  }
