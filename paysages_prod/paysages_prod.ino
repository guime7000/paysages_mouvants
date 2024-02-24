#include <Servo.h>
#include <TimeLib.h>

const byte one_fan = 32; // Urn 1 : One BIG fan
unsigned int urn1_start[5]={10, 13, 18, 30, 45}; // Start time offsets in min since Show is ON.
unsigned int urn1_stop[5]={12, 17, 20, 35, 50}; // Stop time offsets in min since Show is ON.
byte urn1_index = 0;

const byte organ = 1; // Urn 2: the organic organ
unsigned int urn2_start[4]={18, 25, 33, 43}; // Start time offsets in min since Show is ON.
unsigned int urn2_stop[4]={23, 28, 38, 48}; // Start time offsets in min since Show is ON.
byte urn2_index = 0;

const byte pampa1 = 2; // Urn 3 : The Pampa, fan 1
const byte pampa2 = 3; // Urn 3 : The Pampa, fan 2
const byte pampa3 = 4; // Urn 3 : The Pampa, fan 3
const byte pampa4 = 5; // Urn 3 : The Pampa, fan 4
unsigned int urn3_start[4]={15, 25, 35, 45}; // Start time offsets in min since Show is ON.
unsigned int urn3_stop[4]={20, 30, 40, 50}; // Start time offsets in min since Show is ON.
byte urn3_index = 0;

const byte hourglass_tall = 6; // Urn 4 : Tall and long hourglass
const byte hourglass_small = 7; // Urn 4: THe surprising hourglass

const byte landscape_light = 8; // Urn 5: Light dimmer ouput for landscape Urn
const byte landscape_motor_1 = 9; // Urn 5 : Motor 1
const byte landscape_motor_2 = 10; // Urn 5 : Motor 2
const byte landscape_motor_3 = 11; // Urn 5 : Motor 3
const byte landscape_motor_4 = 12; // Urn 5 : Motor 1
unsigned int urn5_start[3]={30, 35, 40}; // Start time offsets in min since Show is ON.
unsigned int urn5_stop=50; // Start time offsets in min since Show is ON.
byte urn5_index = 0;

const byte start_switch = 23; // Analog input A9 on pin 23 for start button
const byte red_led = 22 ;
const byte green_led = 21 ;

bool show_paused = true;
unsigned long start_time = millis();


void setup() {
  pinMode(one_fan, OUTPUT);

  pinMode(organ, OUTPUT);

  pinMode(pampa1, OUTPUT);
  pinMode(pampa2, OUTPUT);
  pinMode(pampa3, OUTPUT);
  pinMode(pampa4, OUTPUT);

  pinMode(hourglass_tall, OUTPUT);
  pinMode(hourglass_small, OUTPUT);

  pinMode(landscape_light, OUTPUT);
  pinMode(landscape_motor_1, OUTPUT);
  pinMode(landscape_motor_2, OUTPUT);
  pinMode(landscape_motor_3, OUTPUT);
  pinMode(landscape_motor_4, OUTPUT);

  pinMode(start_switch, INPUT);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);


}

void loop() {

  while(show_paused){
    start_time = now();
    digitalWrite(red_led, HIGH);
    digitalWrite(green_led, LOW);
    if(digitalRead(start_switch)){
      digitalWrite(green_led, HIGH);
      digitalWrite(red_led, LOW);
      show_paused = false;
    }
  }

  if is_valid_timestamp(start_time + urn1_start[urn1_index]*60){
    digitalWrite(one_fan, HIGH);
  }
  if is_valid_timestamp(start_time + urn1_stopœ[urn1_index]*60){
    digitalWrite(one_fan, Low);
    urn1_index +=1;
  }

  #FIXME : write servo function  
  if is_valid_timestamp(start_time + urn2_start[urn1_index]*60){
  }
  if is_valid_timestamp(start_time + urn2_stop[urn1_index]*60){
    urn2_index +=1;
  }

  #FIXME : write 4 fans function  
  if is_valid_timestamp(start_time + urn3_start[urn1_index]*60){
  }
  if is_valid_timestamp(start_time + urn3_stop[urn1_index]*60){
    urn3_index +=1;
  }


  #FIXME : write 3 functions for landscape urn
  if is_valid_timestamp(start_time + urn5_start[urn1_index]*60){
  }
  if is_valid_timestamp(start_time + urn5_stop*60){
    urn5_index +=1;
  }


}




bool is_valid_timestamp(unsigned long timestamp){
  if ((timestamp - 1 <= now()) && (now() <= timestamp + 1)){
    return true;
  }
  return false;
}
