/*================================================================================================
Fichier:            main.ino
  Projet:             Résolution de problème et conception en génie

  Equipe:             Ingénierie Da Vinci / P14
  Auteurs:            
                      Simon St-Onge (Fonctions de mouvements)
                      Philippe B-L (Normes)
                      Sébastien St-Denis (Fonctions de mouvements, normes & formes)
                      Éric Leduc (???)

  Date:               04-10-2018
  Révision:           
  Compilateur:        Arduino IDE
  Description:        Code source du programme des robots LéonardUS.
                      Robots artistes, un robot contrôlé et un robot automatisé pour dessiner.

  Modification:      10-11-2018
=================================================================================================*/
#include <LibRobus.h> //Librairie de la platforme Robus (Robot)
/*===========================================================================
Defines globales & robots
===========================================================================*/
//DÉCOMMENTEZ le #define du robot que vous voulez utiliser SEULEMENT
#define ROBOTAUTONOME
//#define ROBOTMANUEL

//Si 2 robots définis, dé-defini les deux codes
#ifdef ROBOTAUTONOME
  #ifdef ROBOTMANUEL
    #undef ROBOTAUTONOME
    #undef ROBOTMANUEL
  #endif
#endif

#define GAUCHE 0
#define DROITE 1
#define DEVANT 1
#define DERRIERE -1

#define SOURIRE 0
#define TRISTE 1
#define BLAZE 2
//AUTRE ÉMOTIONS

#define speed0 0
#define speed1 0.4
#define speed2 0.25
#define speed3 0.3
#define speed4 0.35
/*===========================================================================
Variables globales
===========================================================================*/
float vitesse;
/*===========================================================================
Appel des fonctions
===========================================================================*/
float PICalcul(float distanceGauche, float distanceDroite);
float distance_mm_pulse(float distance_mm);
void acceleration(float v, float vMax, float distance);
void MOTORS_reset();
float angle_degree_a_pulse(float angle);
void danse(/*float angle*/);
void avancer(float distance_mm);
void reculer(float distance_mm);
void tourner(int direction, float angle, int sens);
void tournerCentre(int direction, float angle);
void tournerCrayon(int direction, float angle);

// ----- R O B O T  A U T O N O M E ------ //Appel des fonctions du robot autonome ici
//Formes
void polygone(int nbSommets, int lngrArete);
void parallelogramme(int base, int hauteur, float angle);
//Polygone étoilé
void arc(int rayon, float angle, int t);
void ellipse(int longeur, int largeur, int t);
//Spirale
void emotion(int emotion, int rayon);
//Électirque
//Informatique

// ----- R O B O T  M A N U E L ----- //Appel des fonctions du robot manuel ici

/*===========================================================================
Lancement
===========================================================================*/
void setup()
{
  BoardInit();
}
/*===========================================================================
Boucle infinie
===========================================================================*/
void loop()
{
  polygone(3, 100);//Test
  emotion(SOURIRE, 400);//Test
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  delay(10);// Delais pour décharger le CPU
}
/*===========================================================================
Définition des fonctions
===========================================================================*/
// ----- 2  R O B O T S ------
float PICalcul(float distanceGauche, float distanceDroite)
{
  float erreur = 0;
  float proportionnel = 0;
  float integral = 0;
  float PIresultant = 0;

  erreur = distanceGauche - distanceDroite;//Calcul de l'erreur
  proportionnel = erreur * 0.03f;//P
  integral = (integral + (erreur * 0.1f)) * 0.0f;//I

  //P=20, I=20    //PI=40 -> 40 tick de plus a faire
  PIresultant = (proportionnel+integral)/100;//Calcul PI en pulse
  Serial.print("PIOUT: ");
  Serial.println(PIresultant);
  return (PIresultant);
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
void acceleration(float *v, float vVoulue, float distance)
{
  float distanceAvantFrein = distance - ((-((*v)*(*v)))/(2*(0.01f/0.035f)));
  while(*v != vVoulue && ENCODER_Read(GAUCHE) < distance_mm_pulse(distanceAvantFrein))
  {
    if(*v < vVoulue)
      *v = *v + 0.01f;
    else if(*v > vVoulue)
      *v = *v - 0.01f;
    
    MOTOR_SetSpeed(GAUCHE,*v);
    MOTOR_SetSpeed(DROITE,*v);
    delay(35);
  }
  while(*v != 0)
  {
    if(*v < 0)
      *v = *v + 0.01f;
    else if(*v > 0)
      *v = *v - 0.01f;
    
    MOTOR_SetSpeed(GAUCHE,*v);
    MOTOR_SetSpeed(DROITE,*v);
    delay(35);
  }
}
void MOTORS_reset()
{
  MOTOR_SetSpeed(GAUCHE,speed0);
  MOTOR_SetSpeed(DROITE,speed0);
  ENCODER_Reset(GAUCHE);
  ENCODER_Reset(DROITE);
  delay(150);
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

  return pulses_pour_angle_x;
}
void danse(/*float angle*/)
{
  //float anglePulse = angle_degree_a_pulse(angle);//Variable en pulse selon l'angle
  float vDecr, aDecr;
  for(vDecr = 0.6f, aDecr = 0.0f; vDecr > -0.6f; vDecr = vDecr-0.005f+aDecr, aDecr = aDecr-0.00025f)
  {
    MOTOR_SetSpeed(GAUCHE, 0.6f);
    MOTOR_SetSpeed(DROITE, vDecr);
    delay(50);
  }
  delay(3000);
  MOTORS_reset();
}
void avancer(float distance_mm)
{
  float distance_pulse, distgauche1, distdroite1, k;
  float speed= speed1;
  int counter=0;
  distance_pulse = distance_mm_pulse(distance_mm);

  while(ENCODER_Read(GAUCHE)<=distance_pulse)
  {
    //accel
    if(counter==0)
    {
      //acceleration(AVANCER);
      counter=1;
    }
    //deccel
    if(ENCODER_Read(GAUCHE)>=(distance_pulse/1.3))
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k=PICalcul(distgauche1,distdroite1);
      speed = speed3+k;
    
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
      k=PICalcul(distgauche1,distdroite1);
      speed = speed1+k;

      MOTOR_SetSpeed(GAUCHE,speed1);
      MOTOR_SetSpeed(DROITE,speed);
      delay(100);
    }
  }
  MOTORS_reset();
}
void reculer(float distance_mm)
{
  int counter=0;
  float distance_pulse,distgauche1,distdroite1;
  float k;
  //float accel;
  float speed= -speed1;
  distance_pulse = distance_mm_pulse(distance_mm);
  while(ENCODER_Read(GAUCHE)>=-distance_pulse)
  {
     //accel
    if(counter==0)
    {
      //acceleration(RECULER);
      counter=1;
    }
    //deccel
    if(ENCODER_Read(GAUCHE)>=(distance_pulse/1.3))
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k=PICalcul(distgauche1,distdroite1);
      speed = speed3+k;
    
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
      k=PICalcul(distgauche1,distdroite1);
      speed = -speed1+k;
      MOTOR_SetSpeed(GAUCHE,-speed1);
      MOTOR_SetSpeed(DROITE,speed);
      delay(100);
    }
  }
  MOTORS_reset();
}
void tourner(int direction, float angle, int sens)
{
  float angle_pulse;
 
  //détermine le nombre de pulse pour arriver à l'angle demandé
  angle_pulse = angle_degree_a_pulse(angle);

  if((direction == GAUCHE && sens == DEVANT) || (direction == DROITE && sens == DERRIERE))
  {
    while(ENCODER_Read(DROITE)<=angle_pulse)
    {
        MOTOR_SetSpeed(GAUCHE,speed0);
        MOTOR_SetSpeed(DROITE,sens*speed4);
    }
  }
  if((direction == DROITE && sens == DEVANT) || (direction == GAUCHE && sens == DERRIERE))
  {
    while(ENCODER_Read(GAUCHE)<=angle_pulse)
    {
      MOTOR_SetSpeed(GAUCHE,sens*speed4);
      MOTOR_SetSpeed(DROITE,speed0);
    }
  }
  MOTORS_reset();
}
void tournerCentre(int direction, float angle)
{
  float anglePulse = angle_degree_a_pulse(angle);//Variable en pulse selon l'angle

  if(direction == GAUCHE)
  {
    while(ENCODER_Read(DROITE) <= anglePulse/2)
    {
      MOTOR_SetSpeed(GAUCHE,-speed2);
      MOTOR_SetSpeed(DROITE,speed2);
    }
  }
  else if(direction == DROITE)
  {
    while(ENCODER_Read(GAUCHE) <= anglePulse/2)
    {
      MOTOR_SetSpeed(GAUCHE,speed2);
      MOTOR_SetSpeed(DROITE,-speed2);
    }
  }
  MOTORS_reset();
}
void tournerCrayon(int direction, float angle)
{
  //LEVER LE CRAYON
  //avancer(/*INSÉRER DISTANCE EN MM ENTRE LE MILIEU DES ROUES ET LE CRAYON*/);
  tournerCentre(direction, angle);
  //reculer(/*INSÉRER DISTANCE EN MM ENTRE LE MILIEU DES ROUES ET LE CRAYON*/);
  //BAISSER LE CRAYON
}

// ----- R O B O T  A U T O N O M E ------ //Définitions des fonctions du robot autonome ici
#ifdef ROBOTAUTONOME
  //Formes
  void polygone(int nbSommets, int lngrArete)
  {
    for(int tournant = 0 ; tournant < nbSommets ; tournant++)
    {
      avancer(lngrArete);
      tournerCrayon(GAUCHE, ((nbSommets-2)*180)/nbSommets);
    }
  }
  void parallelogramme(int base, int hauteur, float angle)
  {
    for(int diagonale = 0 ; diagonale < 2 ; diagonale++)
    {
      avancer(base);
      tournerCrayon(GAUCHE, 180 - angle);
      avancer(hauteur/cos(angle - 90));
      tournerCrayon(GAUCHE, angle);
    }
  }
  void arc(int rayon, float angle, int t)
  {
    float anglePulse = angle_degree_a_pulse(angle);//Variable en pulse selon l'angle

    float vG = 2*PI*(rayon-(/*DISTANCE ENTRE ROUES MM*/100/2))/t;
    float vD = 2*PI*(rayon+(/*DISTANCE ENTRE ROUES MM*/100/2))/t;

    while(ENCODER_Read(DROITE) <= anglePulse)
    {
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
  }
  void ellipse(int longeur, int largeur, int t)
  {
    float vG = 2*PI*(largeur/2)/t;
    float vD = 2*PI*(largeur/2)/t;
    
    for(int i = 0 ; i < t/4 ; i = i++)
    {
      //vG = vG + ???;
      //r = sqrt(pow(largeur/2,2) + pow(longueur/2,2));
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
    for(int i = 0 ; i < t/4 ; i = i++)
    {
      //vG = vG - ???;
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
    for(int i = 0 ; i < t/4 ; i = i++)
    {
      //vG = vG + ???;
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
    for(int i = 0 ; i < t/4 ; i = i++)
    {
      //vG = vG - ???;
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
  }
  void emotion(int emotion, int rayon)
  {
    //Contour
    arc(rayon, 360, 200);//1
    tournerCrayon(GAUCHE, 90);//2
    switch(emotion)
    {
      case SOURIRE:
      //Transition
      avancer(rayon/3);//3
      tournerCrayon(DROITE, 90);//4
      avancer(rayon/3);//5
      tournerCrayon(GAUCHE, 90);//6
      avancer(rayon/3);//7
      //Yeux
      arc(rayon/6, 360, 200/6);//8
      avancer(rayon*2/3);//9
      arc(rayon/6, 360, 200/6);//10
      //Transition
      avancer(rayon/3);//11
      tournerCrayon(GAUCHE, 90);//12
      avancer(rayon/3);//13
      //Bouche
      arc(rayon*2/3, 180, 100);//14
      //Transition
      tournerCrayon(DROITE, 90);//15
      avancer(rayon/3);//16
      tournerCrayon(GAUCHE, 90);//17
      break;
      case TRISTE:
      //Transition
      avancer(rayon/3);//3
      tournerCrayon(DROITE, 90);//4
      avancer(rayon/3);//5
      tournerCrayon(GAUCHE, 90);//6
      avancer(rayon/3);//7
      //Yeux
      arc(rayon/6, 360, 200/6);//8
      avancer(rayon*2/3);//9
      arc(rayon/6, 360, 200/6);//10
      //Transition
      avancer(rayon/3);//11
      tournerCrayon(GAUCHE, 90);//12
      avancer(rayon);//13
      tournerCrayon(GAUCHE, 90);//14
      avancer(rayon*4/3);//15
      tournerCrayon(GAUCHE, 90);//16
      //Bouche
      arc(rayon*2/3, 180, 100);//17
      //Transition
      tournerCrayon(GAUCHE, 90);//18
      avancer(rayon*4/3);//19
      tournerCrayon(GAUCHE, 90);//20
      avancer(rayon*2/3);//21
      tournerCrayon(DROITE, 90);//22
      avancer(rayon/3);//23
      tournerCrayon(GAUCHE, 90);//24
      break;
      case BLAZE:
      //Transition
      avancer(rayon/3);//3
      tournerCrayon(DROITE, 90);//4
      avancer(rayon/3);//5
      tournerCrayon(GAUCHE, 90);//6
      avancer(rayon/3);//7
      //Yeux
      arc(rayon/6, 360, 200/6);//8
      avancer(rayon*2/3);//9
      arc(rayon/6, 360, 200/6);//10
      //Transition
      avancer(rayon/3);//11
      tournerCrayon(GAUCHE, 90);//12
      avancer(rayon/3);//13
      //Bouche
      arc(rayon*2/3, 180, 100);//14
      //Transition
      tournerCrayon(DROITE, 90);//15
      avancer(rayon/3);//16
      tournerCrayon(GAUCHE, 90);//17
      break;
      default:
      //ERREUR
      break;
    }
  }
#endif

// ----- R O B O T  M A N U E L ----- //Définitions des fonctions du robot manuel ici
#ifdef ROBOTMANUEL
  //Formes
#endif
