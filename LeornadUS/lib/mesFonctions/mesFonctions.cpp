#include "mesFonctions.h"

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
