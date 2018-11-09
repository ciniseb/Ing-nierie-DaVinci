/*============================================
Projet: Code source
Equipe: P14
Auteurs: Simon St-Onge, Philippe B-L, Éric Leduc, Sébastien St-Denis
Description: Parcours de LéonardUS
Date: 11 octobre 2018
============================================*/
#include <mesFonctions.h>
/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibales globales
void setup()
{
  BoardInit();
}
/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"
void loop()
 {
   danse();
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  delay(10);// Delais pour décharger le CPU

}