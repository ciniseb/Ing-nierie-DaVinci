/*============================================
Projet: Code source
Equipe: P14
Auteurs: Simon St-Onge, Philippe B-L, Éric Leduc, Sébastien St-Denis
Description: Parcours de LéonardUS
Date: 24 octobre 2018
============================================*/
#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <ADJDS311.h>

//Variables globales et defines
#define GAUCHE 0
#define DROITE 1
#define speed0 0
#define speed1 0.8
#define speed2 0.25
#define speed3 0.25
#define speed4 0.35
#define SEUILDEBUT 410
#define DISTNOIRE 150
//Fonctions
float dPICalc(float distancegauche1, float distancedroite1)
{
  float dErreur = 0;
  int dNbEssais;
  float dIntgral;
  float dPout = 0;
  float dIout = 0;
  float dKp = 0.03;
  float dKi = 0; 
  float dPIout = 0;

  //Calcul de l'erreur
  dErreur = distancegauche1 - distancedroite1;

  //P
  dPout = dErreur * dKp;
  
  //I
  dIntgral = dIntgral+(dErreur * 0.1);
  dIout = dIntgral * dKi;

  //P=20, I=20
  //PI=40 -> 40 tick de plus a faire
  //Calcul PI en pulse
  dPIout = (dPout+dIout)/100;
  Serial.print("PIOUT: ");
  Serial.println(dPIout);
  Serial.println(" ");
  return (dPIout);
}
void accel_avancer()
{
  float accel=0;
      while(accel<=0.35)
      { 
        vprobeFreq();    
        accel = accel + 0.01;
        MOTOR_SetSpeed(GAUCHE,accel);
        MOTOR_SetSpeed(DROITE,accel);
        delay(35);
      }
}
void accel_reculer()
{
  float accel=0;
      while(accel<=0.35)
      {  
        vprobeFreq();   
        accel = accel + 0.01;
        MOTOR_SetSpeed(GAUCHE,-accel);
        MOTOR_SetSpeed(DROITE,-accel);
        delay(35);
      }
}

int reset_encodeur()
{
  ENCODER_Reset(GAUCHE);
  ENCODER_Reset(DROITE);
  return 0;
}

int transition()
{
  
  reset_encodeur();
  delay(150);
  return 0;
}

float angle_degree_a_pulse(float angle)
{
  // déterminer la circonference d'une roue en mm et en pulse
  float diametre_roue_mm = 75;
  float circonference_roue_mm = 3.1416*diametre_roue_mm;
  float circonference_roue_pulse = 3200;
  // déterminer la circonference des 2 roues en mm et en pulse
  float diametre_2_roues_mm  = 187;
  float quart_circonference_2_roues_mm = (3.1416*2*diametre_2_roues_mm)/4;
  float quart_circonference_2_roues_pulse = (quart_circonference_2_roues_mm/circonference_roue_mm)*circonference_roue_pulse;
  // déterminer le rapport pulses par degré
  float pulses_par_degre_2_roues = quart_circonference_2_roues_pulse/90; 
  // déterminer le nombre de pulses requis pour arriver à l'angle demandé
  float pulses_pour_angle_x;

  pulses_pour_angle_x = angle * pulses_par_degre_2_roues;

  return  pulses_pour_angle_x;
}

int tourner(int direction, float angle)
{
  float angle_pulse;
  float encodeur_gauche = ENCODER_Read(GAUCHE);
  float encodeur_droite = ENCODER_Read(DROITE);
 
  //détermine le nombre de pulse pour arriver à l'angle demandé
  angle_pulse = angle_degree_a_pulse(angle);

  if(direction == GAUCHE)
  {
    
    while(ENCODER_Read(DROITE)<=angle_pulse+encodeur_droite)
    {
        vprobeFreq();
        MOTOR_SetSpeed(GAUCHE,speed0);
        MOTOR_SetSpeed(DROITE,speed4);
    }
  }
  if(direction == DROITE)
  {
    while(ENCODER_Read(GAUCHE)<=angle_pulse+encodeur_gauche)
    {
      vprobeFreq();
      MOTOR_SetSpeed(GAUCHE,speed4);
      MOTOR_SetSpeed(DROITE,speed0);
    }
  }
    MOTOR_SetSpeed(GAUCHE,speed0);
    MOTOR_SetSpeed(DROITE,speed0);
    transition();
  return 0;
}
void tournerCentre(int direction, float angle)
{
  float anglePulse = angle_degree_a_pulse(angle);//Variable en pulse selon l'angle
  float encodeur_gauche = ENCODER_Read(GAUCHE);
  float encodeur_droite = ENCODER_Read(DROITE);
  
  if(direction == GAUCHE)
  {
    while(ENCODER_Read(DROITE) <= anglePulse/2+encodeur_droite)
    {
        vprobeFreq();
        MOTOR_SetSpeed(GAUCHE,-speed2);
        MOTOR_SetSpeed(DROITE,speed2);
    }
  }
  else if(direction == DROITE)
  {
    while(ENCODER_Read(GAUCHE) <= anglePulse/2+encodeur_gauche)
    {
        vprobeFreq();
        MOTOR_SetSpeed(GAUCHE,speed2);
        MOTOR_SetSpeed(DROITE,-speed2);
    }
  }
  MOTOR_SetSpeed(GAUCHE,speed0);
  MOTOR_SetSpeed(DROITE,speed0);
}
void danse(float angle)
{
  float anglePulse = angle_degree_a_pulse(angle);//Variable en pulse selon l'angle
  for(float dVitesse = 0.4f; dVitesse > -0.4f ; dVitesse = dVitesse-0.05f)
  {
    MOTOR_SetSpeed(GAUCHE, 0.4f);
    MOTOR_SetSpeed(DROITE, dVitesse);
  }
  MOTOR_SetSpeed(GAUCHE, speed0);
  MOTOR_SetSpeed(DROITE, speed0);
  transition();
}

float distance_mm_pulse(float distance_mm)
{
   // déterminer la circonference d'une roue en mm et en pulse
  float diametre_roue_mm = 75;
  float circonference_roue_mm = 3.1416*diametre_roue_mm;
  float circonference_roue_pulse = 3200;
  float distance_pulse = (distance_mm/circonference_roue_mm)*circonference_roue_pulse;
  return distance_pulse;
}
void avancer(float distance_mm)
{
  float distance_pulse,distgauche1,distdroite1;
  float k;
  float speed= speed1;
  int counter=0;
  float encodeur_gauche = ENCODER_Read(GAUCHE);
 
  distance_pulse = distance_mm_pulse(distance_mm);

  while(ENCODER_Read(GAUCHE)<=distance_pulse+encodeur_gauche)
  {
    //accel
    if(counter==0)
    {
       vprobeFreq();
      accel_avancer();
      counter=1;
    }
    //deccel
    if(ENCODER_Read(GAUCHE)>=(distance_pulse/1.3)+encodeur_gauche)
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k=dPICalc(distgauche1,distdroite1);
      speed = speed3+k;
      vprobeFreq();
      MOTOR_SetSpeed(GAUCHE,speed3);
      MOTOR_SetSpeed(DROITE,speed);
      delay(100);
      counter=2;
    }
    //vitesse intermédiaire
    if(counter==1)
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k=dPICalc(distgauche1,distdroite1);
      speed = speed1+k;
      vprobeFreq();
      MOTOR_SetSpeed(GAUCHE,speed1);
      MOTOR_SetSpeed(DROITE,speed);
      delay(100);
    }

  }
  
  MOTOR_SetSpeed(GAUCHE,speed0);
  MOTOR_SetSpeed(DROITE,speed0);
 
}

void avancer_def(float distance_mm)
{
   float distance_pulse;
   float encodeur_gauche = ENCODER_Read(GAUCHE);
 
  distance_pulse = distance_mm_pulse(distance_mm);

  while(ENCODER_Read(GAUCHE)<=distance_pulse+encodeur_gauche)
  {
    if(ROBUS_ReadIR(3)>100)
    {
      tournerCentre(DROITE,90);
      avancer(50);
      reculer(50);
      tournerCentre(GAUCHE,90);
      break;
    }
    vprobeFreq();
    MOTOR_SetSpeed(GAUCHE,speed3);
    MOTOR_SetSpeed(DROITE,speed3);
    
  }
}
void reculer_def(float distance_mm)
{
   float distance_pulse;
   float encodeur_gauche = ENCODER_Read(GAUCHE);
 
  distance_pulse = distance_mm_pulse(distance_mm);

  while(ENCODER_Read(GAUCHE)>=-distance_pulse-encodeur_gauche)
  {
    if(ROBUS_ReadIR(3)>100)
    {
      vprobeFreq();
      tournerCentre(DROITE,90);
      avancer(50);
      vprobeFreq();
      reculer(50);
      tournerCentre(GAUCHE,90);
      break;
    }
    vprobeFreq();
    MOTOR_SetSpeed(GAUCHE,-speed3);
    MOTOR_SetSpeed(DROITE,-speed3);
    
  }
}
void reculer(float distance_mm)
{
  int counter=0;
  float distance_pulse,distgauche1,distdroite1;
  float k;
  float accel;
  float speed= -speed1;
  float encodeur_gauche = ENCODER_Read(GAUCHE);
  
  distance_pulse = distance_mm_pulse(distance_mm);
  while(ENCODER_Read(GAUCHE)>=-distance_pulse-encodeur_gauche)
  {
      vprobeFreq();
      MOTOR_SetSpeed(GAUCHE,-speed3);
      MOTOR_SetSpeed(DROITE,-speed3);
      
  }
  MOTOR_SetSpeed(GAUCHE,speed0);
  MOTOR_SetSpeed(DROITE,speed0);
 
}
int tourner_reculer(int direction, float angle)
{
  float angle_pulse;
  //détermine le nombre de pulse pour arriver à l'angle demandé
  angle_pulse = angle_degree_a_pulse(angle);

  if(direction == GAUCHE)
  {
    while(ENCODER_Read(DROITE)>=-angle_pulse)
    {
        vprobeFreq();
        MOTOR_SetSpeed(GAUCHE,speed0);
        MOTOR_SetSpeed(DROITE,-speed4);
    }
  
  }
  if(direction == DROITE)
  {
    while(ENCODER_Read(GAUCHE)>=-angle_pulse)
    {
        vprobeFreq();
        MOTOR_SetSpeed(GAUCHE,-speed4);
        MOTOR_SetSpeed(DROITE,speed0);
    }
  }
    MOTOR_SetSpeed(GAUCHE,speed0);
    MOTOR_SetSpeed(DROITE,speed0);
    transition();
  return 0;
}
void vprobeFreq(void)
{
  // Debug:
  Serial.print("Ambient: ");
  Serial.println(analogRead(A4));

  Serial.print("5kHz: ");
  Serial.println(analogRead(A5));

  Serial.println(" ");
  // fin debug.

  if(analogRead(A4) < analogRead(A5) && analogRead(A5) >= SEUILDEBUT)
  {
    delay(100);
     if(analogRead(A4) < analogRead(A5) && analogRead(A5) >= SEUILDEBUT)
    {
      MOTOR_SetSpeed(GAUCHE,0);
      MOTOR_SetSpeed(DROITE,0);
      delay(5000);
      Serial.print("Delai 5s");
    // Activer le programme ici.
    }
  }
}
void vzoneNoire()
{
  int icaptgauche = analogRead(A6);
  int icaptmilieu = analogRead(A7);
  int icaptdroit = analogRead(A8);

  // Milieux:
  if(icaptmilieu < DISTNOIRE && icaptgauche < DISTNOIRE)
  {
    tournerCentre(GAUCHE,90);
    while(icaptgauche < DISTNOIRE)
    {
    avancer(25);
    }
  }

  // GAUCHE:
  if((icaptgauche < DISTNOIRE) || (icaptgauche < DISTNOIRE && icaptmilieu < DISTNOIRE))
  {
    tournerCentre(DROITE,90);
     while(icaptdroit < DISTNOIRE)
    {
    avancer(25);
    }
  }

  // DROIT:
  if((icaptdroit < DISTNOIRE) || (icaptdroit < DISTNOIRE && icaptmilieu < DISTNOIRE))
  {
    tournerCentre(GAUCHE,90);
     while(icaptdroit < DISTNOIRE)
    {
    avancer(25);
    }
  }
  //GAUCHE-MILIEU-DROITE
  if(icaptgauche < DISTNOIRE && icaptmilieu < DISTNOIRE && icaptdroit < DISTNOIRE)
  {
    tournerCentre(GAUCHE,180);
    avancer(1500);
  }
}
  


uint8_t ledPin = 38;
ADJDS311 color(ledPin);

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibales globales
void setup(){
  BoardInit();
   //Serial.begin(9600);
    //color.init();
    //color.ledOn();
    // ::init() preset values in the registers.
    // The S311 and S371 have different gains.
    // Here you can use ::calibrate() to adjust the sensor's gains.
    // You can also use ::setintegrationtime() and ::setcapacitor() to adjust the sensor manually

    //color.calibrate();  // first make sure the sensor faces a white surface at the focal point
    pinMode(A4,INPUT);
    pinMode(A5,INPUT);
    pinMode(A6,INPUT);
    pinMode(A7,INPUT);
    pinMode(A8,INPUT);
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"
void loop()
 {
  /* SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  delay(10);// Delais pour décharger le CPU
  color.ledOn();
  Serial.print("R: "); Serial.print(color.readRed());Serial.print(", ");
  Serial.print("G: "); Serial.print(color.readGreen());Serial.print(", ");
  Serial.print("B: "); Serial.print(color.readBlue());Serial.print(", ");
  Serial.print("C: "); Serial.print(color.readClear());
  Serial.println();
  color.ledOff();
  delay(250);
*/
avancer_def(50);
delay(100);
reculer_def(50);
delay(100);
 }
