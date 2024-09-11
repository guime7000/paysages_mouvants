

const byte check_button=23; // Pin for check sequence push button
bool check_button_state = 0;
const byte check_green_led=10; // Green Led for checks
const byte check_red_led=11; // Red Led for checks



void setup() {

  Serial.begin(57600);

  pinMode(check_button, INPUT_PULLUP);
  pinMode(13, OUTPUT);

}

void loop(){

  check_button_state = digitalRead(check_button);
  while (!check_button_state){
    check_button_state = digitalRead(check_button);
    Serial.println(check_button_state);
    analogWrite(13, 0);
    }
    Serial.println(check_button_state);
    
    analogWrite(13, 255);

  
}

