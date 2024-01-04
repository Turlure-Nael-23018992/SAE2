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


/**
 * @brief initMat Cette fonction sert à initier la matrice en lui donnant un taille c'est à dire une longueur et une largeur ainsi que la valeur max
 * qu'elle peut atteindre dans ses cases.
 * @param mat : la matrice
 * @param nbLignes : le nombre de lignes de la matrice
 * @param nbColonnes : le nombre de colonnes de la matrice
 * @param nbMax : la valeur max que peut atteindre
 */
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


/**
 * @brief superExplosionHorizontale : Cette fonction nous permet de casser une ligne entière lorsqu'une super suite est détectée, elle casse toute
 * la ligne et ensuite fait descendre tous les caractère du dessus et regénère des caractères sur la ligne du dessus.
 * @param mat : la matrice
 * @param ligne : la ligne du motif
 */
CMatrice superExplosionHorizontale(CMatrice mat , const size_t & ligne){
    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {

            cout << mat[i][j];




        }

}
     return mat;
}


/**
 * @brief superExplosionVerticale : Cette fonction nous permet de casser une colonne entière lorsqu'une super suite est detectée, elle génère
 * à chaque case de la colonne une valeur aléatoire car cela revient au même que de casser la colonne entière et ensuite générer de nouveau
 * des caractères.
 * @param mat : la matrice
 * @param colonne : la colonne ou se trouve la super suite
 */
CMatrice superExplosionVerticale(CMatrice mat, const size_t &colonne){
    for (size_t i=0 ; i < mat.size() ; ++i) {
         cout << mat[i][colonne] << endl;
        mat[i][colonne] =0;
    }

    return mat;
}

/**
 * @brief explosionUneBombeHorizontale : la fonction nous permet de détruire les motifs de 3 lettres à l'horizontale, faire descendre les caractères
 * du dessus , et regénérer des caractères en haut de la matrice.
 * @param mat : la matrice
 * @param numLigne : la ligne à laquelle le motif se trouve
 * @param numColonne : la ligne à laquelle le motif commence
 * @param aLaSuite : la taille du motif
 */
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

/**
 * @brief explosionUneBombeVerticale : cette fonction nous permet de détruire les motifs de 3 lettres à la verticale , faire descendre les caractères
 * du dessus , et regénérer des caractères en haut de la matrice.
 * @param mat : la matrice
 * @param numLigne : la ligne à laquelle le motif commence
 * @param numColonne : la colonne à laquelle le motif se trouve
 * @param aLaSuite : la taille du motif
 */
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


/**
 * @brief detectionExplosionUneBombeVerticale :fonction booleen qui détecte s'il y a une suite d'au moins 3 caractères égaux à la verticale
 * et apelle en conséquence la fonction pour casser le motif
 * @param mat : la matrice
 * @return : la fonction renvoie true si un motif est présent à la verticale dans la matrice
 */
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
                if (aLaSuite >= 3 && (j == mat[i].size() - 1 || mat[i][j + 1] != caseAct)) {
                    auMoinsUneExplosion = true;
                    if (aLaSuite == 3){
                        cout << "On a une suite en position numLigne = " << i + 1
                             << "; colonne = " << temp + 1
                             << "; sur  " << aLaSuite << " cases" << endl;
                        nombrePoints += aLaSuite ;
                        cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                        afficheMatriceV2(mat);

                        cout << "POINTS :  " << nombrePoints  << endl;
                      sleep(1);

                        explosionUneBombeHorizontale(mat, i, temp, aLaSuite);
                        cout << string(20, '-') << endl << "Matrice après suppression" << endl;
                                    afficheMatriceV2(mat);}

                    else if (aLaSuite ==4){
                        cout << "On a une SUPER suite en position numLigne = " << i + 1
                             << "; colonne = " << temp + 1
                             << "; sur  " << aLaSuite << " cases" << endl;
                        nombrePoints += aLaSuite ;
                        cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                        afficheMatriceV2(mat);

                        cout << "POINTS :  " << nombrePoints  << endl;
                        sleep(1);

                        CMatrice nouvellematrice = superExplosionVerticale(mat, i);
                        mat = nouvellematrice;
                        cout << string(20, '-') << endl << "Matrice après suppression" << endl;
                                    afficheMatriceV2(mat);

                    }
                }
            }
        }
    }
    return auMoinsUneExplosion;
}

/**
 * @brief detectionExplosionUneBombeVerticale :fonction booleen qui détecte s'il y a une suite d'au moins 3 caractères égaux à la verticale
 * et apelle en conséquence la fonction pour casser le motif
 * @param mat : la matrice
 * @return : la fonction renvoie true si un motif est présent à la verticale dans la matrice
 */
bool detectionExplosionUneBombeVerticale(CMatrice &mat) {
    bool auMoinsUneExplosion = false;
    size_t caseAct ;
    size_t aLaSuite = 0;
    vector<size_t> tab;
    for (size_t colonne = 0; colonne < mat[0].size(); ++colonne) {
        aLaSuite = 0;
        for (size_t ligne = 0; ligne < mat.size(); ++ligne) {
            tab.push_back(mat[ligne][colonne]);}
        caseAct=tab[0];
        for (size_t i = 0; i < tab.size(); ++i) {
            if (caseAct != tab[i]) {
                caseAct = tab[i];
                aLaSuite = 1;
            } else {
                ++aLaSuite;
                if (aLaSuite >= 3 && (i == tab.size() - 1 || tab[i + 1] != caseAct)) {
                    auMoinsUneExplosion = true;
                    if (aLaSuite == 3){
                                    cout << "On a une suite en position numLigne = " << i % mat.size() - aLaSuite/2
                                         << "; colonne = " << colonne + 1
                                         << "; sur  " << aLaSuite << " cases" << endl;
                                    nombrePoints += aLaSuite;
                                    // Effectuer la suppression (appel à la fonction explositionUneBombeVerticale)
                                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                                    afficheMatriceV2(mat);
                                    cout << "POINTS :" << nombrePoints << endl;
                                    sleep(1);

                                    explosionUneBombeVerticale(mat, i % mat.size() - aLaSuite / 2 - 1, colonne, aLaSuite);
                                    cout << string(20, '-') << endl << "Matrice après suppression" << endl;
                                                afficheMatriceV2(mat);
                    }

                    else if (aLaSuite == 4){
                                    cout << "On a une SUPER suite en position numLigne = " << i % mat.size() - aLaSuite/2
                                         << "; colonne = " << colonne + 1
                                         << "; sur  " << aLaSuite << " cases" << endl;
                                    nombrePoints += aLaSuite;
                                    // Effectuer la suppression (appel à la fonction explositionUneBombeVerticale)
                                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                                    afficheMatriceV2(mat);
                                    cout << "POINTS : " << nombrePoints << endl;
                                    sleep(1);
                                    CMatrice nouvellematrice=superExplosionVerticale(mat, colonne);
                                    mat = nouvellematrice;
                                    cout << string(20, '-') << endl << "Matrice après suppression" << endl;
                                                afficheMatriceV2(mat);


                    }
                }
            }
        }
        // Effacer le vecteur pour la colonne suivante
        tab.clear();
    }
    return auMoinsUneExplosion;
}

/**
 * @brief destructionCascade : la fonction vérifie si il y a un motif présent dans la matrice, et temps qu'un motif est présent elle apelle les
 * détections qui ensuite apelleront les fonctions qui détruiront les motifs.
 * @param mat : la matrice
 * @return : la fonction renvoie la matrice apres modifications.
 */
CMatrice destructionCascade(CMatrice &mat){
    while (detectionExplosionUneBombeHorizontale(mat) /*||detectionExplosionUneBombeVerticale(mat)*/){
        detectionExplosionUneBombeHorizontale(mat);
        //detectionExplosionUneBombeVerticale(mat);
    }

    return mat;
}

/**
 * @brief estCoupJouable : la fonction vérifie si un coup est jouable c'est à dire si lors d'un changement de place de 2 caractères où les cases
 * sont collées un motif apparait, ça voudrait dire que le coup est jouable.
 * @param mat : la matrice
 * @param pair1 : la pair contenant la ligne et la colonne de la case initiale
 * @param ajoutLigne : la valeur de la ligne à ajouter pour arriver à la case avec laquelle la case initiale doit échanger sa valeur
 * @param ajoutColonne : la valeur de la colonne à ajouter pour arriver à la case avec laquelle la case initiale doit échanger sa valeur
 * @return
 */
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


/**
 * @brief faitUnMouvement : la fonction utilise un switch case pour pouvoir se déplacer dans la matrice (grille de jeu) et lorsqu'on appuie sur la touche
 * pour rentrer en mode inversion, elle sauvegarde la case initiale et demande un déplacement pour savoir avec quelle case elle échagera sa valeur
 * si le coup est jouabel , c'est-à-dire qu'il mène à la création d'un motif.
 * @param mat : la matrice
 * @return : la fonction renvoie true si un mouvement a été fait
 */
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







