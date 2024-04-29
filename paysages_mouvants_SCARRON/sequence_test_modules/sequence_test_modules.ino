#include <Servo.h>
#include <TimeLib.h>

Servo organ_servo;  // Organ urn servo object
Servo hourglass_servo; // Hourglass servo object
Servo surprise_servo; // Surprise servo object in Hourglass urn

//Urn1 Big Fan
const byte one_fan = 0; // Urn 1 : One BIG fan

//Urn2 Organ
const byte organ = 1; // Urn 2: the organic organ*

//Urn 3 Pampa
const byte pampa1 = 2; // Urn 3 : The Pampa, fan 1
const byte pampa2 = 3; // Urn 3 : The Pampa, fan 2
const byte pampa3 = 4; // Urn 3 : The Pampa, fan 3
const byte pampa4 = 5; // Urn 3 : The Pampa, fan 4

//Urn4 Hourglass
const byte hourglass_tall = 6; // Urn 4 : Tall and long hourglass
const byte hourglass_surprise = 7; // Urn 4: THe surprising hourglass

//Urn 5 Landscape
const byte landscape_light = 8; // Urn 5: Light dimmer ouput for landscape Urn
const byte landscape_light_2 = 9; //
const byte landscape_light_3 = 10;
const byte landscape_light_4 = 11;

int leds[6] = {8,9,10,11};

void setup() {
  pinMode(13, OUTPUT);

//Urn 1 BIG FAN
  pinMode(one_fan, OUTPUT);

//Urn 2 ORGAN
  pinMode(organ, OUTPUT);
  organ_servo.attach(organ);

//Urn 3 PAMPA
  pinMode(pampa1, OUTPUT);
  pinMode(pampa2, OUTPUT);
  pinMode(pampa3, OUTPUT);
  pinMode(pampa4, OUTPUT);
  analogWriteResolution(8);
  analogWriteFrequency(3, 234375); // Teensy 4.1 pin 3 and 4 234.375 kHz
  analogWriteFrequency(5, 234375); // Teensy 4.1 pin 5 and 6 also changes to 375 kHz

//Urn 4 Hourglass
  pinMode(hourglass_tall, OUTPUT);
  hourglass_servo.attach(hourglass_tall);
  surprise_servo.attach(hourglass_surprise);

//Urn 5 Landscape
  pinMode(landscape_light, OUTPUT);
  pinMode(landscape_light_2, OUTPUT);
  pinMode(landscape_light_3, OUTPUT);
  pinMode(landscape_light_4, OUTPUT);

  blink_internal_led();


}

void loop() {

// Test Urne 1, ventilateur
digitalWrite(one_fan, HIGH);
delay(2000);
digitalWrite(one_fan, LOW);
delay(1000);
digitalWrite(one_fan, HIGH);
delay(2000);
digitalWrite(one_fan, LOW);

//=====================================
// Test Urne 2, coeur
organ_servo.write(50);
delay(500);
organ_servo.write(90);
delay(1000);
organ_servo.write(140);
delay(500);
organ_servo.write(90);

//=====================================
// Test Urne 3, pampas
pampas_fans_on();
delay(2000);
pampas_fans_off();
delay(1000);
pampas_fans_on();
delay(2000);
pampas_fans_off();
delay(1000);

//=====================================
// Test Urne 4, Sablier
hourglass_servo.write(50);
delay(1000);
hourglass_servo.write(90);
delay(1000);
hourglass_servo.write(140);
delay(1000);
hourglass_servo.write(90);

// Test urne 4, Surprise
surprise_servo.write(50);
delay(1000);
surprise_servo.write(90);
delay(1000);
surprise_servo.write(140);
delay(1000);
surprise_servo.write(90);

//=====================================
// Test Urne 5, Paysages
for(int j=0; j<3; j++){
  for (int i=0; i<4; i++){
    analogWrite(leds[i],128);
    delay(500);
  }
} 

}

void pampas_fans_on(void){
  analogWrite(pampa1, 255);
  analogWrite(pampa2, 255);
  analogWrite(pampa3, 255);
  analogWrite(pampa4, 255);
}

void pampas_fans_off(void){
  analogWrite(pampa1, 0);
  analogWrite(pampa2, 0);
  analogWrite(pampa3, 0);
  analogWrite(pampa4, 0);
}

void blink_internal_led(){
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
}
