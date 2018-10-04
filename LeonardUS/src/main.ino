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
double dPICalc(double dDistanceMoMa, double dDistanceMoSl)
{
  double dErreur = 0;
  static double dNbEssais = 0;
  static double dIntgral = 0;
  double dPout = 0;
  double dIout = 0;
  double dKp = 2; // 0.3 cm/unit -> 30 mm/unit. ???
  double dKi = 2; // 
  double dPIout = 0;

  dNbEssais++; // compte le nb d'essai (pour le I)

  // Calcul de l'erreur.
  dErreur = dDistanceMoMa - dDistanceMoSl;

  // P.
  dPout = dErreur / dKp;
  
  // I.,
  dIntgral = (dErreur * dNbEssais) + dIntgral;
  dIout = dIntgral / dKi;

  // P=20, I=20
  // PI = 40 -> 40 tick de plus a faire.
  // convertion a 600 mm de plus à faire.

  // Calcul PI en pulse.
  dPIout = dKp + dKi;

  return dPIout;
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
  delay(100);
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
  int angle_pulse;
  int angle_pulse_1;
  float accel;
  //détermine le nombre de pulse pour arriver à l'angle demandé
  angle_pulse = angle_degree_a_pulse(angle);

  
  if(direction == GAUCHE)
  {
    angle_pulse_1=ENCODER_Read(DROITE)+3936.85;
    while(ENCODER_Read(DROITE)<=angle_pulse)
    {
        MOTOR_SetSpeed(GAUCHE,speed0);
        MOTOR_SetSpeed(DROITE,speed1);
    }
  
  }
  if(direction == DROITE)
  {
    angle_pulse_1=ENCODER_Read(GAUCHE)+angle_pulse;
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
  float distance_pulse;
  float k;
  distance_pulse = distance_mm_pulse(distance_mm);
  while(ENCODER_Read(GAUCHE)<=distance_pulse)
  {
    distgauche = ENCODER_Read(GAUCHE);
    distdroite = ENCODER_Read(DROITE);
    K=dPICalc(distgauche,distdroite);
    speed2 = speed2*k;
        MOTOR_SetSpeed(GAUCHE,speed1);
        MOTOR_SetSpeed(DROITE,speed1);
  }

  MOTOR_SetSpeed(GAUCHE,speed0);
  MOTOR_SetSpeed(DROITE,speed0);
  transition();
 
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
  avancer(2000);

  tourner(GAUCHE,90);

  avancer(400);

  tourner(DROITE,90);

  avancer(350);

  tourner(DROITE,90);
 
  avancer(400);

  tourner(GAUCHE,90);

  avancer(180);

  tourner(DROITE,45);
 
  avancer(340);
  
  tourner(GAUCHE,90);

  avancer(750);

  tourner(DROITE,45);
 
  avancer(300);

  tourner(DROITE,15);

  avancer(750);
}