#include <Servo.h>  // on inclut la bibliothèque pour piloter un servomoteur

Servo monServo;  // on crée l'objet monServo

void setup()
{
    monServo.attach(7); // on définit le Pin utilisé par le servomoteur
    pinMode(13, OUTPUT);
}

void loop()
{
    // LE sablier desend vite sans s'arreter....
  // Pour le stopper, débrancher le câble relié au moteur du sablier
    monServo.write(115);
    }