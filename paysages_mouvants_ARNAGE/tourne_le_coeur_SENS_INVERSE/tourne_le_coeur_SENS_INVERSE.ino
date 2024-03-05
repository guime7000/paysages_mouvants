#include <Servo.h>  // on inclut la bibliothèque pour piloter un servomoteur

Servo monServo;  // on crée l'objet monServo

void setup()
{
    monServo.attach(1); // on définit le Pin utilisé par le servomoteur
    pinMode(13, OUTPUT);
}

void loop()
{
  // LE COEUR TOURNE SANS S'arreter....
  // Pour le stopper, débrancher le câble relié au moteur du coeur
    monServo.write(100);

}