#include <Servo.h>  // on inclut la bibliothèque pour piloter un servomoteur

Servo monServo;  // on crée l'objet monServo

void setup()
{
    monServo.attach(6); // on définit le Pin utilisé par le servomoteur
    pinMode(13, OUTPUT);
}

void loop()
{
    // LE sablier monte sans s'arreter....
  // Pour le stopper, débrancher le câble relié au moteur du sablier
    monServo.write(0);
    delay(80);
    monServo.write(90);
    delay(3000);

}