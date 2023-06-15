# AlexaControlledAC
Automating the control of an older model Hitachi Air conditioner and a simple relay control with an ESP32 and an Alexa Echo Dot.

# Why do this?
My aircon remote control is always out of reach and im Wayyy too lazy to get off my ass to get it to change the temperature or set a timer. So instead I spent 3 weeks of my life developing this(most of it on the electronics).

# Methodology
I used a NodeMCU ESP32 board for this project as it was easier. You can use a ESP8266 if you want to. The first step was to figure out how to connect the board to my Echo dot. That part was pretty easy, I just used [Espalexa](https://github.com/Aircoookie/Espalexa) and built off one of their basic relay control examples. Next was trying to figure out what protocal my Aircon cassette runs on, that one was quite a pain in the ass. After messing around with the [IR recorders](https://github.com/crankyoldgit/IRremoteESP8266) for a week trying to decode the protocal from my existing remote, I found that the [SmartIRRepeater](https://github.com/crankyoldgit/IRremoteESP8266/tree/master/examples/SmartIRRepeater) was able to decode the signal. But in the end that was kinda pointless cuz there was also this [CommonACcontrol](https://github.com/crankyoldgit/IRremoteESP8266/tree/master/examples/CommonAcControl) which basically cycles thru every single protocal they have untill one of them turns it on. Which worked alot better. But do try the [SmartIRRepeater](https://github.com/crankyoldgit/IRremoteESP8266/tree/master/examples/SmartIRRepeater) to see if it helps you immediately identify what protcal you are using. 

# Electronics
