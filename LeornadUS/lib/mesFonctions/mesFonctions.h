// ----- G É N É R A L ------
#ifndef GENERAL
    #define GENERAL

    #include <LibRobus.h>

    //Defines
    #define GAUCHE 0
    #define DROITE 1
    #define DEVANT 1
    #define DERRIERE -1

    #define CONTENT 0
    #define TRISTE 1

    #define speed0 0
    #define speed1 0.4
    #define speed2 0.25
    #define speed3 0.3
    #define speed4 0.35

    //Variables
    float vitesse;
    
    //Fonctions
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

    //Formes
    void polygone(int nbSommets, int lngrArete);
    void emotion(int emotion, int rayon);

    //DÉCOMMENTEZ LE #define du robot que vous voulez utiliser SEULEMENT
    //#define ROBOTAUTONOME
    //#define ROBOTMANUEL

    //Si 2 robots définis, dé-defini les deux codes
    #ifdef ROBOTAUTONOME
        #ifdef ROBOTMANUEL
            #undef ROBOTAUTONOME
            #undef ROBOTMANUEL
        #endif
    #endif

    // ----- A U T O N O M E ------
    #ifdef ROBOTAUTONOME
        //Fonctions du robot autonome ici
    #endif

    // ----- M A N U E L -----
    #ifdef ROBOTMANUEL
        //Fonctions du robot manuel ici

    #endif
#endif