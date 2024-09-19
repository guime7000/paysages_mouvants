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
int urn4_stop[2]={5, 35}; // Stop time offsets in min since Show is ON.
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
// int urn5_start[7]={20, 25, 30, 34, 35, 36, 50}; // Start time offsets in min since Show is ON.
int urn5_start[7]={0, 0, 1, 2, 3, 4, 5}; // Start time offsets in min since Show is ON.

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



//Urn 5
// const int landscape_light[4] = {8, 11, 9, 10}; // Urn 5: Light dimmer ouput for landscape Urn 

    analogWrite(landscape_light[3],128);
}