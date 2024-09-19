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
// int urn1_start[5]={0, 3, 7, 20, 35}; // Start time offsets in min since Show is ON.
int urn1_start[5]={1, 3, 7, 20, 35}; // Start time offsets in min since Show is ON.
int urn1_stop[5]={2, 5, 8, 25, 40}; // Stop time offsets in min since Show is ON.
byte urn1_index = 0;

//Urn2 Organ
const byte organ = 1; // Urn 2: the organic organ*
int urn2_start[3]={12, 25, 35}; // Start time offsets in min since Show is ON.
int urn2_stop[3]={18, 28, 40}; // Start time offsets in min since Show is OFF.
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
const byte hourglass_surprise = 7; // Urn 4: The surprising hourglass

int urn4_start[2]={0, 10}; // Start time offsets in min since Show is ON.
int urn4_stop[2]={5, 33}; // Stop time offsets in min since Show is ON.
byte urn4_index = 0;

unsigned long hourglass_pause_duration=8000; // Durée de pause de rotation du moteur du sablier (en ms)
unsigned long hourglass_rotation_duration=250; // Durée de rotation du moteur du sablier (en ms)
byte hourglass_rise_speed = 60; // Réglage de la vitesse de montée du sablier : 0 = très rapide, 90 = arrêté

bool hourglass_rotates=false;

bool hourglass_is_launched = 0;
unsigned long actual_hourglass_time=0;
unsigned long previous_hourglass_time=0;

//Urn 5 Landscape
const int landscape_light[4] = {8, 11, 9, 10}; // Urn 5: Light dimmer ouput for landscape Urn 

// const int low_landscape_light[2] = {11, 10}; // Urn 5: Low ligths ramp up outputs
const int low_landscape_light[2] = {8, 11}; // Urn 5: Low ligths ramp up outputs
const int low_landscape_light_rightoff[2] = {10, 11}; // Urn 5: Low ligths ramp up outputs
int urn5_start[7]={20, 25, 30, 34, 35, 36, 50}; // Start time offsets in min since Show is ON.

bool landscape_blink = true;

unsigned int landscape_delay = 0;
unsigned long elapsed=0;
unsigned long landscape_now=0;
byte landscape_chenillard[5][5]={{9,9,8,10,11},{9,8,10,10,11},{8,10,9,9,11},{10,11,8,8,9},{11,11,8,9,10}};

// [[ON,OFF],[2,0], [0,1], [3,2], [1,0]]
// Durant tests {8,11,9,10,13}
unsigned long landscape_chenillard_delay =0;
unsigned long index_chenillard = 0;
byte sequence_chenillard=0;



//*************************************************************************************

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
for (int jj=0; jj<sizeof(landscape_light)/sizeof(int);jj++){
    pinMode(landscape_light[jj],OUTPUT);
    analogWrite(landscape_light[jj],0);
    delay(10);
  }

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
// // Serial.println(hourglass_push_button_state);
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

if (is_valid_index(urn4_index, ARRAY_SIZE(urn4_start))){
  
  if (is_valid_timestamp(start_time + urn4_start[urn4_index]*time_factor)){
    hourglass_rotates = true;
    hourglass_is_launched = 1;
    }

  if (is_valid_timestamp(start_time + urn4_stop[urn4_index]*time_factor)){
    hourglass_rotates = false;
    urn4_index += 1;
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
}

if ((start_time+34*time_factor<= now()) && (now() < start_time+35*time_factor)){
  hourglass_servo.write(78); 
}
if (start_time+2*time_factor<= now()){
  hourglass_servo.write(90); 
}


// Urn 4 Montée de la SURPRISE

if ((start_time+35*time_factor<= now()) && (now() < start_time+36*time_factor)){
  surprise_servo.write(108); // testé à 108, Peut être mettre 109 si ca monte pas assez vite
}
else{
  surprise_servo.write(90);

}

//Urn 5
landscape_now = millis();
// int urn5_start[7]={-1, -1, 1, 2, 3, 4, 5}; // Start time offsets in min since Show is ON.
// fond droite :8 [0]
// dev droite: 9 [2]

// fond gauche: 11 [1]
//devant_gauche: 10 [3]
// int urn5_start[7]={20, 25, 30, 34, 35, 36, 40}; // Start time offsets in min since Show is ON.

if ((start_time+urn5_start[0]*time_factor<= now()) && (now() < start_time+urn5_start[1]*time_factor)) {// Fete Foraine
  if (landscape_now - elapsed >= landscape_delay ){ 
    analogWrite(landscape_light[random(0,sizeof(landscape_light)/sizeof(int))],random(32,64));
    landscape_delay=random(100,300);
    elapsed = millis();
  }
}

if ((start_time+urn5_start[1]*time_factor<= now()) && (now() < start_time+urn5_start[2]*time_factor)) {// Faible mouvement (2 lampes)
  if (landscape_now - elapsed >= landscape_delay ){ 
    analogWrite(landscape_light[2], 0);
    analogWrite(landscape_light[3], 0);
    analogWrite(low_landscape_light[random(0,2)],random(32,64));
    landscape_delay=random(200,500);
    elapsed = millis();
  }
}

if ((start_time+urn5_start[2]*time_factor<= now()) && (now() < start_time+urn5_start[3]*time_factor)) {// Montée (4 lampes) + Vivre
  if (landscape_now - elapsed >= landscape_delay ){ 
    analogWrite(landscape_light[random(0,sizeof(landscape_light)/sizeof(int))],random(64,128));
    landscape_delay = random(200,500);
    elapsed = millis();
  }
}

if ((start_time+urn5_start[3]*time_factor<= now()) && (now() < start_time+urn5_start[4]*time_factor)) {// Virer lum droite 1 min puis UNE sec de blackout
  if (landscape_now - elapsed >= landscape_delay ){ 
    // analogWrite(landscape_light[1], 0);
    analogWrite(landscape_light[0], 0);
    analogWrite(landscape_light[2], 0);
    analogWrite(low_landscape_light_rightoff[random(0,2)],random(64,128));
    landscape_delay=random(100,300);
    elapsed = millis();
  }
}

if ((start_time+urn5_start[4]*time_factor <= now()) && (now() < start_time+urn5_start[4]*time_factor+1)) {// 1 sec BLACKOUT
  for (int jj=0; jj<sizeof(landscape_light)/sizeof(int);jj++){
    analogWrite(landscape_light[jj],0);
  }
}

if ((start_time+urn5_start[4]*time_factor+1 <= now()) && (now() < start_time+urn5_start[5]*time_factor)) {// ON / OFF / ON / OFF (4 * 15s)
  if (landscape_now - elapsed >= landscape_delay ){ 
    if (landscape_blink){
      for (int jj=0; jj<sizeof(landscape_light)/sizeof(int);jj++){
      analogWrite(landscape_light[jj],128);
      }
    elapsed = millis();
    }
    else {
      for (int jj=0; jj<sizeof(landscape_light)/sizeof(int);jj++){
      analogWrite(landscape_light[jj],0);
      }
    elapsed = millis();
    }
  landscape_delay = random(8000,15000);
  landscape_blink = !landscape_blink;
  }
}
if ((start_time+urn5_start[5]*time_factor <= now()) && (now() < start_time+urn5_start[6]*time_factor)) {// CHENILLARD
    if (landscape_now - elapsed >= landscape_chenillard_delay ){
    sequence_chenillard = index_chenillard % 6;
    if (sequence_chenillard == 0){
      for (int jj=0; jj<sizeof(landscape_light)/sizeof(int);jj++){
        analogWrite(landscape_light[jj],0);
      }
    elapsed = millis();
    index_chenillard += 1;
    landscape_chenillard_delay = random(100,250);

    }
    else{
    analogWrite(landscape_chenillard[sequence_chenillard][0],128);
    analogWrite(landscape_chenillard[sequence_chenillard][1],128);

    for (int jj=2; jj <=4; jj++){
      analogWrite(landscape_chenillard[sequence_chenillard][jj],0);
    }
    elapsed = millis();
    index_chenillard += 1;
    landscape_chenillard_delay = random(100,500);

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

