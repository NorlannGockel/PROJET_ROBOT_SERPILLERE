#include <Servo.h>

Servo myservo;  // création de l'objet myservo 
 
#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

#define LED_PIN 2

int mapX;
int ReceivedMessage[1] = {};
int mapY;
RF24 radio(9,10); // NRF24L01 SPI pins. Pin 9 and 10 on the Nano

const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 
// Définition des numéros de port
const int trigPin = 13;  // Trigger (emission)
const int echoPin = 12;  // Echo    (réception)
int pinMoteurD1 = 5; //on initialise nos pins
int pinMoteurD2 = 2;
int pinMoteurDPWM = 3;
int pinMoteurG1 = 4;
int pinMoteurG2 = 7;
int pinMoteurGPWM =6;
int pompe=8;
int i;
long duree;   // durée de l'echo
int distance; // distance


void setup()
{
    radio.begin(); // Start the NRF24L01
      
  radio.openReadingPipe(1,pipe); // Get NRF24L01 ready to receive
  
  radio.startListening(); // Listen to see if information received
  myservo.attach(11);
  pinMode(pinMoteurD1,OUTPUT);
  pinMode(pinMoteurD2,OUTPUT);
  pinMode(pinMoteurDPWM,OUTPUT); 
  pinMode(pompe, OUTPUT);
  pinMode(pinMoteurG1,OUTPUT);
  pinMode(pinMoteurG2,OUTPUT);
  pinMode(pinMoteurGPWM,OUTPUT);
    digitalWrite(pinMoteurG1,LOW);
  digitalWrite(pinMoteurG2,HIGH);
    digitalWrite(pinMoteurD1,HIGH);
  digitalWrite(pinMoteurD2,LOW);
  pinMode(trigPin, OUTPUT); // Configuration du port du Trigger comme une SORTIE
pinMode(echoPin, INPUT);  // Configuration du port de l'Echo  comme une ENTREE
Serial.begin(9600); // Démarrage de la communication série
myservo.write(90);
  
  
}
void marche_arriere()
{
      digitalWrite(pinMoteurG1,HIGH);
  digitalWrite(pinMoteurG2,LOW);
    digitalWrite(pinMoteurD1,LOW);
  digitalWrite(pinMoteurD2,HIGH);
}
void marche_avant()
{
        digitalWrite(pinMoteurG1,LOW);
  digitalWrite(pinMoteurG2,HIGH);
    digitalWrite(pinMoteurD1,HIGH);
  digitalWrite(pinMoteurD2,LOW);
}
void marche_mot()
{
  analogWrite(pinMoteurGPWM, 200); //Moteur1
  analogWrite(pinMoteurDPWM, 200); //Moteur1
  
}
void calcul_mesures() {

  // Émission d'un signal de durée 10 microsecondes
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Écoute de l'écho 
  duree = pulseIn(echoPin, HIGH);

  // Calcul de la distance
  distance = duree*0.034/2;

 
 Serial.print(distance);
 Serial.println("cm");
} 



void reception_radio()
{
        int angleV = 0;
      radio.read(&angleV, sizeof(angleV));
      int y=angleV;
      Serial.print("Valeur de Y:   ");
      Serial.print(y);
      Serial.print("   ");
      delay(100);
    
      radio.read(&angleV, sizeof(angleV));
      int x=angleV;
      Serial.print(" Valeur de X ");
      Serial.println(x);
      delay(10);
}
void marche_servo()
{
  myservo.write(0);
  delay(2000);
    myservo.write(90);
  delay(2000);
  myservo.write(180);
  delay(2000);

  delay(2000);
}
void loop(){
    int x,y;
      analogWrite(pinMoteurGPWM, 0); //Moteur1
  analogWrite(pinMoteurDPWM, 0); //Moteur1
 
  calcul_mesures();
    marche_avant();;
    analogWrite(pinMoteurGPWM, 250); //Moteur1
    analogWrite(pinMoteurDPWM, 250); //Moteur1
    delay(10);
    calcul_mesures();
    
    delay(10);

    
    
  
  if (distance<20 && distance >10)
  {
    analogWrite(pinMoteurGPWM, 0); //Moteur1
    analogWrite(pinMoteurDPWM, 0); //Moteur1
    myservo.write(0);
    delay(1000);
    calcul_mesures();
    int distance_droite=distance;
    myservo.write(180);
    delay(750);
    calcul_mesures();
    int distance_gauche=distance;
    myservo.write(90);
    Serial.print("Distance a droite :");
    Serial.print(distance_droite);
    Serial.print("     Distance a gauche :");
    Serial.println(distance_gauche);
    delay(750);
    
    if (distance_droite>distance_gauche)
    {
    marche_arriere();
    analogWrite(pinMoteurGPWM, 250); //Moteur1
    analogWrite(pinMoteurDPWM, 250); //Moteur1
    delay(1000);
    marche_avant();
    analogWrite(pinMoteurGPWM, 250); //Moteur1
    analogWrite(pinMoteurDPWM, 0); //Moteur1
    delay(1250);//les distances sont a revoir
    }
    if (distance_gauche>distance_droite)
    {
      marche_arriere();
      analogWrite(pinMoteurGPWM, 250); //Moteur1
      analogWrite(pinMoteurDPWM, 250); //Moteur1
      delay(1000);
      marche_avant();
      analogWrite(pinMoteurGPWM, 0); //Moteur1
      analogWrite(pinMoteurDPWM, 250); //Moteur1
      delay(1250);//les distances sont a revoir
    }
    if (distance_gauche==distance_droite)
    {
      
    marche_arriere();
    analogWrite(pinMoteurGPWM, 250); //Moteur1
    analogWrite(pinMoteurDPWM, 250); //Moteur1
    delay(1000);
    marche_avant();
    analogWrite(pinMoteurGPWM, 250); //Moteur1
    analogWrite(pinMoteurDPWM, 0); //Moteur1
    delay(1250);//les distances sont a revoir
    } 
  }
 
delay(100);
}

  
  
