#include <Servo.h>
#include <TimeLib.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

Servo organ_servo;  // Organ urn servo object
Servo hourglass_servo; // Hourglass servo object
Servo surprise_servo; // Surprise servo object in Hourglass urn

//Urn1 Big Fan
const byte one_fan = 0; // Urn 1 : One BIG fan
int urn1_start[3]={3, 9, 15}; // Start time offsets in min since Show is ON.
int urn1_stop[3]={7, 13, 20}; // Stop time offsets in min since Show is ON.
byte urn1_index = 0;

//Urn2 Organ
const byte organ = 1; // Urn 2: the organic organ

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
const byte landscape_light_2 = 9; 
const byte landscape_light_3 = 10;
const byte landscape_light_4 = 11;
int landscape_pin_array[4] = {8,9,10,11};

int urn5_start[3]={0, 8, 15}; // Start time offsets in min since Show is ON.
int urn5_stop[3]={5, 13, 20}; // Start time offsets in min since Show is ON.
byte urn5_index = 0;
bool landscape_light_on = false;
unsigned long landscape_light_duration = 0;
unsigned long actual_landscape_timestamp = 0;
unsigned long previous_landscape_timestamp = 0;

byte min_led_power = 0; // Puisance minimale des LEDS;
byte max_led_power = 64; // Puisance maximale des LEDS (valeur max 256);
int min_light_duration = 20; // Durée minimale d'éclairage d'une LED en ms
int max_light_duration = 500; // // Durée maximale d'éclairage d'une LED en ms

byte time_factor=1; // 1 for working in seconds, 60 for working in minutes
byte block_offset_time = 0;
unsigned long start_time = now() + block_offset_time*60;

void setup() {
  pinMode(13, OUTPUT);

  //Urn 1 BIG FAN
  pinMode(one_fan, OUTPUT);
  digitalWrite(one_fan, 0);

  //Urn 2 ORGAN
  pinMode(organ, OUTPUT);
  organ_servo.attach(organ);
  organ_servo.write(90);

  //Urn 3 PAMPA
  pinMode(pampa1, OUTPUT);
  pinMode(pampa2, OUTPUT);
  pinMode(pampa3, OUTPUT);
  pinMode(pampa4, OUTPUT);

  pampas_fans_off();

  //Urn 4 Hourglass
  pinMode(hourglass_tall, OUTPUT);
  hourglass_servo.attach(hourglass_tall);
  hourglass_servo.write(90);

  surprise_servo.attach(hourglass_surprise);
  surprise_servo.write(90);

  //Urn 5 Landscape
  pinMode(landscape_light, OUTPUT);
  pinMode(landscape_light_2, OUTPUT);
  pinMode(landscape_light_3, OUTPUT);
  pinMode(landscape_light_4, OUTPUT);

  blink_internal_led();

  start_time = now() + block_offset_time*60;
}

void loop() {

  //Urn 1 BIG Fan
  if (is_valid_index(urn1_index, ARRAY_SIZE(urn1_start))){
    if (is_valid_timestamp(start_time + urn1_start[urn1_index]*time_factor)){
      digitalWrite(one_fan, HIGH);
    }
    if (is_valid_timestamp(start_time + urn1_stop[urn1_index]*time_factor)){
      digitalWrite(one_fan, LOW);
      urn1_index +=1;
    }
  }

  //Urn 5 Landscapes
  actual_landscape_timestamp = millis();

  if (is_valid_index(urn5_index, ARRAY_SIZE(urn5_start))){

    if (is_valid_timestamp(start_time + urn5_start[urn5_index]*time_factor)){
      landscape_light_on = 1;      
    }

    if (is_valid_timestamp(start_time + urn5_stop[urn5_index]*time_factor)){
      for (int i=0; i<4; i++){
        analogWrite(landscape_pin_array[i], 0);
      }
      landscape_light_on = 0;
      urn5_index +=1;
    }

    if(landscape_light_on){
        if (actual_landscape_timestamp - previous_landscape_timestamp > landscape_light_duration){
          analogWrite(landscape_pin_array[random(0,4)],random(min_led_power, max_led_power));
          landscape_light_duration = random(min_light_duration, max_light_duration);
          previous_landscape_timestamp = millis();
        }
    }
  }
}

bool is_valid_index(byte index, int size_of_array){
  if (index <= size_of_array){
    return true;
  }
  return false;
}

bool is_valid_timestamp(unsigned long timestamp){
  if ((timestamp - 1 <= now()+ block_offset_time*60) && (now()+ block_offset_time*60 <= timestamp + 1)){
    return true;
  }
  return false;
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

void pampas_fans_off(void){
  analogWrite(pampa1, 0);
  analogWrite(pampa2, 0);
  analogWrite(pampa3, 0);
  analogWrite(pampa4, 0);
}