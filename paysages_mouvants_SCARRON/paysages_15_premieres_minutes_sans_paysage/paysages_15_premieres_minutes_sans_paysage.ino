#include <Servo.h>
#include <TimeLib.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

Servo organ_servo;  // Organ urn servo object
Servo hourglass_servo; 

const byte one_fan = 28; // Urn 1 : One BIG fan
int urn1_start[5]={10, 13, 17, 30, 45}; // Start time offsets in min since Show is ON.
int urn1_stop[5]={12, 15, 18, 35, 50}; // Stop time offsets in min since Show is ON.
byte urn1_index = 0;

const byte organ = 1; // Urn 2: the organic organ
int urn2_start[4]={18, 25, 33, 43}; // Start time offsets in min since Show is ON.
int urn2_stop[4]={23, 28, 38, 48}; // Start time offsets in min since Show is ON.
byte urn2_index = 0;
bool organ_turn_flag=false;
byte rotation_direction=0;

const byte pampa1 = 2; // Urn 3 : The Pampa, fan 1
const byte pampa2 = 3; // Urn 3 : The Pampa, fan 2
const byte pampa3 = 4; // Urn 3 : The Pampa, fan 3
const byte pampa4 = 5; // Urn 3 : The Pampa, fan 4
int urn3_start[4]={15, 25, 35, 45}; // Start time offsets in min since Show is ON.
int urn3_stop[4]={20, 30, 40, 50}; // Start time offsets in min since Show is ON.
byte urn3_index = 0;
bool pampa_turn_flag=false;
byte sub_pampa_timestamp_index =0;

const byte hourglass_tall = 6; // Urn 4 : Tall and long hourglass
// const byte hourglass_small = 7; // Urn 4: THe surprising hourglass
unsigned long hourglass_rotation_duration=300; // Durée de rotation du moteur du sablier (en ms)
unsigned long actual_hourglass_time;
unsigned long previous_hourglass_time=0;

const byte landscape_light = 8; // Urn 5: Light dimmer ouput for landscape Urn
const byte landscape_light_2 = 7; //
byte landscape_light_choice = 0; 

const byte landscape_motor_1 = 9; // Urn 5 : Motor 1
const byte landscape_motor_2 = 10; // Urn 5 : Motor 2
const byte landscape_motor_3 = 11; // Urn 5 : Motor 3
const byte landscape_motor_4 = 12; // Urn 5 : Motor 1
int urn5_start[3]={30, 35, 40}; // Start time offsets in min since Show is ON.
int urn5_stop=50; // Start time offsets in min since Show is ON.
bool landscape_turn_flag=false;
byte landscape_slice_duration=5; // Duration of one of the landscape slice in minutes.
unsigned int landscape_start_timestamp=5; // Duration of one of the landscape slice in minutes.
int landscape_attack_duration = 10; // Duration of final fade in for landscape Urn in minutes. MUST BE GREATER THAN 5 minutes.
const unsigned int timestamp_step = floor(landscape_attack_duration/256);
int pwm_steps_count=0;
bool landscape_next_ts_set = false;
unsigned int landscape_next_timestamp;

byte urn5_index = 0;

const byte start_switch = 23; // Analog input A9 on pin 23 for start button
const byte red_led = 22 ;
const byte green_led = 21 ;

bool show_paused = true;

byte block_offset_time = 0;

unsigned long start_time = now() + block_offset_time*60;


void setup() {
  pinMode(one_fan, OUTPUT);

  pinMode(organ, OUTPUT);
  organ_servo.attach(organ);

  pinMode(pampa1, OUTPUT);
  pinMode(pampa2, OUTPUT);
  pinMode(pampa3, OUTPUT);
  pinMode(pampa4, OUTPUT);
  analogWriteResolution(8);
  analogWriteFrequency(3, 234375); // Teensy 4.1 pin 3 and 4 234.375 kHz
  analogWriteFrequency(5, 234375); // Teensy 4.1 pin 5 and 6 also changes to 375 kHz

  pinMode(hourglass_tall, OUTPUT);
  hourglass_servo.attach(hourglass_tall);
  // pinMode(hourglass_small, OUTPUT);

  pinMode(landscape_light, OUTPUT);
  pinMode(landscape_light_2, OUTPUT);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);

  start_time = now() + block_offset_time*60;

}

void loop() {

  // while(show_paused){
  //   start_time = now();
  //   digitalWrite(red_led, HIGH);
  //   digitalWrite(green_led, LOW);
  //   if(digitalRead(start_switch)){
  //     digitalWrite(green_led, HIGH);
  //     digitalWrite(red_led, LOW);
  //     show_paused = false;
  //   }
  // }

  // Hourglass / Sablier
  actual_hourglass_time = millis();
  hourglass_servo.write(10);
  if (actual_hourglass_time - previous_hourglass_time > hourglass_rotation_duration){
    hourglass_servo.write(90);
    previous_hourglass_time = millis();
  }

  if (is_valid_index(urn1_index, ARRAY_SIZE(urn1_start))){
    if (is_valid_timestamp(start_time + urn1_start[urn1_index]*60)){
      digitalWrite(one_fan, HIGH);
    }
    if (is_valid_timestamp(start_time + urn1_stop[urn1_index]*60)){
      digitalWrite(one_fan, LOW);
      urn1_index +=1;
    }
  }

  if (is_valid_index(urn2_index, ARRAY_SIZE(urn2_start))){
    
    if (organ_turn_flag){
      if (rotation_direction== 0){
        organ_servo.write(80);
      }
      if (rotation_direction== 1){
        organ_servo.write(100);
      }
    }
    if (organ_turn_flag == false){
      organ_servo.write(90);
    }

    if (is_valid_timestamp(start_time + urn2_start[urn2_index]*60)){
      organ_turn_flag = true;
      rotation_direction = random(0,2);
    }

    if (is_valid_timestamp(start_time + urn2_stop[urn2_index]*60)){
      organ_turn_flag = false;
      rotation_direction = random(0,2);
      urn2_index +=1;
    }
  }

  if (is_valid_index(urn3_index, ARRAY_SIZE(urn3_start))){
    
    if (is_valid_timestamp(start_time + urn3_start[urn3_index]*60+sub_pampa_timestamp_index*60) && pampa_turn_flag){
      pampas_fans();
      sub_pampa_timestamp_index +=1;
    }
    
    if (is_valid_timestamp(start_time + urn3_start[urn3_index]*60)){
      pampa_turn_flag= true;;
    }

    if (is_valid_timestamp(start_time + urn3_stop[urn3_index]*60)){
      pampa_turn_flag = false;
      sub_pampa_timestamp_index = 0;
      urn3_index +=1;
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
  if ((timestamp - 1 <= now()) && (now() <= timestamp + 1)){
    return true;
  }
  return false;
}

bool is_valid_millis_timestamp(unsigned long timestamp){
  if ((timestamp - 1000 <= now()*1000) && (now()*1000 <= timestamp + 1000)){
    return true;
  }
  return false;
}

void pampas_fans(void){
  analogWrite(pampa1, random(80, 256));
  analogWrite(pampa2, random(80, 256));
  analogWrite(pampa3, random(80, 256));
  analogWrite(pampa4, random(80, 256));
}
