#include "affichage.h"
#include <iostream>

using namespace std;



void clearScreen () {
    cout << "\033[H\033[2J";
}
void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

void  afficheMatriceV0 (const CMatrice & mat) {
    cout << "\033[" << 30 <<"m";
    for (size_t i=0 ; i < mat[0].size() ; ++i){
        for (size_t j=0 ; j< mat[0].size(); ++j){
            cout << "\033[" << mat[i][j]%10+40 <<"m";
            cout << mat[i][j] << " " ;
        }cout << endl ;
    }
    cout << "\033[" << 0 <<"m";
}

void  afficheMatriceV2 (const CMatrice & mat) {
    cout << "    ";
    for (size_t m=0 ; m< mat[0].size(); ++m){
        cout << m+1 << " ";
    }
    cout << endl;
    for (size_t i=0 ; i < mat[0].size() ; ++i){
        cout << "\033[" << 0 <<"m";
        if (i+1 == 10){
            cout << i+1 << ": ";
        }
        else {
            cout << i+1 << " : ";
        }
        for (size_t j=0 ; j< mat[0].size(); ++j){
            cout << "\033[" << 30 <<"m";
            cout << "\033[" << mat[i][j]%10+40 <<"m";
            cout << mat[i][j] << " " ;
        }cout << endl ;
    }
    cout << "\033[" << 0 <<"m";
}
