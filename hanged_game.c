#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*Appel des fonctions générales*/
int freeBuffer();
int pseudoPlayer();
int readText();
int processingUnderscore();
int prezAlphabet();
int prezProposal();
int prezLetter();
int WinOrLose();
int drawHanged();
int TimeToGame();

/*Constantes*/
#define LENGTH_WORD_MAX 20
#define LENGTH_PSEUDO 15
#define ATTEMPTS_MAX 8
#define true 1
#define false 0

/*Tableaux*/
char tabPseudo[LENGTH_PSEUDO];      /*Tableau de stockage du pseudo*/
char tabWordGuess[LENGTH_WORD_MAX];     /*Tableau de stockage du mot à deviner récupérer dans le fichier texte*/
char tabUnderscore[LENGTH_WORD_MAX];        /*Tableau de stockage/affichage pour le joueur*/
char tabProposal[LENGTH_WORD_MAX];      /*Tableau de stockage des lettres proposées*/

/*Variables globales*/
char pseudo_recovery = 0;       /*Variable de sauvegarde du pseudo du joueur*/
char letter_proposal = 0;       /*Variable de stockage de la lettre proposée par le joueur*/
char letter_actual = 0;     /*Variable de stockage des lettres du mot à deviner*/
int state_of_proposal = 0;      /*Etat de la lettre, si elle a déjà été proposée ou non*/
int nb_letters_in_word = 0;     /*Variable pour compter le nombre de lettre du mot à deviner*/
int nb_letters_proposal = 0;        /*Variable de déplacement dans le tableau de sauvegarde des propositions*/
int nb_letters_finds = 0;       /*Variable qui compte le nombre de lettres trouvées par le joueur dans le mot*/

/*Structure de jeu*/        /*Valérie m'a dit*/
struct game {
    int state_of_game;      /*Etat des tentatives*/
    int attempts;       /*Etat du jeu en fonction du nombre de tentatives ou lettres trouvées*/
};

/*Définition des variables de structure*/
struct game hanged = {0, 0};

int main() {

    srand(time(NULL));      /*Permet de choisir un mot aléatoirement*/
    readText();     /*Lis le fichier texte*/
    pseudoPlayer();     /*Demande le nom du joueur*/
    processingUnderscore();     /*Transforme le mot à deviner en underscore pour le joueur*/

    while (hanged.state_of_game == false) {        /*Tant que le jeu est initialisé à 0 on continue la boucle*/
        printf("Quelle lettre veux-tu me proposer ? \n");       /*Permet de redemander une lettre à chaque boucle*/
        TimeToGame();       /*Fonction qui contient les trois étapes du jeu et le changement d'état*/
    } 
}

int pseudoPlayer() {
    int i = 0;

    printf("Quel est ton pseudo ? \n");
    while ((pseudo_recovery = getchar()) != '\n') {     /*Tant que la variable n'a pas trouvé le signe de saut à la ligne*/
        tabPseudo[i] = pseudo_recovery;     /*Met les lettres dans chaque case du tableau*/
        i++;        /*Déplace l'indice d'indexation dans le tableau de stockage*/
    }
    printf("Tu vas jouer avec le pseudo %s ! \n", tabPseudo);
}

int readText() {
    FILE* fichier = NULL;
    int line_rand = rand()%835;     /*Détermine une ligne au hasard dans le fichier*/
    int i = 0;

    fichier = fopen("words.txt", "r");      /*Ouvre le fichier texte*/
    for (i; i < line_rand; i++) {       /*Indique le nombre de tours pour arriver jusqu'à la ligne aléatoire, qui sera gardée en mémoire*/
        fgets(tabWordGuess, LENGTH_WORD_MAX, fichier);      /*Récupère et stocke le mot*/
    }
    printf("Le mot choisi aléatoirement est : %s. \n", tabWordGuess);
}

int processingUnderscore() {
    letter_actual = tabWordGuess[nb_letters_in_word];       /*Récupère la première lettre*/

    while ((letter_actual != '\0') && (letter_actual != '\n')) {        /*Ne sort pas du mot tant qu'il n'y a pas une fin*/
        tabUnderscore[nb_letters_in_word] = '_';        /*Indique un underscore dans le tableau de stockage*/
        nb_letters_in_word++;       /*Augmente le nombre de lettres*/
        letter_actual = tabWordGuess[nb_letters_in_word];       /*Déplace l'indice d'indexation*/
    }
    printf("Le mot que tu dois trouver est : %s, il contient %d lettres, bonne chance %s. \n \n",tabUnderscore,nb_letters_in_word,tabPseudo);
}

int TimeToGame() {      /*Fonction de jeu*/
    prezAlphabet();     /*Vérification de l'alphabet*/
    prezProposal();     /*Vérification si déjà proposée*/
    prezLetter();       /*Vérification si lettre dans le mot*/
}

int prezAlphabet() {
    int state_of_letter = 0;        /*Etat de la lettre*/

    while (state_of_letter == false) {      /*Boucle tant que l'état est à 0*/
        letter_proposal = getchar();        /*Récupère la lettre pour la stockée dans la variable*/
        freeBuffer();       /*Je vide la mémoire pour ne pas prendre le saut à laligne*/
        if ((letter_proposal >= 97) && (letter_proposal <= 122)) {      /*Je vérifie que le code de la lettre est bien compris dans les minuscules*/
            state_of_letter = true;      /*Si oui, je change l'état de la lettre pour quitter la boucle*/
        } else {        /*Si la lettre n'est pas comprise dans les codes ASCII concerné, on redemande une lettre*/
            printf("Tu dois proposer un caractère de l'alphabet, je te laisse en proposer un autre. \n \n");
        }
    }
}

int prezProposal() {        /*Vérifie que la lettre n'a pas déjà été proposée par le joueur*/
    int i = 0;

    while ((i < nb_letters_proposal) && (state_of_proposal == false)) {     /*Tant que i est < au nombre de lettres et que l'état est à 0*/
        if (letter_proposal == tabProposal[i]) {        /*Si la lettre proposée est présente dans la première case*/
            state_of_proposal = true;        /*On change l'état pour arrêter la boucle*/
            nb_letters_finds--;
            printf("Tu as déjà utilisé la lettre %c \n \n", letter_proposal);
        }
        i++;        /*Dans le cas où la lettre ne correspond pas à la première case, on augment l'index*/
    }
    if (state_of_proposal == false) {       /*Si l'état n'a pas changé : donc pas de doublon*/
    nb_letters_proposal++;      /*On augment le nombre de lettres proposées pour le premier while*/
    tabProposal[i] = letter_proposal;       /*On affecte la lettre proposée dans le tableau*/
    }
}

int prezLetter() {      /*Vérification de la lettre présente dans le mot*/
    int i = 0, statuts = 0;

    while (tabWordGuess[i] != '\0') {       /*Tant qu'on est pas arrivé au bout du tableau qui contient le mot*/
        if (tabWordGuess[i] == letter_proposal) {       /*Si la lettre proposée est contenu dans la case vérifiée*/
            nb_letters_finds++;     /*On augment le nombre de lettres trouvées pour modifier l'état du jeu*/
            statuts = true;      /*On change le statut pour pouvoir donner les conséquences de cette proposition*/
            tabUnderscore[i] = letter_proposal;     /*On remplace l'underscore par la lettre proposée*/
        }
        i++;        /*On augmente l'indice d'indexation pour se déplacer dans le tableau de stockage et d'underscore*/
    }
/*Une fois le tour du tableau du mot à deviner fait*/
    if ((statuts != false) && (state_of_proposal == false)) {           /*Si le statut est différent de 0 mais qu'il n'y a pas de doublons dans les propositions*/ 
        printf("La lettre %c est présente dans le mot %d fois.\nTrouve encore %s. \n \n", letter_proposal, statuts, tabUnderscore);     /*C'est ok, bien joué*/
    } else {        /*Dans le cas où une des conditions n'est pas respectée*/
        if (state_of_proposal == false) {       /*Dans le cas où ce n'était pas un double et que la lettre n'était pas dans le mot à deviner*/
        hanged.attempts++;     /*On augmente le nombre de tentatives*/
        drawHanged();       /*On dessine l'ASCII concerné*/
        printf("La lettre %c n'est pas dans le mot, il te reste %d tentative(s). \n \n", letter_proposal, (ATTEMPTS_MAX - hanged.attempts));   /*On indique les tenta restantes*/
        }
    }
    state_of_proposal = 0;      /*On repasse l'état de la vérification des doublons à 0*/
    WinOrLose();
}

int WinOrLose() {       /*Changement de l'état du jeu*/

    if (hanged.attempts == ATTEMPTS_MAX) {     /*Dans le cas où on atteint le nombre maximum de tentatives*/
        hanged.state_of_game = true;      /*Changement d'état et on quitte le jeu*/
        printf("Tu n'as pas trouvé le mot %s %s, relance le jeu pour faire un meilleur score !\n", tabWordGuess, tabPseudo);
    }

    if ((hanged.attempts < ATTEMPTS_MAX) && (nb_letters_in_word == nb_letters_finds)) {        /*Moins du nbre de tenta autorisées mais bon nbre de lettres trouvées*/
        hanged.state_of_game = true;      /*Changement d'état et on quitte le jeu*/
        printf("Tu as gagné en %d tentative(s), le mot éait bien %s. Bien joué %s, tu peux relancer le jeu pour faire un meilleur score !\n", hanged.attempts, tabUnderscore, tabPseudo);
    }
}

int freeBuffer() {

    int c;
    c = 0;

    while (c != EOF && c != '\n') {
        c = getchar();
    }
}

int drawHanged() {

    if (hanged.attempts == 1) {
        printf(" _ _\n");
        printf("|   |______\n");
        printf("|          |\n");
        printf("|__________|\n");
    }

    if (hanged.attempts == 2) {
        printf("  | \n");
        printf(" _|_\n");
        printf("|   |______\n");
        printf("|          |\n");
        printf("|__________|\n");
    }

    if (hanged.attempts == 3) {
        printf("  |\n");
        printf("  |\n");       
        printf("  |\n");
        printf("  |\n");       
        printf(" _|_\n");
        printf("|   |______\n");
        printf("|          |\n");
        printf("|__________|\n");
    }

    if (hanged.attempts == 4) {
        printf("   ____\n");
        printf("  | \n");
        printf("  | \n");
        printf("  | \n");       /*On met deux antislash, puis l'antislash n pour afficher un bras*/
        printf("  | \n");
        printf("  | \n");       /*On met deux antislash, puis l'antislash n pour afficher la jambe*/
        printf(" _|_\n");
        printf("|   |______\n");
        printf("|          |\n");
        printf("|__________|\n");
    }

    if (hanged.attempts == 5) {
        printf("   ____\n");
        printf("  |    | \n");
        printf("  |\n");
        printf("  |\n");       /*On met deux antislash, puis l'antislash n pour afficher un bras*/
        printf("  |\n");
        printf("  |\n");       /*On met deux antislash, puis l'antislash n pour afficher la jambe*/
        printf(" _|_\n");
        printf("|   |______\n");
        printf("|          |\n");
        printf("|__________|\n");

    }

    if (hanged.attempts == 6) {
        printf("   ____\n");
        printf("  |    | \n");
        printf("  |    o\n");
        printf("  |\n");       /*On met deux antislash, puis l'antislash n pour afficher un bras*/
        printf("  |\n");
        printf("  |\n");       /*On met deux antislash, puis l'antislash n pour afficher la jambe*/
        printf(" _|_\n");
        printf("|   |______\n");
        printf("|          |\n");
        printf("|__________|\n");
    }

    if (hanged.attempts == 7) {
        printf("   ____\n");
        printf("  |    | \n");
        printf("  |    o\n");
        printf("  |   /|\\  \n");       /*On met deux antislash, puis l'antislash n pour afficher un bras*/
        printf("  |\n");
        printf("  |\n");       /*On met deux antislash, puis l'antislash n pour afficher la jambe*/
        printf(" _|_\n");
        printf("|   |______\n");
        printf("|          |\n");
        printf("|__________|\n");
    }

    if (hanged.attempts == 8) {
        printf("   ____\n");
        printf("  |    | \n");
        printf("  |    o\n");
        printf("  |   /|\\  \n");       /*On met deux antislash, puis l'antislash n pour afficher un bras*/
        printf("  |    |\n");
        printf("  |   / \\  \n");       /*On met deux antislash, puis l'antislash n pour afficher la jambe*/
        printf(" _|_\n");
        printf("|   |______\n");
        printf("|          |\n");
        printf("|__________|\n");
    }
}