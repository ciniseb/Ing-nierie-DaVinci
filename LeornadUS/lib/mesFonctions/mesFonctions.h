#ifndef MESFONCTIONS_H
#define MESFONCTIONS_H

#include <LibRobus.h>
#include <Arduino.h>

//Defines
#define GAUCHE 0
#define DROITE 1
#define speed0 0
#define speed1 0.4
#define speed2 0.25
#define speed3 0.3
#define speed4 0.35

//Variables
float vitesse;

//Fonctions
float PICalcul(float distanceGauche, float distanceDroite);
void acceleration(float v, float vMax, float distance);
void MOTORS_reset();
float angle_degree_a_pulse(float angle);
void tourner(int direction, float angle);
void tournerCentre(int direction, float angle);
void danse(/*float angle*/);
float distance_mm_pulse(float distance_mm);
void avancer(float distance_mm);
void reculer(float distance_mm);
void tourner_reculer(int direction, float angle);

#endif