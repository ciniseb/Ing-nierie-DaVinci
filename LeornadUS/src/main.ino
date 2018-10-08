/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <LibRobus.h> // Essentielle pour utiliser RobUS



/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces
#define GAUCHE 0
#define DROITE 1
#define speed0 0
#define speed1 0.5


/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
float dPICalc(float distancegauche1, float distancedroite1)
{
  float dErreur = 0;
  int dNbEssais;
  float dIntgral;
  float dPout = 0;
  float dIout = 0;
  float dKp = 0.1;
  float dKi = 0; 
  float dPIout = 0;

  dNbEssais++; // compte le nb d'essai (pour le I)

  // Calcul de l'erreur.
  dErreur = distancegauche1 - distancedroite1;

  // P.
  dPout = dErreur * dKp;
  
  // I.,
  dIntgral = dIntgral+(dErreur * 0.1);
  dIout = dIntgral * dKi;

  // P=20, I=20
  // PI = 40 -> 40 tick de plus a faire.
  // Calcul PI en pulse.
  dPIout = (dPout+dIout)/100;
  Serial.print("PIOUT: ");
  Serial.println(dPIout);
  Serial.println(" ");
  return (dPIout);
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
  //détermine le nombre de pulse pour arriver à l'angle demandé
  angle_pulse = angle_degree_a_pulse(angle);

  
  if(direction == GAUCHE)
  {
    while(ENCODER_Read(DROITE)<=angle_pulse)
    {
        MOTOR_SetSpeed(GAUCHE,speed0);
        MOTOR_SetSpeed(DROITE,speed1);
    }
  
  }
  if(direction == DROITE)
  {
    while(ENCODER_Read(GAUCHE)<=angle_pulse)
    {
        MOTOR_SetSpeed(GAUCHE,speed1);
        MOTOR_SetSpeed(DROITE,speed0);
    }
  }
    MOTOR_SetSpeed(GAUCHE,speed0);
    MOTOR_SetSpeed(DROITE,speed0);
    transition();
  return 0;
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
  float speed2= speed1;
  float accel=0;
  int counter=0;
  distance_pulse = distance_mm_pulse(distance_mm);
  while(ENCODER_Read(GAUCHE)<=distance_pulse)
  {
    if(counter=0)
    {
      for (int i = 0; i < 3; i++) 
      {     
        accel = accel + 0.1;
        MOTOR_SetSpeed(GAUCHE,accel);
        MOTOR_SetSpeed(DROITE,accel);
        delay(25);
      }
      counter=1;
    }
    distgauche1 = ENCODER_Read(GAUCHE);
    distdroite1 = ENCODER_Read(DROITE);
    k=dPICalc(distgauche1,distdroite1);
    speed2 = speed1+k;

        MOTOR_SetSpeed(GAUCHE,speed1);
        MOTOR_SetSpeed(DROITE,speed2);
           delay(100);
  }

  MOTOR_SetSpeed(GAUCHE,speed0);
  MOTOR_SetSpeed(DROITE,speed0);
  transition();
 
}
void reculer(float distance_mm)
{
  float distance_pulse,distgauche1,distdroite1;
  float k;
  float speed2= -speed1;
  distance_pulse = distance_mm_pulse(distance_mm);
  while(ENCODER_Read(GAUCHE)>=-distance_pulse)
  {
    distgauche1 = ENCODER_Read(GAUCHE);
    distdroite1 = ENCODER_Read(DROITE);
    k=dPICalc(distgauche1,distdroite1);
    speed2 = -speed1+k;
        MOTOR_SetSpeed(GAUCHE,-speed1);
        MOTOR_SetSpeed(DROITE,speed2);
           delay(100);
  }

  MOTOR_SetSpeed(GAUCHE,speed0);
  MOTOR_SetSpeed(DROITE,speed0);
  transition();
 
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
        MOTOR_SetSpeed(GAUCHE,speed0);
        MOTOR_SetSpeed(DROITE,-speed1);
    }
  
  }
  if(direction == DROITE)
  {
    while(ENCODER_Read(GAUCHE)>=-angle_pulse)
    {
        MOTOR_SetSpeed(GAUCHE,-speed1);
        MOTOR_SetSpeed(DROITE,speed0);
    }
  }
    MOTOR_SetSpeed(GAUCHE,speed0);
    MOTOR_SetSpeed(DROITE,speed0);
    transition();
  return 0;
}

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibales globales

void setup(){
  BoardInit();
}


/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop()
 {
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  delay(10);// Delais pour décharger le CPU
  if(ROBUS_IsBumper(3))
 {
  //a-b
  avancer(2020);
  tourner(GAUCHE,90);
  //b-c
  avancer(300);

  tourner(DROITE,90);
  //c-d
  avancer(280);

  tourner(DROITE,90);
  //d-e
  avancer(300);

  tourner(GAUCHE,90);
  //e-f
  avancer(180);

  tourner(DROITE,40);
  //f-g
  avancer(300);
  
  tourner(GAUCHE,85);
  //g-h
  avancer(600);

  tourner(DROITE,50);
  //h-i
  avancer(250);

  tourner(DROITE,15);

  avancer(720);
//aller compléter

  delay(300);

  reculer(700);

  tourner_reculer(DROITE,15);

  reculer(275);

  tourner_reculer(DROITE,50);

  reculer(600);

  tourner_reculer(GAUCHE,90);

  reculer(300);
  
  tourner_reculer(DROITE,40);
  
  reculer(130);

  tourner_reculer(GAUCHE,90);
  
  reculer(300);

  tourner_reculer(DROITE,90);

  reculer(270);

  tourner_reculer(DROITE,90);

  reculer(310);

  tourner_reculer(GAUCHE,90);

  reculer(2020);
 }
}