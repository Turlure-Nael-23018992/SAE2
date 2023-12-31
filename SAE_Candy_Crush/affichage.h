#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include <cst.h>
#include <type.h>


void clearScreen ();

void couleur (const unsigned & coul);

void  afficheMatriceV0 (const CMatrice & mat);

void  afficheMatriceV2 (const CMatrice & mat);

void  afficheMatriceVSwitch (const CMatrice & mat, const size_t & ligneAct , const size_t & colAct);

#endif // AFFICHAGE_H
