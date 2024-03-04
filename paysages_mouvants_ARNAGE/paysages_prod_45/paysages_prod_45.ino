#include <Servo.h>
#include <TimeLib.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

Servo organ_servo;  // Organ urn servo object

const byte one_fan = 28; // Urn 1 : One BIG fan
int urn1_start[5]={10, 13, 19, 30, 45}; // Start time offsets in min since Show is ON.
int urn1_stop[5]={12, 17, 20, 35, 50}; // Stop time offsets in min since Show is ON.
byte urn1_index = 4;

const byte organ = 1; // Urn 2: the organic organ
int urn2_start[4]={20, 25, 33, 45}; // Start time offsets in min since Show is ON.
int urn2_stop[4]={23, 28, 38, 48}; // Start time offsets in min since Show is ON.
byte urn2_index = 3;
bool organ_turn_flag=false;
byte rotation_direction=0;

const byte pampa1 = 2; // Urn 3 : The Pampa, fan 1
const byte pampa2 = 3; // Urn 3 : The Pampa, fan 2
const byte pampa3 = 4; // Urn 3 : The Pampa, fan 3
const byte pampa4 = 5; // Urn 3 : The Pampa, fan 4
int urn3_start[4]={15, 25, 35, 45}; // Start time offsets in min since Show is ON.
int urn3_stop[4]={20, 30, 40, 50}; // Start time offsets in min since Show is ON.
byte urn3_index = 3;
bool pampa_turn_flag=false;
byte sub_pampa_timestamp_index =0;

const byte hourglass_tall = 6; // Urn 4 : Tall and long hourglass
// const byte hourglass_small = 7; // Urn 4: THe surprising hourglass

const byte landscape_light = 8; // Urn 5: Light dimmer ouput for landscape Urn
const byte landscape_light_2 = 7; //
byte landscape_light_choice = 0; 


int urn5_start[3]={30, 35, 45}; // Start time offsets in min since Show is ON.
int urn5_stop=50; // Start time offsets in min since Show is ON.
bool landscape_turn_flag=false;
byte landscape_slice_duration=5; // Duration of one of the landscape slice in minutes.
unsigned int landscape_start_timestamp=5; // Duration of one of the landscape slice in minutes.
int landscape_attack_duration = 10; // Duration of final fade in for landscape Urn in minutes. MUST BE GREATER THAN 5 minutes.
const unsigned int timestamp_step = floor(landscape_attack_duration/256);
int pwm_steps_count=0;
bool landscape_next_ts_set = false;
unsigned int landscape_next_timestamp;

byte urn5_index = 2;

const byte start_switch = 23; // Analog input A9 on pin 23 for start button
const byte red_led = 22 ;
const byte green_led = 21 ;

bool show_paused = true;

float block_offset_time = 44.95;

unsigned long start_time = now() + round(block_offset_time*60);


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
  // pinMode(hourglass_small, OUTPUT);

  pinMode(landscape_light, OUTPUT);
  pinMode(landscape_light_2, OUTPUT);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  digitalWrite(landscape_light, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  digitalWrite(landscape_light, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  digitalWrite(landscape_light_2, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  digitalWrite(landscape_light_2, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  digitalWrite(landscape_light, HIGH);
  digitalWrite(landscape_light_2, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  digitalWrite(landscape_light, LOW);
  digitalWrite(landscape_light_2, LOW);

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
        organ_servo.write(85);
      }
      if (rotation_direction== 1){
        organ_servo.write(95);
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

  if (is_valid_index(urn5_index, ARRAY_SIZE(urn5_start))){

    if (is_valid_timestamp(start_time + urn5_start[urn5_index]*60)){
      landscape_next_timestamp = now()*1000;
    }

    if (now()-int(landscape_next_timestamp/1000) >= landscape_slice_duration*60){
      urn5_index +=1;
    }


    switch (urn5_index) {
      case 0:
        if (landscape_next_ts_set ==false){
          landscape_next_timestamp += random(30,1000);
          landscape_next_ts_set = true;
        }
        if (is_valid_millis_timestamp(landscape_start_timestamp*1000+random(30,1000))){
          landscape_light_choice = random(0,4);
          switch (landscape_light_choice){
            case 0:
              // analogWrite(landscape_light, random(50, 255));
              // analogWrite(landscape_light_2, random(50, 255));
              digitalWrite(landscape_light, HIGH);
              digitalWrite(landscape_light_2, HIGH);
              landscape_next_ts_set = false;
              break;
            case 1:
              // analogWrite(landscape_light, random(50, 255));
              digitalWrite(landscape_light, HIGH);
              digitalWrite(landscape_light_2, LOW);
              landscape_next_ts_set = false;
              break;
            
            case 2:
              // analogWrite(landscape_light_2, random(50, 255));
              digitalWrite(landscape_light, LOW);
              digitalWrite(landscape_light_2, HIGH);
              landscape_next_ts_set = false;
              break;
            case 3:
              digitalWrite(landscape_light, LOW);
              digitalWrite(landscape_light_2, LOW);
              landscape_next_ts_set = false;
            
          }
          
        }
        break;
      case 1:
        // analogWrite(landscape_light, 50);
        digitalWrite(landscape_light, HIGH);
        digitalWrite(landscape_light_2, HIGH);
        break;
      case 2:
        digitalWrite(landscape_light, HIGH);
        digitalWrite(landscape_light_2, HIGH);
        break;
    }

    if (is_valid_timestamp(start_time + urn5_stop*60)){
      //analogWrite(landscape_light, 0);
      digitalWrite(landscape_light, LOW);
      digitalWrite(landscape_light_2, LOW);
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
