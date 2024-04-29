// TODO: rajouter la surprise
// TODO: gestion lumières
// TODO: Gestion codes par blocs de temps
#include <Servo.h>
#include <TimeLib.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

Servo organ_servo;  // Organ urn servo object
Servo hourglass_servo; // Hourglass servo object
Servo surprise_servo; // Surprise servo object in Hourglass urn

// Push buttons config
const byte start_push_button = 23; // Sortie pour bouton poussoir Rouge (top départ)
bool start_push_button_state = 0;

const byte hourglass_push_button = 22; // Bouton poussoir déroulage sablier
bool hourglass_push_button_state = 0;

const byte surprise_push_button = 21; // Bouton poussoir déroulage de la surprise
bool surprise_push_button_state = 0;

//Urn1 Big Fan
const byte one_fan = 0; // Urn 1 : One BIG fan
int urn1_start[5]={0, 3, 7, 20, 35}; // Start time offsets in min since Show is ON.
int urn1_stop[5]={2, 5, 8, 25, 40}; // Stop time offsets in min since Show is ON.
byte urn1_index = 0;

//Urn2 Organ
const byte organ = 1; // Urn 2: the organic organ*
// ======== ARNAGE settings for Organ =======================
// int urn2_start[4]={18, 25, 33, 43}; // Start time offsets in min since Show is ON.
// int urn2_stop[4]={23, 28, 38, 48}; // Start time offsets in min since Show is ON.
// ==========================================================

int urn2_start[2]={12, 25}; // Start time offsets in min since Show is ON.
int urn2_stop[2]={18, 28}; // Start time offsets in min since Show is ON.
byte urn2_index = 0;
bool organ_turn_flag=false;
byte rotation_direction=0;

//Urn 3 Pampa
const byte pampa1 = 2; // Urn 3 : The Pampa, fan 1
const byte pampa2 = 3; // Urn 3 : The Pampa, fan 2
const byte pampa3 = 4; // Urn 3 : The Pampa, fan 3
const byte pampa4 = 5; // Urn 3 : The Pampa, fan 4
int urn3_start[4]={5, 15, 25, 35}; // Start time offsets in min since Show is ON.
int urn3_stop[4]={10, 20, 30, 40}; // Start time offsets in min since Show is ON.
byte urn3_index = 0;
bool pampa_turn_flag=false;
byte sub_pampa_timestamp_index=0;

//Urn4 Hourglass
const byte hourglass_tall = 6; // Urn 4 : Tall and long hourglass
const byte hourglass_surprise = 7; // Urn 4: THe surprising hourglass

int urn4_start=0; // Start time offsets in min since Show is ON.
unsigned long hourglass_pause_duration=250; // Durée de pause de rotation du moteur du sablier (en ms)
unsigned long hourglass_rotation_duration=5000; // Durée de rotation du moteur du sablier (en ms)
byte hourglass_rise_speed = 60; // Réglage de la vitesse de montée du sablier : 0 = très rapide, 90 = arrêté

bool hourglass_rotates=false;
bool hourglass_is_launched = 0;
unsigned long actual_hourglass_time=0;
unsigned long previous_hourglass_time=0;

//Urn 5 Landscape
const byte landscape_light = 8; // Urn 5: Light dimmer ouput for landscape Urn
const byte landscape_light_2 = 9; //
const byte landscape_light_3 = 10;
const byte landscape_light_4 = 11;

int urn5_start[3]={20, 25, 30}; // Start time offsets in min since Show is ON.
float urn5_stop[3]={24.95, 29.95, 40}; // Start time offsets in min since Show is ON.

byte landscape_light_choice = 0;
bool landscape_turn_flag=false;
byte landscape_slice_duration=10; // Duration of one of the landscape slice in minutes.
unsigned int landscape_start_timestamp=5; // Duration of one of the landscape slice in minutes.
int landscape_attack_duration = 10; // Duration of final fade in for landscape Urn in minutes. MUST BE GREATER THAN 5 minutes.
const unsigned int timestamp_step = floor(landscape_attack_duration/256);
int pwm_steps_count=0;
bool landscape_next_ts_set = false;
unsigned int landscape_next_timestamp;
bool lights_on=false;
byte randomized_light=0;

unsigned long actual_landscape_time;
unsigned long previous_landscape_time=0;

byte urn5_index = 0;

byte time_factor=60; // 1 for working in seconds, 60 for working in minutes
byte block_offset_time = 0;
unsigned long start_time = now() + block_offset_time*60;

void setup() {
  pinMode(13, OUTPUT);

// Push buttons initialization
  pinMode(start_push_button, INPUT_PULLUP);
  pinMode(hourglass_push_button, INPUT_PULLUP);
  pinMode(surprise_push_button, INPUT_PULLUP);

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

  blink_internal_led();

  start_time = now() + block_offset_time*60;
}

void loop() {

// ========= Waiting for start Push button to be used ==============
// start_push_button_state = digitalRead(start_push_button);
//   while (!start_push_button_state){
//     hourglass_servo.write(140);
//     }
// ==================================================================


// // Pour dérouler le sablier qd nécessaire 
// //Rester appuyer sur le bouton pour dérouler. Relâcher le bouton pour arrêter le moteur.
// hourglass_push_button_state = digitalRead(hourglass_push_button);
//   while (!hourglass_push_button_state){
//     hourglass_servo.write(140);
//     }

// // Pour dérouler la surprise qd nécessaire
// //Rester appuyer sur le bouton pour dérouler. Relâcher le bouton pour arrêter le moteur.
// surprise_push_button_state = digitalRead(surprise_push_button);
//   while (!surprise_push_button_state){
//     hourglass_servo.write(140);
//     }

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

//Urn 2 Organ
if (is_valid_index(urn2_index, ARRAY_SIZE(urn2_start))){

    if (organ_turn_flag){
      if (rotation_direction== 0){
        organ_servo.write(80);
      }
      if (rotation_direction== 1){
        organ_servo.write(80);

      }
    }
    if (organ_turn_flag == false){
      organ_servo.write(90);
    }

    if (is_valid_timestamp(start_time + urn2_start[urn2_index]*time_factor)){
      organ_turn_flag = true;
      rotation_direction = random(0,2);
    }

    if (is_valid_timestamp(start_time + urn2_stop[urn2_index]*time_factor)){
      organ_turn_flag = false;
      rotation_direction = random(0,2);
      urn2_index +=1;
    }
  }

//Urn 3 Pampa
if (is_valid_index(urn3_index, ARRAY_SIZE(urn3_start))){

    if (is_valid_timestamp(start_time + urn3_start[urn3_index]*time_factor+sub_pampa_timestamp_index*time_factor) && pampa_turn_flag){
      pampas_fans_on();
      sub_pampa_timestamp_index +=1;
    }

    if (is_valid_timestamp(start_time + urn3_start[urn3_index]*time_factor)){
      pampa_turn_flag= true;;
    }

    if (is_valid_timestamp(start_time + urn3_stop[urn3_index]*time_factor)){
      pampas_fans_off();
      pampa_turn_flag = false;
      sub_pampa_timestamp_index = 0;
      urn3_index +=1;
    }
  }

// Urn 4 Hourglass

  if (is_valid_timestamp(start_time + urn4_start*time_factor)){
    hourglass_rotates = true;
    hourglass_is_launched = 1;
    }

  actual_hourglass_time = millis();
  hourglass_servo.write(90);

  if (actual_hourglass_time - previous_hourglass_time > hourglass_pause_duration && !hourglass_rotates && hourglass_is_launched){
    hourglass_rotates = true;
    previous_hourglass_time = millis();
  }

  if (hourglass_rotates){
    hourglass_servo.write(hourglass_rise_speed);
    if (actual_hourglass_time - previous_hourglass_time > hourglass_rotation_duration){
      hourglass_rotates = false;
      previous_hourglass_time = millis();
    }
  }

//Urn 5

if (is_valid_index(urn5_index, ARRAY_SIZE(urn5_start))){

    if (is_valid_timestamp(start_time + urn5_start[urn5_index]*time_factor)){

      landscape_next_timestamp = now()*1000;
      landscape_start_timestamp = now();
      lights_on = true;
    }

    if (is_valid_timestamp(start_time + urn5_stop[urn5_index]*time_factor)){
      urn5_index +=1;
      lights_on = false;
      digitalWrite(landscape_light, LOW);
      digitalWrite(landscape_light_2, LOW);

      digitalWrite(13, LOW);
    }

    if(lights_on){
      
        if (urn5_index==0){
          if (landscape_next_ts_set ==false){
          actual_landscape_time = millis();
            // landscape_next_timestamp += random(30,100);
            landscape_next_timestamp = random(30,100);
            landscape_next_ts_set = true;
            randomized_light = random(0,4);
      digitalWrite(13, LOW);

          }
            digitalWrite(landscape_light, HIGH);
            digitalWrite(landscape_light_2, HIGH);
      digitalWrite(13, HIGH);

            landscape_next_ts_set = false;
            previous_landscape_time = millis();
      
        }
     
        if (urn5_index==1){
          // analogWrite(landscape_light, 50);

          digitalWrite(landscape_light, LOW);
          digitalWrite(landscape_light_2, HIGH);
        }
     
        if (urn5_index==2){
          digitalWrite(landscape_light, HIGH);
          digitalWrite(landscape_light_2, LOW);
        
      }
    }
  }
}
// }

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

bool is_valid_millis_timestamp(unsigned long timestamp){
  if ((timestamp - 1000 <= now()*1000) && (now()*1000 <= timestamp + 1000)){
    return true;
  }
  return false;
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

void landscape_aleas(byte random_value){
  landscape_light_choice = random_value;
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
      break;
  }
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

