#include <Servo.h>
#include <TimeLib.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

Servo organ_servo;  // Organ urn servo object
Servo hourglass_servo; // Hourglass servo object
Servo surprise_servo; // Surprise servo object in Hourglass urn


//Urn1 Big Fan
const byte one_fan = 0; // Urn 1 : One BIG fan
// int urn1_start[5]={0, 3, 7, 20, 35}; // Start time offsets in min since Show is ON.
//Urne 1: top/stop en minute : (0,2), (3,5), (7,8), (20,25), (35,40)
int urn1_start[4]={1, 180, 420, 1320}; // Start time offsets in min since Show is ON.
int urn1_stop[4]={120, 300, 480, 1500}; // Stop time offsets in min since Show is ON.
byte urn1_index = 0;


//Urn2 Organ
const byte organ = 1; // Urn 2: the organic organ*
//Urne 2: top/stop en minute : (12,18), (25,28), (35,40)
int urn2_start[3]={780, 1500, 2100}; // Start time offsets in min since Show is ON.
int urn2_stop[3]={1320, 1680, 2400}; // Start time offsets in min since Show is OFF.
byte urn2_index = 0;
bool organ_turn_flag=false;
byte rotation_direction=0;

//Urn 3 Pampa
const byte pampa1 = 2; // Urn 3 : The Pampa, fan 1
const byte pampa2 = 3; // Urn 3 : The Pampa, fan 2
const byte pampa3 = 4; // Urn 3 : The Pampa, fan 3
const byte pampa4 = 5; // Urn 3 : The Pampa, fan 4
//Urne 3: top/stop en minute : (5,10), (15,20), (25,30), (35,40)
int urn3_start[4]={300, 900, 1500, 2100}; // Start time offsets in min since Show is ON.
int urn3_stop[4]={600, 1080, 1800, 2400}; // Start time offsets in min since Show is ON.
byte urn3_index = 0;

bool pampa_turn_flag=false;
byte sub_pampa_timestamp_index=0;

//Urn4 Hourglass
const byte hourglass_tall = 6; // Urn 4 : Tall and long hourglass
const byte hourglass_surprise = 7; // Urn 4: The surprising hourglass

//Urne 4: top/stop en minute : (0,5), (10,33)
int urn4_start[2]={1, 600}; // Start time offsets in min since Show is ON.
int urn4_stop[2]={300, 1980}; // Stop time offsets in min since Show is ON.
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

const int low_landscape_light[2] = {8, 11}; // Urn 5: Low ligths ramp up outputs
const int low_landscape_light_rightoff[2] = {10, 11}; // Urn 5: Low ligths ramp up outputs
//Urne 5: top en minute : (20,25,30,34,35,36,50)
int urn5_start[6]={1320, 1500, 1800, 2040, 2160, 3000}; // Start time offsets in min since Show is ON.

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
unsigned long start_time = now() + block_offset_time;

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
for (int jj=0; jj<sizeof(landscape_light)/sizeof(int);jj++){
    pinMode(landscape_light[jj],OUTPUT);
    analogWrite(landscape_light[jj],0);
    delay(10);
  }

  start_time = now() + block_offset_time;
}

void loop() {


//Urn 1 BIG Fan
  if (is_valid_index(urn1_index, ARRAY_SIZE(urn1_start))){
    if (is_valid_timestamp(start_time + urn1_start[urn1_index])){
      digitalWrite(one_fan, HIGH);
    }
    if (is_valid_timestamp(start_time + urn1_stop[urn1_index])){
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

    if (is_valid_timestamp(start_time + urn2_start[urn2_index])){
      organ_turn_flag = true;
      rotation_direction = random(0,2);
    }

    if (is_valid_timestamp(start_time + urn2_stop[urn2_index])){
      organ_turn_flag = false;
      rotation_direction = random(0,2);
      urn2_index +=1;
    }
  }

//Urn 3 Pampa
if (is_valid_index(urn3_index, ARRAY_SIZE(urn3_start))){

    if (is_valid_timestamp(start_time + urn3_start[urn3_index]+sub_pampa_timestamp_index*60) && pampa_turn_flag){
      pampas_fans_on();
      sub_pampa_timestamp_index +=1;
    }

    if (is_valid_timestamp(start_time + urn3_start[urn3_index])){
      pampa_turn_flag= true;;
    }

    if (is_valid_timestamp(start_time + urn3_stop[urn3_index])){
      pampas_fans_off();
      pampa_turn_flag = false;
      sub_pampa_timestamp_index = 0;
      urn3_index +=1;
    }
  }

// Urn 4 Hourglass

if (is_valid_index(urn4_index, ARRAY_SIZE(urn4_start))){
  
  if (is_valid_timestamp(start_time + urn4_start[urn4_index])){
    hourglass_rotates = true;
    hourglass_is_launched = 1;
    }

  if (is_valid_timestamp(start_time + urn4_stop[urn4_index])){
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

if ((start_time+2040<= now()) && (now() < start_time+2090)){
  hourglass_servo.write(78); 
}
if (start_time+2090<= now()){
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

if ((start_time+urn5_start[0]<= now()) && (now() < start_time+urn5_start[1])) {// Fete Foraine
  if (landscape_now - elapsed >= landscape_delay ){ 
    analogWrite(landscape_light[random(0,sizeof(landscape_light)/sizeof(int))],random(32,64));
    landscape_delay=random(100,300);
    elapsed = millis();
  }
}

if ((start_time+urn5_start[1]<= now()) && (now() < start_time+urn5_start[2])) {// Faible mouvement (2 lampes)
  if (landscape_now - elapsed >= landscape_delay ){ 
    analogWrite(landscape_light[2], 0);
    analogWrite(landscape_light[3], 0);
    analogWrite(low_landscape_light[random(0,2)],random(32,64));
    landscape_delay=random(200,500);
    elapsed = millis();
  }
}

if ((start_time+urn5_start[2]<= now()) && (now() < start_time+urn5_start[3])) {// Montée (4 lampes) + Vivre
  if (landscape_now - elapsed >= landscape_delay ){ 
    analogWrite(landscape_light[random(0,sizeof(landscape_light)/sizeof(int))],random(64,128));
    landscape_delay = random(200,500);
    elapsed = millis();
  }
}

if ((start_time+urn5_start[3]<= now()) && (now() < start_time+urn5_start[4])) {// Virer lum droite 1 min puis UNE sec de blackout
  if (landscape_now - elapsed >= landscape_delay ){ 
    analogWrite(landscape_light[0], 0);
    analogWrite(landscape_light[2], 0);
    analogWrite(low_landscape_light_rightoff[random(0,2)],random(64,128));
    landscape_delay=random(100,300);
    elapsed = millis();
  }
}

if ((start_time+urn5_start[4] <= now()) && (now() < start_time+urn5_start[4]+1)) {// 1 sec BLACKOUT
  for (int jj=0; jj<sizeof(landscape_light)/sizeof(int);jj++){
    analogWrite(landscape_light[jj],0);
  }
}

if ((start_time+urn5_start[4]+1 <= now()) && (now() < start_time+urn5_start[5])) {// CHENILLARD
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

