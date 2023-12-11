#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

void clearScreen () {
    cout << "\033[H\033[2J";
}

const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);
const unsigned KBGNoir  (40);
const unsigned KBGRouge (41);
const unsigned KGBBleu  (44);
const unsigned KBGCyan  (46);

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

typedef ushort contenueDUneCase;
typedef vector <contenueDUneCase> CVLigne; // un type représentant une ligne de la grille
typedef vector <CVLigne> CMatrice; // un type représentant la grille

const contenueDUneCase KAIgnorer = 0;
const contenueDUneCase KPlusGrandNombreDansLaMatrice = 4;

void initMat (CMatrice & mat, const size_t & nbLignes = 10,
              const size_t & nbColonnes = 10,
              const unsigned & nbMax= KPlusGrandNombreDansLaMatrice){
    mat.resize(nbColonnes);
    for (size_t i=0 ; i < nbLignes ; ++i){
        mat[i].resize(nbLignes);
        for (size_t j=0 ; j< nbLignes; ++j){
            mat[i][j] = rand() % KPlusGrandNombreDansLaMatrice +1 ;
        }
    }
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

void explositionUneBombeHorizontale (CMatrice & mat, const size_t & numLigne,
                                     const size_t & numColonne, const size_t & aLaSuite){
    for (size_t i=numLigne ; i>0 ; i=i-1){
        for (size_t j = 0 ; j < aLaSuite ; ++j){
            mat[i][numColonne+j]=mat[i-1][numColonne+j];
        }
    }
    for (size_t p=0 ; p < aLaSuite ; ++p){
        mat[0][numColonne+p]=rand() % KPlusGrandNombreDansLaMatrice +1;
    }
}

void explositionUneBombeVerticale (CMatrice & mat, const size_t & numLigne,
                                   const size_t & numColonne, const size_t & aLaSuite){
    size_t j=numColonne;
    cout << numLigne << "   " << numColonne << "    " << numLigne+aLaSuite ;
    for (size_t i=numLigne ; i < numLigne+aLaSuite ; ++i){
        mat[i][j]=mat[i][aLaSuite];
    }
}

void casserVertical(){

}

void remplirVertical(){

}

bool detectionExplositionUneBombeHorizontale (CMatrice & mat){
    bool auMoinsUneExplosion (false);
    size_t caseAct ;
    size_t temp;
    size_t aLaSuite = 0 ;
    for (size_t i = 0 ; i < mat[0].size() ; ++i){
        caseAct = 0 ;
        for (size_t j = 0 ; j< mat[0].size() ; ++j){
            if (caseAct != mat[i][j]){
                aLaSuite = 1 ;
                temp = j+1;
                caseAct=mat[i][j];
            }
            else {
                ++aLaSuite;
                if (aLaSuite >=3 && mat[i][j+1]!= caseAct){
                    auMoinsUneExplosion = true;
                    cout << "on a une suite en position numLigne = " << i+1
                         << "; colonne = " << temp
                         << "; sur  " << aLaSuite << " cases" << endl;
                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                    afficheMatriceV2(mat);
                    explositionUneBombeHorizontale (mat, i, temp-1, aLaSuite);
                    cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                    afficheMatriceV0(mat);
                }
            }

        }
    }
    return auMoinsUneExplosion;
}

bool detectionExplositionUneBombeVerticale(CMatrice & mat){
    bool auMoinsUneExplosion (false);
    size_t caseAct ;
    size_t temp;
    size_t aLaSuite = 0 ;
    vector<size_t>tab ;
    for (size_t colonne = 0 ; colonne < mat[0].size() ; ++colonne){
        for (size_t ligne = 0 ; ligne < mat[0].size() ; ++ligne ){
            tab.push_back(mat[ligne][colonne]);
        }
    }
    for(size_t i=0 ; i < tab.size() ; ++i){
        if (caseAct != tab[i]){
            caseAct = tab[i];
            aLaSuite = 1;
        }
        else{
            ++ aLaSuite ;
            if (aLaSuite >= 3 && tab[i+1] != caseAct){
                auMoinsUneExplosion = true;
                cout << "on a une suite en position numLigne = " << i%10 -aLaSuite/2
                     << "; colonne = " << 1 + i/10
                     << "; sur  " << aLaSuite << " cases" << endl;
                cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                afficheMatriceV2(mat);
                explositionUneBombeVerticale (mat, i%10 -(aLaSuite/2) -1, i/10, aLaSuite);
                cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                afficheMatriceV2(mat);
            }
        }
    }
    return auMoinsUneExplosion;
}
int main() {
    clearScreen();
    CMatrice mat ;
    initMat(mat,10,10,20);
    detectionExplositionUneBombeVerticale(mat);

    return 0;
}
