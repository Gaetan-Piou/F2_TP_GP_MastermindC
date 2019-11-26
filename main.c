#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define NBCHIFFRE 4
#define TOURMAX 12

void initialisation(int TCode[NBCHIFFRE], int TNbTrouve[NBCHIFFRE]);
void AfficheNombre(int TCode[NBCHIFFRE], int TNbTrouve[NBCHIFFRE]);
void SaisieCode(int TSaisie[NBCHIFFRE][TOURMAX], int tourActu);
void AfficheSaisies(int TSaisie[NBCHIFFRE][TOURMAX], int TCode[NBCHIFFRE], int TNbTrouve[NBCHIFFRE], int tourActu);
void verifFin(int TSaisie[NBCHIFFRE][TOURMAX], int TCode[NBCHIFFRE], int tourActu, int *finPartie);
void PartieFinie(int resultat, int TCode[NBCHIFFRE], int nbtours);

void main() {
    srand(time(NULL));

    int TChiffre[NBCHIFFRE];            //Le code que le joueur doit deviner
    int TTrouve[NBCHIFFRE];             //Verifie si un chiffre du code a deja ete trouve
    int TPrec[NBCHIFFRE][TOURMAX];      //Les precedentes hypotheses de code du joueur
    int tourJeu = 0;                    //Le tour de jeu actuel
    int finPartie = 0;                  //Verifie si la partie est terminee ou non

    initialisation(TChiffre,TTrouve);

    //On continue la partie tant qu'on n'a pas atteint le nombre de tours maximum ou que le joueur n'a pas trouve le code
    while(finPartie == 0) {

        AfficheNombre(TChiffre,TTrouve);
        printf("\n");
        SaisieCode(TPrec,tourJeu);

        AfficheSaisies(TPrec,TChiffre,TTrouve,tourJeu);

        verifFin(TPrec,TChiffre,tourJeu,&finPartie);
        tourJeu++;
    }

    PartieFinie(finPartie,TChiffre,tourJeu);


}

void initialisation(int TCode[NBCHIFFRE], int TNbTrouve[NBCHIFFRE]) {
//BUT: Initialiser le code aleatoirement en evitant les doublons et le tableau des chiffres trouves
//ENTREE:
    //int TCode[]: Le code secret a generer aleatoirement
    //int TNbTrouve[]: Le tableau verifiant si un chiffre du code a deja ete trouve, a initialiser a 0
//SORTIE: Le code et le tableau des chiffres trouves initialises

    int i,j;
    int meme = 0;

    //Initialisation du code a deviner et de l'etat du chiffre (devine ou non)
    for(i=0; i<NBCHIFFRE; i++) {
        //Tant que le chiffre genere est identique a un autre chiffre du code, on regenere un autre chiffre
        do {
            TCode[i] = rand() % 10;
            meme = 0;
            for(j=0; j<i; j++) {
                if(TCode[j] == TCode[i]) {
                    meme = 1;
                }
            }
        } while(meme == 1);
        TNbTrouve[i] = 0;
    }

}


void AfficheNombre(int TCode[NBCHIFFRE], int TNbTrouve[NBCHIFFRE]) {
//BUT: Afficher les chiffres du code deja trouves
//ENTREE:
    //int TCode[]: Le code secret que l'utilisateur doit trouver
    //int TNbTrouve[]: Le tableau verifiant si un chiffre du code a deja ete trouve ulterieurement par l'utilisateur
//SORTIE: Une chaine de (NBCHIFFRE) chiffres affichant les chiffres deja trouves ou affichant un "-" pour les chiffres n'ayant pas encore ete trouves

    int i;
    for(i=0; i<NBCHIFFRE; i++) {
        if(TNbTrouve[i] == 1) {
            printf("%d",TCode[i]);
        } else {
            printf("-");
        }
    }
}


void SaisieCode(int TSaisie[NBCHIFFRE][TOURMAX], int tourActu) {
//BUT: Recevoir la saisie du code suppose par l'utilisateur
//ENTREE:
    //int TSaisie[][]: Le tableau contenant toutes les saisies precedentes et le numero du tour de jeu actuel
    //int tourActu: Le numero du tour de jeu actuel
//SORTIE: La saisie de l'utilisateur

    int i;
    int saisie;

    for(i=0; i<NBCHIFFRE; i++) {
        printf("Entrez le chiffre numero %d sur %d\n",i,NBCHIFFRE);
        fflush(stdin);
        scanf("%d",&saisie);
        while(saisie / 10 != 0) {
            printf("ERREUR! VEUILLEZ RECOMMENCER: Entrez le chiffre numero %d sur %d\n",i+1,NBCHIFFRE);
            fflush(stdin);
            scanf("%d",&saisie);
        }
        TSaisie[i][tourActu] = saisie;
    }
}


void AfficheSaisies(int TSaisie[NBCHIFFRE][TOURMAX], int TCode[NBCHIFFRE], int TNbTrouve[NBCHIFFRE], int tourActu) {
//BUT: Afficher toutes les saisies precedentes de l'utilisateur en couleur :
    //Rouge si le chiffre saisi est nul part, ou que le chiffre a deja ete saisi au bon endroit
    //Jaune si le chiffre saisi est dans le code, mais qu'il n'a pas ete saisi au bon endroit
    //Vert si le chiffre saisi est le bon au bon endroit.
//ENTREE:
    //int TSaisie[][]: Le tableau contenant toutes les saisies precedentes de l'utilisateur
    //int TCode[]: Le code secret que l'utilisateur doit trouver
    //int TNbTrouve[]: Le tableau verifiant si un chiffre du code a deja ete trouve ulterieurement par l'utilisateur
    //int tourActu: Le numero du tour de jeu actuel
//SORTIE: Les differentes hypotheses de code du joueur depuis le debut de la partie, affichees avec le code couleur decrit precedemment

    int i,j,k;
    int appartient;

    system("cls");
    for(k=0; k<=tourActu; k++) {
        for(i=0; i<NBCHIFFRE; i++) {
            //On verifie si le chiffre suppose par l'utilisateur est le meme que celui dans le code, au bon endroit
            if(TSaisie[i][k] == TCode[i]) {
                TNbTrouve[i] = 1;
                printf("\033[0;32m");
            } else {
                appartient = 0;
                //On verifie si le chiffre suppose par l'utilisateur se trouve dans le code et que l'utilisateur ne l'a pas deja trouve
                for(j=0; j<NBCHIFFRE; j++) {
                    if(TSaisie[i][k] == TCode[j] && TSaisie[j][k] != TCode[j]) {
                        appartient = 1;
                    }
                }
                if(appartient == 1) {
                    printf("\033[0;33m");
                } else {
                    printf("\033[0;31m");
                }
            }
            printf("%d",TSaisie[i][k]);
            printf("\033[0m");
        }
        printf("\n");
    }
}


void verifFin(int TSaisie[NBCHIFFRE][TOURMAX], int TCode[NBCHIFFRE], int tourActu, int *finPartie) {
//BUT: Verifier si la partie est finie: soit car le joueur a trouve le code, soit car le nombre de tours est epuise.
//ENTREE:
    //int TSaisie[][]: Le tableau contenant toutes les saisies precedentes de l'utilisateur
    //int TCode[]: Le code secret que l'utilisateur doit trouver
    //int tourActu: Le numero du tour de jeu actuel
    //int *finPartie: Verifie si la partie est finie:
//SORTIE: On renvoie finPartie en pointeur:
    //Si finPartie=0, la partie n'est pas terminee.
    //Si finPartie=1, la partie est terminee car le joueur a trouve le bon code.
    //Si finPartie=-1, la partie est terminee car le nombre de tours est epuise.

    int nbTrouves = 0;
    int i;

    for(i=0; i<NBCHIFFRE; i++) {
        if(TSaisie[i][tourActu] == TCode[i]) {
            nbTrouves++;
        }
    }

    if(nbTrouves == NBCHIFFRE) {
        *finPartie = 1;
    } else {
        if(tourActu == TOURMAX) {
            *finPartie = -1;
        }
    }
}




void PartieFinie(int resultat, int TCode[NBCHIFFRE], int nbtours) {
//BUT: Afficher le message de fin de partie en fonction du resultat du joueur
//ENTREE:
    //int resultat: Le resultat du joueur
        //resultat=-1 si le joueur a perdu
        //resultat=1 si le joueur a gagne
    //int TCode[]: Le code que le joueur devait deviner
    //int nbtours: Le nombre de tours qui se sont epuises avant que la partie ne se termine
//SORTIE: Un message au joueur en fonction de s'il a gagne ou non.

    if(resultat == 1) {
        printf("Bravo ! Tu as trouve le bon code en %d tours !",nbtours);
    } else {
        printf("Perdu... Le resultat etait %d%d%d%d",TCode[0],TCode[1],TCode[2],TCode[3]);
    }
}
