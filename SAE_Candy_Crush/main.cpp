#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <sstream>

#include <type.h>
#include <cst.h>
#include <affichage.h>

using namespace std;

char haut ('z');
char bas('s');
char gauche ('q');
char droite ('d');
char inv ('i');

/**
 * @brief actualiserTouche : la fonction actualise les touches du joueur en fonction du fichier config.yaml si le joueur a changé les touhes.
 */
void actualiserTouche() {
    //Touches par défaut
    const string fichier = "../SAE_Candy_Crush/config.yaml";
    ifstream ifs(fichier);

    if (ifs.is_open()) {
        unordered_map<string, string> config;

        string line;
        while (getline(ifs, line)) {
            istringstream iss(line);
            string key, value;
            char colon;

            if (iss >> key >> colon >> value) {
                config[key] = value;
            } else {
                cerr << "Error parsing line: " << line << endl;
            }
        }

        haut = config["haut"].empty() ? haut : config["haut"][0];
        bas = config["bas"].empty() ? bas : config["bas"][0];
        gauche = config["gauche"].empty() ? gauche : config["gauche"][0];
        droite = config["droite"].empty() ? droite : config["droite"][0];
        inv = config["inversion"].empty() ? inv : config["inversion"][0];

        ifs.close();
    } else {
        cerr << "Unable to open file: " << fichier << endl;
    }
}


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
 * @return : renvoie la matrice après modification
 */
CMatrice superExplosionVerticale(CMatrice mat, const size_t &colonne){
    for (size_t i=0 ; i < mat.size() ; ++i) {
        mat[i][colonne] =rand() % KPlusGrandNombreDansLaMatrice +1;
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
    for (size_t i = numLigne ; i < numLigne+aLaSuite; ++i){


        for (size_t j = 1 ; j < nombreAFaireDescendre+1 ; ++j){

            mat[numLigne+aLaSuite-j][numColonne] = mat[nombreAFaireDescendre-j][numColonne];

        }
    }
    for (size_t x = 0; x < aLaSuite; ++x){
        mat[x][numColonne] = rand() % KPlusGrandNombreDansLaMatrice +1;
    }
}
bool partieCommence = false;
size_t score = 0;
size_t nombreEssaisRestants = 5;

/**
 * @brief detectionExplosionUneBombeVerticale :fonction booleen qui détecte s'il y a une suite d'au moins 3 caractères égaux à l'horizontale
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

                    cout << "On a une suite en position numLigne = " << i + 1
                         << "; colonne = " << temp + 1
                         << "; sur  " << aLaSuite << " cases" << endl << endl;
                    if (partieCommence == true){
                        score += aLaSuite ;

                        sleep(1);
                    }
                    else {
                        cout << "CHARGEMENT...."<< endl;
                    }

                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                    afficheMatriceV2(mat);
                    if(partieCommence == true){
                        sleep(1);
                    }



                    explosionUneBombeHorizontale(mat, i, temp, aLaSuite);
                    cout << string(20, '-') << endl << "Matrice après suppression" << endl;
                    afficheMatriceV2(mat);}

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
                             << "; sur  " << aLaSuite << " cases" << endl << endl;
                        if (partieCommence == true){
                            score+=aLaSuite;

                            sleep(1);
                        }
                        else {
                            cout << "CHARGEMENT... "<< endl;
                        }


                        // Effectuer la suppression (appel à la fonction explositionUneBombeVerticale)
                        cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                        afficheMatriceV2(mat);
                        if (partieCommence == true){
                            sleep(1);
                        }

                        explosionUneBombeVerticale(mat, i % mat.size() - aLaSuite / 2 - 1, colonne, aLaSuite);
                        cout << string(20, '-') << endl << "Matrice après suppression" << endl;
                        afficheMatriceV2(mat);
                    }

                    else if (aLaSuite >= 4){
                        if (partieCommence == true){
                            score += mat.size();
                            cout << "POINTS : " << score << endl;
                        }
                        else {
                            cout << "CHARGEMENT...." << endl;
                        }
                        cout << "On a une SUPER suite en position numLigne = " << i % mat.size() - aLaSuite/2
                             << "; colonne = " << colonne + 1
                             << "; sur  " << aLaSuite << " cases" << endl << endl;

                        // Effectuer la suppression (appel à la fonction explositionUneBombeVerticale)
                        cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                        afficheMatriceV2(mat);
                        if (partieCommence== true){
                            sleep(1);
                        }
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
 * @brief detectionExplosionUneBombeHorizontaleVERIFICATION : La fonction fait comme l'autre détection horizontale elle vérifie si une suite est présente
 * mais sans afficher les matrices.
 * @param mat : la matrice
 * @return : true si une suite est présente
 */
bool detectionExplosionUneBombeHorizontaleVERIFICATION(CMatrice &mat) {
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

                }
            }
        }

    }
    return auMoinsUneExplosion;
}
/**
 * @brief detectionExplosionUneBombeVerticaleVERIFICATION
 * @param mat
 * @return
 */
bool detectionExplosionUneBombeVerticaleVERIFICATION(CMatrice &mat) {
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

                }

            }

        }

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
    while (detectionExplosionUneBombeHorizontale(mat) ||detectionExplosionUneBombeVerticale(mat)){

        detectionExplosionUneBombeHorizontale(mat);
        detectionExplosionUneBombeVerticale(mat);
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
bool estCoupJouable(CMatrice mat,CInt pair1, size_t ajoutLigne,size_t ajoutColonne ){
    bool jouable = false ;
    CMatrice mat2 = mat;
    swap (mat2[pair1.first][pair1.second], mat2[pair1.first+ajoutLigne][pair1.second+ajoutColonne]);
    if (detectionExplosionUneBombeHorizontaleVERIFICATION(mat2) || detectionExplosionUneBombeVerticaleVERIFICATION(mat2)){
        jouable = true ;
    }
    return jouable;
}




//Ces deux variables sont déclarées en dehors de la fonction, puisque sinon elles serait réinitialisés à chaque itération de la fonction
size_t ligneAct = 2;
size_t colonneAct =2;


/**
 * @brief faitUnMouvement : la fonction utilise des conditions pour pouvoir se déplacer dans la matrice (grille de jeu) et lorsqu'on
 * appuie sur la touche pour rentrer en mode inversion, elle sauvegarde la case initiale et demande un déplacement pour savoir avec quelle case elle échagera sa valeur
 * si le coup est jouabel , c'est-à-dire qu'il mène à la création d'un motif.
 * @param mat : la matrice
 * @return : la fonction renvoie true si un mouvement a été fait
 */
bool faitUnMouvement(CMatrice & mat, char haut, char gauche, char bas, char droite, char inv) {
    bool premierMouvement = false;
    char deplacement;
    char inversion;

    cout << "POINTS :  " << score  << endl;
    cout << "Nombres d'essais restant : " << nombreEssaisRestants << endl;
    cout << "Veuillez faire un mouvement" << endl;
    cin >> deplacement;

    bool deplacementValide = true;
    char deplacementLowerCase = tolower(deplacement);

    if (deplacementLowerCase == haut) {
        cout << "Déplacement vers le haut demandé." << endl;
        --ligneAct;
        premierMouvement = true;
    } else if (deplacementLowerCase == gauche) {
        cout << "Déplacement vers la gauche demandé." << endl;
        --colonneAct;
        premierMouvement = true;
    } else if (deplacementLowerCase == bas) {
        cout << "Déplacement vers le bas demandé." << endl;
        ++ligneAct;
        premierMouvement = true;
    } else if (deplacementLowerCase == droite) {
        cout << "Déplacement vers la droite demandé." << endl;
        ++colonneAct;
        premierMouvement = true;
    } else if (deplacementLowerCase == inv) {
        CInt position = make_pair(ligneAct, colonneAct);
        cout << "VOUS ETES EN MODE INVERSION" << endl << "Entrez " << haut << ", " << gauche << ", " << bas << ", " << droite << " pour intervertir la case actuelle avec celle du dessus, de gauche, de droite, ou d'en bas" << endl;
        cin >> inversion;
        switch (inversion) {
        case 'z':{
            if (estCoupJouable(mat, position, -1, 0)){
                swap(mat[ligneAct][colonneAct], mat[ligneAct-1][colonneAct]);
                --nombreEssaisRestants;
            } else {
                cout << "CE DEPLACEMENT N'EST PAS POSSIBLE !!!!" << endl;
            }
            break;
        }
        case 'q':{
            if (estCoupJouable(mat, position, 0, -1)){
                swap(mat[ligneAct][colonneAct], mat[ligneAct][colonneAct-1]);
                --nombreEssaisRestants;
            } else {
                cout << "CE DEPLACEMENT N'EST PAS POSSIBLE !!!!" << endl;
            }
            break;
        }
        case 's':{
            if (estCoupJouable(mat, position, 1, 0)){
                swap(mat[ligneAct][colonneAct], mat[ligneAct+1][colonneAct]);
                --nombreEssaisRestants;
            } else {
                cout << "CE DEPLACEMENT N'EST PAS POSSIBLE !!!!" << endl;
            }
            break;
        }
        case 'd':{
            if (estCoupJouable(mat, position, 0, 1)){
                swap(mat[ligneAct][colonneAct], mat[ligneAct][colonneAct+1]);
                --nombreEssaisRestants;
            } else {
                cout << "CE DEPLACEMENT N'EST PAS POSSIBLE !!!!" << endl;
            }
            break;
        }
        default:
            break;
        }
    } else {
        cout << "Choisissez " << haut << ", " << gauche << ", " << bas << ", " << droite << endl;
    }

    if (ligneAct < 0 || ligneAct > mat.size()-1 || colonneAct < 0 || colonneAct > mat.size()-1) {
        deplacementValide = false;
    }
    if (deplacementValide == true){
        afficheMatriceVSwitch(mat, ligneAct, colonneAct);
    } else {
        cout << "Ce déplacement n'est pas possible, vous allez sortir de la grille" << endl << endl << endl << "Votre position a été réinitialisée au milieu de la matrice" << endl;
        colonneAct = mat.size()/2 ;
        ligneAct = mat.size()/2;
    }
    return premierMouvement;
}


/**
 * @brief testSwitch : la fonction nous dit de faire un mouvement et apelle faitUnMouvement.
 * @param mat : la matrice
 */
void testSwitch(CMatrice &mat)
{
    cout << "Entrez, z (haut) , q (gauche), s (bas) , d (droite) Pour vous déplacer " << endl << endl << "Pour passer en mode inversion de case entrer I" << endl << endl;
    faitUnMouvement(mat, haut, gauche, bas, droite, inv);
}
/**
 * @brief choixNiveau : la fonction nous demande de choisir le niveau de difficulté auquel on veut jouer (1 pour facile, 2 pour moyen et 3 pour difficile).
 * @return : renvoie la matrice .
 */
CMatrice choixNiveau(){
    size_t niveau;
    cout << "Choisissez le niveau, 1 pour facile , 2 pour moyen et 3 pour difficile" << endl;
    cin >> niveau ;
    CMatrice mat ;
    while (niveau!=1 && niveau!=2 && niveau!=3){
        cout << "1 , 2 ou 3 please" << endl;
        cin >> niveau;
    }
    if (niveau == 1){
        initMat(mat,5,5,20);
        nombreEssaisRestants = 15;
    }
    else if (niveau == 2){
        initMat(mat, 10,10,20);
    }
    else{

        initMat(mat,10,10,20);
        nombreEssaisRestants = 3;
    }

    return mat;
}

/**
 * @brief partieModeNiveau : Cette fonction simule une partie dans ce mode de jeu ou le joueur doit faire + de 50 points.
 * @param mat : la matrice
 */
void partieModeNiveau(CMatrice & mat) {
    bool partieGagné = false;
    while (true) {
        destructionCascade(mat);
        partieCommence = true;

        if (score >= 50){
            partieGagné = true;
            break;
        }
        if (nombreEssaisRestants == 0){
            break;
        }
        testSwitch(mat);
    }
    system("clear");
    if(partieGagné == true){
        cout << "Niveau Terminé BRAVO " << endl;
    }
    else {
        cout <<" GAME OVER " << endl;
    }
}

/**
 * @brief modeNiveau : la fonction apelle la fonction qui permet de choisir le niveau de difficulté et initie la matrice en conséquence
 * puis apelle la fonction qui simule la partie.
 */
void modeNiveau(){
    CMatrice choixJoueur = choixNiveau();
    partieModeNiveau(choixJoueur);

}
/**
 * @brief modeArcade : la fonction lance le mode de jeu arcade, une partie avec un timer de 2 minutes (120 secondes) ou le joueur doit faire le plus
 * de points en 2 mins.
 */
void modeArcade(){

    CMatrice  mat;
    initMat(mat,10,10,20);
    chrono::seconds dureeMinuteur(120);
    auto debutMinuteur = chrono::high_resolution_clock::now();
    while (true) {
        auto maintenant = chrono::high_resolution_clock::now();
        auto dureePasse = chrono::duration_cast<chrono::seconds>(maintenant - debutMinuteur);

        auto tempsRestant = dureeMinuteur - dureePasse;
        auto minutesRestantes = chrono::duration_cast<chrono::minutes>(tempsRestant);
        auto secondesRestantes = chrono::duration_cast<chrono::seconds>(tempsRestant - minutesRestantes);
        destructionCascade(mat);
        partieCommence = true;
        testSwitch(mat);
        cout << "Temps restant : " << minutesRestantes.count() << " minutes et "
             << secondesRestantes.count() << " secondes." <<
            endl;
        if (dureePasse >= dureeMinuteur) {
            cout << "Minuteur de 1 minute écoulé." << endl;
            cout << "Score final : " << score << endl;
            break;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}

/**
 * @brief modeCreatif : la fonction sert à jouer au mode créatif, le mode ou on choisit les règles de son propre jeu avec un temps, un nombre
 * d'essai, la taille de la grille de jeu et le nombre de points minimum pour gagner.
 */
void modeCreatif (){

    size_t choixTaille;
    CMatrice matriceJoueur;
    size_t objectifPoints;

    cout << "Vous allez ici pouvoir créer votre propre plateau de jeu et y jouer ! " << endl;

    cout << "Commencez par choisir le nombre d'essais que vous voulez" << endl;
    cin >> nombreEssaisRestants;
    cout << "Désormais choisissez la taille de la matrice (le maximum est 10)" << endl;
    cin >> choixTaille;
    initMat(matriceJoueur,choixTaille,choixTaille,5);
    cout << "Combien de points faut il atteindre pour gagner ?" << endl;
    cin >> objectifPoints;


    cout << "La partie peut désormais débuter ! " << endl;

    bool partieGagné = false;
    while (true) {


        destructionCascade(matriceJoueur);
        partieCommence = true;

        if (score >= objectifPoints){
            partieGagné = true;
            break;
        }
        if (nombreEssaisRestants == 0){
            break;
        }
        testSwitch(matriceJoueur);
    }
    system("clear");
    if(partieGagné == true){
        cout << "Niveau Terminé BRAVO " << endl;
    }
    else {
        cout <<" GAME OVER " << endl;

    }
}

/**
 * @brief menuPrincipal : Le menu principal affiché lors du début de la partie dans lequel on choisit le mode de jeu auquel on souhaite jouer.
 */
void menuPrincipal(){
    cout << "BIENVENUE DANS CE MAGNIFIQUE JEU CANDY CRUSH" << endl << endl;

    cout <<"Choisissez quel mode de jeu souhaitez vous jouer" << endl;

    cout << "ARCADE : Vous avez une minute pour faire le plus de points " << endl << endl;

    cout << "NIVEAUX : Atteignez un certain nombre de points avec un certains nombres d'essais" << endl << endl;

    cout << "CREATION : Créez votre propre niveau et jouez y autant que vous le voulez" << endl << endl;

    cout << "Entrez 1(ARCADE), 2(NIVEAUX) ou 3(CREATIF) pour choisir le mode de jeu" << endl;
    size_t choix = 0;
    cin >> choix;

    while (choix  !=1 && choix!= 2 && choix!= 3){
        cout << "1,2 ou 3 pour choisir le mode" << endl;
        cin >> choix;
    }
    if (choix == 1 ){
        modeArcade();

    }
    else if(choix == 2){
        modeNiveau();
    }
    else {
        modeCreatif();
    }
}

int main() {
    actualiserTouche();
    menuPrincipal();
}
