/*
 * This is a basic example on how to use Espalexa and its device declaration methods.
 */ 

#include <WiFi.h>
#include <Espalexa.h>

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>

const uint16_t kIrLed = 4;  // The ESP GPIO pin to use that controls the IR LED.
IRac ac(kIrLed);  // Create a A/C object using GPIO to sending messages with.

#define Ceiling_Fan_Relay 5

// prototypes
boolean connectWifi();

//callback functions
void celingFanChanged(uint8_t brightness);
void ACChanged(uint8_t brightness);
void ACTimer(uint8_t brightness);

// Change this!!
const char* ssid = "Router SSID here";
const char* password = "Router Password here";

boolean wifiConnected = false;

Espalexa espalexa;

int ACtime;

unsigned long prevtime = 0;

boolean timerState = false;


//EspalexaDevice* device3; //this is optional

void setup()
{

  pinMode(Ceiling_Fan_Relay, OUTPUT);
  // pinMode(LED_Relay, OUTPUT);

  Serial.begin(115200);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    
    // Define your devices here. 
    espalexa.addDevice("Ceiling Fan", celingFanChanged); //simplest definition, default state off
    espalexa.addDevice("Aircon", ACChanged, 70); //third parameter is beginning state 
    espalexa.addDevice("Aircon Timer", ACTimer, 60); //third parameter is beginning state 
    
    // device3 = new EspalexaDevice("Light 3", thirdLightChanged); //you can also create the Device objects yourself like here
    // espalexa.addDevice(device3); //and then add them
    // device3->setValue(128); //this allows you to e.g. update their state value at any time!

    espalexa.begin();
    ac.next.protocol = decode_type_t::HITACHI_AC344;  // Set a protocol to use.
    ac.next.model = 1;  // Some A/Cs have different models. Try just the first.
    ac.next.mode = stdAc::opmode_t::kCool;  // Run in cool mode initially.
    ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
    ac.next.degrees = 25;  // 25 degrees.
    ac.next.fanspeed = stdAc::fanspeed_t::kAuto;  // Start the fan at medium.
    ac.next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
    ac.next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
    ac.next.light = false;  // Turn off any LED/Lights/Display that we can.
    ac.next.beep = false;  // Turn off any beep from the A/C if we can.
    ac.next.econo = false;  // Turn off any economy modes if we can.
    ac.next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
    ac.next.turbo = false;  // Don't use any turbo/powerful/etc modes.
    ac.next.quiet = false;  // Don't use any quiet/silent/etc modes.
    ac.next.sleep = -1;  // Don't set any sleep time or modes.
    ac.next.clean = false;  // Turn off any Cleaning options if we can.
    ac.next.clock = -1;  // Don't set any current time if we can avoid it.
    ac.next.power = false;  // Initially start with the unit off.
      
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}
 
void loop()
{
   espalexa.loop();
   delay(1);
   if(timerState == true){ // I wanted more precise control over how long my air conditioner stays on
     if(millis() - prevtime > ACtime * 60000){
        ac.next.power = false;  // We want to turn off the A/C unit.
        ac.sendAc();  // Have the IRac class create and send a message.
        Serial.println("Timer Up");
        timerState = false;
     }
   }
}

//our callback functions
void celingFanChanged(uint8_t brightness) {
    Serial.print("Celing Fan changed to ");
    
    //do what you need to do here

    //EXAMPLE
    if (brightness) {
      if(brightness == 255){
        digitalWrite(Ceiling_Fan_Relay, HIGH);
      }
      
      Serial.print("ON");
      Serial.println(brightness);
    }
    else  {
      digitalWrite(Ceiling_Fan_Relay, LOW);
      Serial.println("OFF");
    }
}

void ACChanged(uint8_t brightness) {
    Serial.print("Air Con changed to ");
    
    //do what you need to do here

    //EXAMPLE
    if (brightness) {
      int temp = brightness/2.55;
      if(temp > 28){temp = 28;}
      if(temp < 18){temp = 18;}
      ac.next.power = true;  // We want to turn on the A/C unit.
      ac.next.fanspeed = stdAc::fanspeed_t::kAuto;
      ac.next.degrees = temp;
      ac.sendAc();  // Have the IRac class create and send a message.
      
      Serial.print("ON, Temp: ");
      Serial.print(temp);
      Serial.println("C");
    }
    else  {
      ac.next.power = false;  // We want to turn off the A/C unit.
      ac.sendAc();  // Have the IRac class create and send a message.
      Serial.println("OFF");
    }
}

void ACTimer(uint8_t brightness) { // Alexa takes in brightness values from 0% to 100%. Which is interpreted here as 0 to 255. Im just converting that range back to 0 to 100 and using that to set a timer in minutes.
    Serial.print("Air Con Timer Set to: ");   
    //do what you need to do here
    //EXAMPLE
    if (brightness) {
      // Have the IRac class create and send a message.
      ACtime = (int) brightness/2.55;
      Serial.print("ON for: ");
      Serial.print(ACtime);
      Serial.println("mins");
      prevtime = millis();
      timerState = true;
    }
    else  {}
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
