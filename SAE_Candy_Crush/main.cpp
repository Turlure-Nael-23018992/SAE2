#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <termios.h>
#include <time.h>
#include <unistd.h>



#include <type.h>
#include <cst.h>
#include <affichage.h>


using namespace std;


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
void explosionUneBombeHorizontale (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & aLaSuite){
    for (size_t i=numLigne ; i>0 ; i=i-1){
        for (size_t j = 0 ; j < aLaSuite ; ++j){
            mat[i][numColonne+j]=mat[i-1][numColonne+j];
        }
    }
    for (size_t p=0 ; p < aLaSuite ; ++p){
        mat[0][numColonne+p]=rand() % KPlusGrandNombreDansLaMatrice +1;
    }
}

void explosionUneBombeVerticale (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & aLaSuite) {
    size_t nombreAFaireDescendre = 0;
    for(; nombreAFaireDescendre < numLigne; ++nombreAFaireDescendre){
    }
    // Cette boucle remplace simplement par des 0 les valeurs alignés
    //        mat[i][numColonne] = 0;
    //        mat[numLigne+aLaSuite-1][numColonne] = mat[nombreAFaireDescendre-1][numColonne];
    //        mat[numLigne+aLaSuite-2][numColonne] = mat[nombreAFaireDescendre-2][numColonne];
    //         mat[numLigne+aLaSuite-3][numColonne] = mat[0][numColonne];

    //    }
    //    mat[nombreAFaireDescendre-1][numColonne] = 0;
    //    mat[nombreAFaireDescendre-2][numColonne] = 0;
    //    mat[0][numColonne] = 0;
    for (size_t i = numLigne ; i < numLigne+aLaSuite; ++i){


        for (size_t j = 1 ; j < nombreAFaireDescendre+1 ; ++j){

            mat[numLigne+aLaSuite-j][numColonne] = mat[nombreAFaireDescendre-j][numColonne];

        }
    }
    for (size_t x = 0; x < aLaSuite; ++x){
        mat[x][numColonne] = rand() % KPlusGrandNombreDansLaMatrice +1;
    }
}

size_t nombrePoints = 0;

bool detectionExplosionUneBombeHorizontale(CMatrice &mat) {
    bool auMoinsUneExplosion = false;
    size_t temp = 0;
    size_t aLaSuite = 0;
    int caseAct = mat[0][0];

    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = 0; j < mat[i].size(); ++j) {
            if (caseAct != mat[i][j]) {
                aLaSuite = 1;
                temp = j;
                caseAct = mat[i][j];
            } else {
                ++aLaSuite;
                    // Mettez à jour temp pour conserver la position du début de la suite
                if (aLaSuite >= 3 && (j == mat[i].size() - 1 || mat[i][j + 1] != caseAct)) {
                    auMoinsUneExplosion = true;
                    cout << "On a une suite en position numLigne = " << i + 1
                         << "; colonne = " << temp + 1
                         << "; sur  " << aLaSuite << " cases" << endl;
                    nombrePoints += aLaSuite ;
                    // Effectuer la suppression (appel à la fonction explositionUneBombeHorizontale)
                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                    afficheMatriceV2(mat);

                    cout << "POINTS :  " << nombrePoints  << endl;
                    sleep(1);

                    explosionUneBombeHorizontale(mat, i, temp, aLaSuite);
                    cout << string(20, '-') << endl << "Matrice après suppression" << endl;
                                afficheMatriceV2(mat);
                }
            }
        }
    }
    return auMoinsUneExplosion;
}

bool detectionExplosionUneBombeVerticale(CMatrice &mat) {
    bool auMoinsUneExplosion = false;
    size_t caseAct = mat[0][0];
    size_t aLaSuite = 0;
    vector<size_t> tab;
    for (size_t colonne = 0; colonne < mat[0].size(); ++colonne) {
        aLaSuite = 0;
        for (size_t ligne = 0; ligne < mat.size(); ++ligne) {
            tab.push_back(mat[ligne][colonne]);
        }
        for (size_t i = 0; i < tab.size(); ++i) {
            if (caseAct != tab[i]) {
                caseAct = tab[i];
                aLaSuite = 1;
            } else {
                ++aLaSuite;
                if (aLaSuite >= 3 && (i == tab.size() - 1 || tab[i + 1] != caseAct)) {
                    auMoinsUneExplosion = true;
                    cout << "On a une suite en position numLigne = " << i % mat.size() - aLaSuite/2
                         << "; colonne = " << colonne + 1
                         << "; sur  " << aLaSuite << " cases" << endl;
                    nombrePoints += aLaSuite;

                    // Effectuer la suppression (appel à la fonction explositionUneBombeVerticale)
                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                    afficheMatriceV2(mat);
                    cout << "POINTS" << nombrePoints << endl;
                    sleep(1);

                    explosionUneBombeVerticale(mat, i % mat.size() - aLaSuite / 2 - 1, colonne, aLaSuite);
                    cout << string(20, '-') << endl << "Matrice après suppression" << endl;
                                afficheMatriceV2(mat);

                }
            }
        }
        // Effacer le vecteur pour la colonne suivante
        tab.clear();
    }
    return auMoinsUneExplosion;
}
CMatrice destructionCascade(CMatrice &mat){
    while (detectionExplosionUneBombeHorizontale(mat) || detectionExplosionUneBombeVerticale(mat)){
        detectionExplosionUneBombeHorizontale(mat);
        detectionExplosionUneBombeVerticale(mat);
    }

    return mat;
}

bool estCoupJouable(CMatrice mat,CInt pair1, size_t ajoutLigne,size_t ajoutColonne )

{
    bool jouable = false ;
    CMatrice mat2 = mat;
    swap (mat2[pair1.first][pair1.second], mat2[pair1.first+ajoutLigne][pair1.second+ajoutColonne]);
    if (detectionExplosionUneBombeHorizontale(mat2) || detectionExplosionUneBombeVerticale(mat2)){
        jouable = true ;
    }
    return jouable;
}


//Ces deux variables sont déclarées en dehors de la fonction, puisque sinon elles serait réinitialisés à chaque itération de la fonction
size_t ligneAct = 4;
size_t colonneAct = 4;

bool faitUnMouvement(CMatrice & mat) {

    bool premierMouvement = false;

    char deplacement;
    char inversion;

    cin >> deplacement ;

    bool deplacementValide = true;

    switch (tolower(deplacement)) {
    case 'z': {
        cout << "Déplacement vers le haut demandé." << endl;
            --ligneAct;
          premierMouvement = true;
        break;
    }
    case 'q': {
        cout << "Déplacement vers la gauche demandé." << endl;
            --colonneAct;
        premierMouvement = true;
        break;
    }
    case 's': {
        cout << "Déplacement vers le bas demandé." << endl;
            ++ligneAct;
        premierMouvement = true;
        break;
    }
    case 'd': {
        cout << "Déplacement vers la droite demandé." << endl;
            ++colonneAct;
        premierMouvement = true;
        break;
    }
    case 'i':{

        CInt position = make_pair(ligneAct,colonneAct) ;
        cout << "VOUS ETES EN MODE INVERSION" << endl << "Entrez z , q , s , d pour intervertir la case actuelle avec celle du dessus, de gauche, de droite, ou d'en bas" << endl;
        cin >> inversion ;

        switch (inversion) {
        case 'z':{
            if (estCoupJouable(mat,position,-1,0)){
                swap (mat[ligneAct][colonneAct], mat[ligneAct-1][colonneAct]);
            }
            else{

                cout << "CE DEPLACEMENT N'EST PAS POSSIBLE !!!!" << endl;
            }


            break;
        }
        case 'q':{

            swap (mat[ligneAct][colonneAct], mat[ligneAct][colonneAct-1]);
            if (estCoupJouable(mat,position,0,-1)){
                swap (mat[ligneAct][colonneAct], mat[ligneAct][colonneAct-1]);
            }
            else{

                cout << "CE DEPLACEMENT N'EST PAS POSSIBLE !!!!" << endl;
            }

            break;
        }
        case 's':{

            if (estCoupJouable(mat,position,1,0)){
                sleep(3);
                swap (mat[ligneAct][colonneAct], mat[ligneAct+1][colonneAct]);

            }
            else{

                cout << "CE DEPLACEMENT N'EST PAS POSSIBLE !!!!" << endl;
            }

            break;
        }
        case 'd':{
            if (estCoupJouable(mat,position,0,1)){
                sleep(3);
                swap (mat[ligneAct][colonneAct], mat[ligneAct][colonneAct+1]);
            }
            else{

                cout << "CE DEPLACEMENT N'EST PAS POSSIBLE !!!!" << endl;
            }



            break;
        }



        default:
            break;
        }



    }
    default:
        cout << "Choisissez z, q, s, d" << endl;
        break;

    }

    if (ligneAct < 0 || ligneAct > 9 || colonneAct < 0 || colonneAct > 9) {
        deplacementValide = false;
    }
    if (deplacementValide == true){

        afficheMatriceVSwitch(mat, ligneAct , colonneAct);
    }
    else {

        cout << "Ce déplacement n'est pas possible, vous allez sortir de la grille" << endl << endl << endl << "Votre position a été réinitialisé au milieu de la matrice" << endl;
            colonneAct = 4 ;
        ligneAct = 4;
    }
    return premierMouvement;
}


void testSwitch(CMatrice &mat)
{
    cout << "Entrez, z (haut) , q (gauche), s (bas) , d (droite) Pour vous déplacer " << endl << endl << "Pour passer en mode inversion de case entrer I" << endl << endl;

                    cout << "Veuillez faire un mouvement" << endl;
    faitUnMouvement(mat);

}

void premiereGame(CMatrice & mat) {




    while(true){
        destructionCascade(mat);
        testSwitch(mat);
    }


}

int main() {
    clearScreen();
    CMatrice mat ;
    initMat(mat,10,10,20);

    premiereGame(mat);





}







