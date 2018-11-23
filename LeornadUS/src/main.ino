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
#include <math.h>
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
//AUTRES ÉMOTIONS

#define speed0 0
#define speed1 0.4
#define speed2 0.25
#define speed3 0.3
#define speed4 0.35

/*===========================================================================
Variables globales
===========================================================================*/
float vitesse;
int anglecrayon;
/*===========================================================================
Appel des fonctions
===========================================================================*/
float PICalcul(float distanceGauche, float distanceDroite);
float distance_mm_pulse(float distance_mm);
void acceleration(float v, float vMax, float distance);
void MOTORS_reset();
float angle_degree_a_pulse(float angle);
void accel_avancer();
void accel_reculer();
void avancer(float distance_mm);
void reculer(float distance_mm);
void baisserCrayon();
void leverCrayon();
void tourner(int direction, float angle, int sens);
void tournerCentre(int direction, float angle);
void tournerCrayon(int direction, float angle);

// ----- R O B O T  A U T O N O M E ------ //Appel des fonctions du robot autonome ici
//Formes
void polygone(int nbSommets, int lngrArete);
void polygoneEtoile(int nbSommets,int diffSommets, int lngrArete);
void croix(int lngrArete);
void arc(int rayon, float angle, int t);
void ellipse(int longeur, int largeur, int t);
void spirale();
void spirale2();
void parallelogramme(float base, float hauteur, float angle);
void emotion(int emotion, int rayon);
void electrique();
void informatique();

// ----- R O B O T  M A N U E L ----- //Appel des fonctions du robot manuel ici

/*===========================================================================
Lancement
===========================================================================*/
void setup()
{
  BoardInit();

  pinMode(A10,INPUT);
  pinMode(A9,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  pinMode(A8,INPUT);
  pinMode(48, INPUT_PULLUP);        // Pin bouton bras.

  delay(5000);
}
/*===========================================================================
Boucle infinie
===========================================================================*/
void loop()
{
  // ----- R O B O T  A U T O N O M E ------
  #ifdef ROBOTAUTONOME
    int noForme = 12;
    switch(noForme)
    {
      case 0:
        polygone(2, 100);//Digone
      break;
      case 1:
        polygone(3, 100);//Triangle
      break;
      case 2:
        polygone(4, 100);//Carré
      break;
      case 3:
        polygone(5, 100);//Pentagone
      break;
      case 4:
        polygone(6, 100);//Hexagone
      break;
      case 5:
        polygone(7, 100);//Heptagone
      break;
      case 6:
        polygone(8, 100);//Octogone
      break;
      case 7:
        polygone(9, 100);//Ennéagone
      break;
      case 8:
        polygone(10, 100);//Décagone
      break;
      case 9:
        polygone(11, 100);//Hendécagone
      break;
      case 10:
        polygone(12, 100);//Dodécagone
      break;
      case 11:
        //polygoneEtoile(, int lngrArete);
      break;
      case 12:
       arc(300, 180, 75);
      break;
      case 13:
      
      break;
      case 14:
      
      break;
      default:
      break;
    }
  #endif
  // ----- R O B O T  M A N U E L -----
  #ifdef ROBOTMANUEL

  #endif
  if(ROBUS_IsBumper(3))
  {
    baisserCrayon();

    //Formes
    //polygone(5, 70);//FONCTIONNE
    //parallelogramme(100, 40, 120);//FONCTIONNE
    //polygoneEtoile(int nbSommets,int diffSommets ,int lngrArete);
    //arc(int rayon, float angle, int t);
    //croix(100);
    //ellipse(int longeur, int largeur, int t);
    //Spirale
    //emotion(SOURIRE, 400);
    //electrique();
    //informatique();

    leverCrayon();
  }

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
  //Serial.print("PIOUT: ");
  //Serial.println(PIresultant);
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
void accel_avancer()
{
  float accel=0;
      while(accel<=0.35)
      { 
        //vprobeFreq();   
        accel = accel + 0.01;
        MOTOR_SetSpeed(GAUCHE,accel);
        MOTOR_SetSpeed(DROITE,accel);
        //marteau();
        delay(35);
      }
}
void accel_reculer()
{
  float accel=0;
      while(accel<=0.35)
      {  
        //vprobeFreq();   
        accel = accel + 0.01;
        MOTOR_SetSpeed(GAUCHE,-accel);
        MOTOR_SetSpeed(DROITE,-accel);
        delay(35);
      }
}
void avancer(float distance_mm)
{
  float distance_pulse,distgauche1,distdroite1;
  float k;
  float speed = speed1;
  int counter=0;
  distance_pulse = distance_mm_pulse(distance_mm);

  while(ENCODER_Read(GAUCHE)<=distance_pulse)
  {
    //accel
    if(counter==0)
    {
      accel_avancer();
      counter=1;
    }
    //deccel
    if(ENCODER_Read(GAUCHE)>=(distance_pulse/1.3))
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k = PICalcul(distgauche1,distdroite1);
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
      k = PICalcul(distgauche1,distdroite1);
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
  float speed = -speed1;
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
void baisserCrayon()
{
  int actif = 1;
  int angle = 125;
  SERVO_Enable(0);
  SERVO_SetAngle(0,angle);
  delay(100);

  while(actif == 1)
  {
    Serial.println(angle);
    SERVO_SetAngle(0, (angle));
    delay(0);
    // ROBUS_IsBumper(1)
    if (!digitalRead(48))
    {
      actif = 0;
      SERVO_Disable(0);
    }
    angle--;
    if (angle == 0)
    {
      actif = 0;
      SERVO_Disable(0);
    }
  }
  SERVO_Disable(0);
}
void leverCrayon()
{
  SERVO_Enable(0);

  delay(100);
  while (anglecrayon > 0)
  {
    anglecrayon--;
    SERVO_SetAngle(0,anglecrayon);
    delay(20);
  }
  delay(20);
  SERVO_Disable(0);
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
  leverCrayon();
  avancer(/*INSÉRER DISTANCE EN MM ENTRE LE MILIEU DES ROUES ET LE CRAYON*/70);
  tournerCentre(direction, angle);
  reculer(/*INSÉRER DISTANCE EN MM ENTRE LE MILIEU DES ROUES ET LE CRAYON*/70);
  baisserCrayon();
}
void tournerEfface(int direction, float angle)
{
  //avancer(distance entre roues et efface);
  //reculer(distance entre roues et efface);
  tournerCentre(direction,angle);
}

// ----- R O B O T  A U T O N O M E ------ //Définitions des fonctions du robot autonome ici
#ifdef ROBOTAUTONOME
  //Formes
  void polygone(int nbSommets, int lngrArete)
  {
    for(int tournant = 0 ; tournant < nbSommets ; tournant++)
    {
      avancer(lngrArete);
      tournerCrayon(GAUCHE, 180 - (((nbSommets-2)*180)/nbSommets));
    }
     /*-----ZAMBONI------
    for(int tournant = 0; tournant < nbSommets ; tournant++)
    {
      avancer(distance entre crayon et roues);
      //DESCENDRE EFFACE
      avancer(lngrArete);
      tournerEfface(GAUCHE, ((nbSommets-2)*180/nbSommets));
    }
    reculer(distance entre crayon et roues);*/
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
    /*
    for(int diagonale = 0 ; diagonale < 2 ; diagonale++)
    {
      avancer(distance entre crayon et roues)
      //Descendre efface
      avancer(base);
      tournerEfface(GAUCHE, 180-angle);
      avancer(hauteur/cos(angle-90));
      tournerEfface(GAUCHE, angle);
    }
    reculer(distance entre crayon et roues)*/
  }
  void polygoneEtoile(int nbSommets, int diffSommets ,int lngrArete)
  {
    
    float angle = 180*(nbSommets-2*diffSommets)/nbSommets;
    float angleExterne = 180-angle;
    float angleInterne = 360*(diffSommets-1)/nbSommets;

    tournerCrayon(DROITE, 90);
    for(int i = 0; i < nbSommets ; i++)
    {
      avancer(lngrArete);
      tournerCrayon(DROITE, angleInterne);
      avancer(lngrArete);
      tournerCrayon(GAUCHE, angleExterne);
    }
    leverCrayon();
    tournerCentre(GAUCHE, 90);
    reculer(18);
    /*
    avancer(/distance entre crayon et roues);
    for(int i = 0; i < nbSommets; i++)
    {
      avancer(lngrArete);
      tournerEfface(DROITE, angleInterne);
      avancer(lngrArete);
      tournerEffaceCrayon);
    }
    reculer(dsitance entre crayon et roues);
    tournerCentre(GAUCHE, 90);
    reculer(Distance entre crayon et roues);*/
  }
  void croix(int lngrArete)
  {
    for(int i=0; i<4; i++)
    {
      avancer(lngrArete);
      tournerCrayon(DROITE, 90);
      avancer(lngrArete);
      tournerCrayon(GAUCHE, 90);
      avancer(lngrArete);
      tournerCrayon(GAUCHE, 90);
    }
    //tournerCrayon(DROITE, 90);
    //avancer(lngrArete);
    //tournerCrayon(GAUCHE, 90);
  }
  void arc(int rayon, float angle, int t)
  {
    //float anglePulse = angle_degree_a_pulse(angle);//Variable en pulse selon l'angle
    float distance = (1.6*2*PI*(rayon+55)*(angle/545));

    float vG = (2*PI*(rayon+94))/t;
    float vD = (2*PI*(rayon-94))/t;

    while(ENCODER_Read(GAUCHE) <= distance_mm_pulse(distance))
    {
      Serial.println(ENCODER_Read(GAUCHE));
      Serial.println(distance_mm_pulse(distance));
      MOTOR_SetSpeed(GAUCHE, vG/100);
      MOTOR_SetSpeed(DROITE, vD/100);
    }
    while(ENCODER_Read(GAUCHE)> distance_mm_pulse(distance))
    {
      MOTOR_SetSpeed(GAUCHE, 0);
      MOTOR_SetSpeed(DROITE, 0);
      break;
    }
  }
  void ellipse(int longeur, int largeur, int t)
  {
    float vG = 2*PI*(largeur/2)/t;
    float vD = 2*PI*(largeur/2)/t;
    
    for(int i = 0 ; i < t/4 ; i++)
    {
      //vG = vG + ???;
      //r = sqrt(pow(largeur/2,2) + pow(lngrArete/2,2));
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
    for(int i = 0 ; i < t/4 ; i++)
    {
      //vG = vG - ???;
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
    for(int i = 0 ; i < t/4 ; i++)
    {
      //vG = vG + ???;
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
    for(int i = 0 ; i < t/4 ; i++)
    {
      //vG = vG - ???;
      MOTOR_SetSpeed(GAUCHE, vG);
      MOTOR_SetSpeed(DROITE, vD);
    }
  }
  void spirale()
  {
    float vMoins, dV;
    for(vMoins = 0.4f, dV = 0.0f; vMoins > -0.4f; vMoins = vMoins-dV, dV = 0.0005f/*dV+0.00025f*/)
    {
      MOTOR_SetSpeed(GAUCHE, vMoins);
      MOTOR_SetSpeed(DROITE, 0.4f);
      delay(50);
    }
    leverCrayon();
    avancer(300);
    delay(3000);
    MOTORS_reset();
  }
  void spirale2()
  {
    float bob=0, ross=0;
    while(ENCODER_Read(GAUCHE)<70000)
    {
      Serial.println(ENCODER_Read(GAUCHE));
      for(bob=0 ; bob < 0.4 ; bob = bob + 0.0005)
      {
        ross=ross+0.00025;
        MOTOR_SetSpeed(GAUCHE, bob);
        MOTOR_SetSpeed(DROITE, ross);
        Serial.print("GAUCHE =");
        Serial.println(bob);
        Serial.print("DROITE =");
        Serial.println(ross);
      }
    }
    /*WHILE(ENCODER_Read(GAUCHE)>70000)
    {
      leverCrayon();
      MOTOR_SetSpeed(GAUCHE, 0.1);
      MOTOR_SetSpeed(DROITE, 0.1)
      avancer(1000000000000);
      tournerCentre(DROITE,90);
      reculer(90);//distance entre crayon et roues
    }*/
  }
  void parallelogramme(float base, float hauteur, float angle)
  {
    for(int diagonale = 0 ; diagonale < 2 ; diagonale++)
    {
      float angle1 = 180 - angle;
      float angle2 = angle - 90;
      float angle3 = 90 - angle;
      avancer(base);
      Serial.print("AVANCE de ");
      Serial.println(base);
      tournerCrayon(GAUCHE, angle1);
      Serial.print("TOURNE de ");
      Serial.println(angle1);
      if(angle >= 90)
      {
        float distance = hauteur/cos(angle2);
        avancer(distance);
        Serial.print("AVANCE de ");
        Serial.println(distance);
      }
      else
      {
        float distance = hauteur/cos(angle3);
        avancer(distance);
        Serial.print("AVANCE de ");
        Serial.println(distance);
      }
      tournerCrayon(GAUCHE, angle);
      Serial.print("TOURNE de ");
      Serial.println(angle);
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
  void electrique()
  {
	  float P=40; //distance en mm des petits côtés
    tournerCrayon(GAUCHE, 55);
    avancer(85.81);
    tournerCrayon(DROITE, 90);
    avancer(P);
    tournerCrayon(GAUCHE, 90);
    avancer(66.65);
    tournerCrayon(DROITE, 90);
    avancer(P);
    tournerCrayon(GAUCHE, 95);
    avancer(60);
    tournerCrayon(GAUCHE, 52);
    avancer(70);
    tournerCrayon(GAUCHE, 125);
    avancer(50);
    tournerCrayon(DROITE,90);
    avancer(P);
    tournerCrayon(GAUCHE, 90);
    avancer(62.72);
    tournerCrayon(DROITE, 90);
    avancer(P);
    tournerCrayon(GAUCHE, 115);
    avancer(152.36/*+distance entre crayon et roues*/);
    //permet de reset le robot à la position ini.
    tournerCentre(DROITE, 149);
    reculer(/*distance entre crayon et roues*/100);
    /*
    avancer(distance entre crayon et roues);
    tournerEfface(GAUCHE, 55);
    avancer(85.81);
    tournerEfface(DROITE, 90);
    avancer(P);
    tournerEfface(GAUCHE, 90);
    avancer(66.65);
    tournerEfface(DROITE, 90);
    avancer(P);
    tournerEfface(GAUCHE, 95);
    avancer(60);
    tournerEfface(GAUCHE, 52);
    avancer(70);
    tournerEfface(GAUCHE, 125);
    avancer(50);
    tournerEfface(DROITE,90);
    avancer(P);
    tournerEfface(GAUCHE, 90);
    avancer(62.72);
    tournerEfface(DROITE, 90);
    avancer(P);
    tournerEfface(GAUCHE, 115);
    avancer(152.36);
    //permet de reset le robot à la position ini.
    leverCrayon();
    tournerCentre(DROITE, 149);
    reculer(/*distance entre crayon et roues100);*/
  }
  void informatique()
  {
    //avancer(/*distance entre crayon et roues*//100);
    tournerCentre(GAUCHE, 70);
    //reculer(/*distance entre crayon et roues*/);
    //Descendre crayon
    //arc(67.7, 134.79, t); 
    tournerCrayon(DROITE, 20);
    avancer(130);
    tournerCrayon(DROITE, 61.44);
    //reculer(/*Distance entre crayon et roues*/);
    //arc(130.23, 57.36, t);
    tournerCrayon(DROITE, 61.1);
    avancer(130);
    leverCrayon();
    tournerCentre(GAUCHE, 180);
    //DESCENDRE CRAYON
    //avancer(90.43-2*(/*distance entre crayon et roues*/));
    tournerCrayon(GAUCHE,90);
    avancer(125);
    leverCrayon();
    tournerCentre(GAUCHE, 180);
    //DESCENDRE CRAYON
    avancer(125/2);
    tournerCrayon(GAUCHE, 90);
    avancer(54.33);
    //arc(62.75, 89.77, t);
    //Retour à la position ini.
    leverCrayon();
    //avancer(/*Distance entre crayon et roues*//100);
    tournerCentre(DROITE, 90);
    avancer(207.75);
    tournerCentre(DROITE, 90);
    //reculer(/*Distance entre crayon et roues*//100);
    /*
    avancer(distance entre crayon et roues);
    tournerCentre(GAUCHE, 70);
    arc(67.7, 134.79, t); 
    tournerEfface(DROITE, 20);
    avancer(130);
    tournerEfface(DROITE, 61.44);
    arc(130.23, 57.36, t);
    tournerEfface(DROITE, 61.1);
    avancer(130);
    tournerCentre(GAUCHE, 180);
    avancer(90.43-2(distance entre crayon et roues));
    tournerEfface(GAUCHE,90);
    avancer(125);
    tournerCentre(GAUCHE, 180);
    avancer(125/2);
    tournerEfface(GAUCHE, 90);
    avancer(54.33);
    arc(62.75, 89.77, t)
    tournerCentre(DROITE, 90);
    avancer(207.75);
    tournerCentre(DROITE, 90);
    reculer(Distance entre crayon et roues);*/
  }  
#endif

// ----- R O B O T  M A N U E L ----- //Définitions des fonctions du robot manuel ici
#ifdef ROBOTMANUEL
  //Formes
#endif
