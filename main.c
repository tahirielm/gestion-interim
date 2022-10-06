
/*-------------------------Declaration des directives--------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

/*-------------------------Declaration des constantes--------------------------*/

#define CMAX 256                             //Taille maximale (pour les chaines de charactères)
#define CMD_CLEAR "cls"                      //cls pour Windows et clear pour Linux
#define MDP_ADMIN "manPower2020"             //Mot de passe pour accéder au menu principale
#define FILE_TEMP "temp.bin"                 //Fichier temporaire
#define FILE_CANDIDATS "candidats.bin"       //Fichier binaire pour stocker les candidats
#define FILE_OFFRES "offres.bin"             //Fichier binaire pour stocker les offres
#define FILE_SPECIALITES "specialites.bin"   //Fichier binaire pour stocker les specialites
#define FILE_ENTREPRISES "entreprises.bin"   //Fichier binaire pour stocker les entreprises
#define FILE_RECRUTEMENTS "recrutements.bin" //Fichier binaire pour stocker les recrutements

/*-------------------------Declaration des structures--------------------------*/

typedef struct Date{
    int jour;
    int mois;
    int annee;
} Date;

typedef struct Specialite{
    int specialite_Index;
    char specialite_Nom[CMAX];
} Specialite;

typedef struct Candidat{
   int ca_Index;
   int dispo; //0 : Non disponible, 1:Disponible
   char ca_Nom[CMAX];
   char ca_Prenom[CMAX];
   char ca_Ville[CMAX];
   char ca_Addresse[CMAX];
   Specialite sp1;
   Specialite sp2;
} Candidat;

typedef struct Entreprise{
    int entrep_Index;
    char entrep_Nom[CMAX];
    char entrep_Ville[CMAX];
} Entreprise;

typedef struct Offre{
    int offre_Index;
    int offre_Salaire;
    int dispo; //0 : Non disponible, 1:Disponible
    char offre_Intitule[CMAX];
    Entreprise e;
    Specialite sp;
    Date date_Debut;
} Offre;

typedef struct Recrutement{
    int rec_Index;
    int rec_Actif; //0: Non actif, 1: Actif
    char type_Recrutement[5];
    Candidat c;
    Offre o;
    Date date_Fin;
}Recrutement;

/*-------------------------Prototype des fonctions--------------------------*/

void menu();
void menu_Gestion_Candidats();
void menu_Gestion_Offres();
void menu_Gestion_Recrutements();
int nb_Candidats();
int nb_Offres();
int nb_Recrutements();
int nb_Entreprises();
int nb_Specialites();
void afficher_Candidats(Candidat* tab, int taille, int mode);
void tableau_Candidats(Candidat* c, int taille);
int index_Prochain_Candidat();
Candidat creer_Candidat(int id);
void ajout_Candidat();
void modifier_Candidat();
void tableau_Offres(Offre* tab, int taille);

/*--------------------------Gestion Des dates--------------------------*/

Date char_to_date(char ligne[CMAX]){
    Date d;
    char* ch = strtok(ligne, "/");
    d.jour = atoi(ch);
    ch = strtok(NULL, "/");
    d.mois = atoi(ch);
    ch = strtok(NULL, "/");
    d.annee = atoi(ch);
    return d;
}

int cmp_Date(Date d1, Date d2){
    /*
    Cette fonction retourne : 
        1 si d1 superieure a d2 (d1 est plus recente)
        -1 sinon
        0 si les deux dates sont les memes
    */
    if( d1.annee > d2.annee ){
        return 1;
    }
    else if( d1.annee < d2.annee ){
        return -1;
    }
    else{
        if( d1.mois > d2.mois ){
            return 1;
        }
        else if( d1.mois < d2.mois ){
            return -1;
        }
        else{
            if( d1.jour > d2.jour ){
                return 1;
            }
            else if( d1.jour < d2.jour ){
                return -1;
            }
            else{
                return 0;
            }
        }
    }
}

/*--------------------------Gestion Specialites--------------------------*/

int nb_Specialites(){
    FILE* pt = NULL;
    pt = fopen(FILE_SPECIALITES, "rb+");
    int i=0;
    Specialite s;
    if (pt != NULL){
        while( fread(&s, sizeof(Specialite), 1, pt) != 0 ){
            i++;
        }
        fclose(pt);
        return i;
    }
}

void tableau_Specialites(Specialite* tab, int n){
    FILE* pt = NULL;
    pt = fopen(FILE_SPECIALITES, "rb");
    if(pt != NULL){
        fread(tab, sizeof(Specialite), n, pt);
        fclose(pt);
    }
}

void afficher_Specialites(Specialite* tab, int taille){
    int i;
    for(i=0 ; i<taille ; i++){
        printf("%d - %s\n", tab[i].specialite_Index, tab[i].specialite_Nom);
    }
}

int index_Prochaine_Specialite(){
    FILE* pt = NULL;
    pt = fopen(FILE_SPECIALITES, "rb+");
    if(pt != NULL){
        int n;
        n = nb_Specialites();
        if(n == 0){
            return 1;
        }
        else{
            fseek(pt, (n-1)*sizeof(Specialite), SEEK_CUR);
            Specialite s;
            fread(&s, sizeof(Specialite), 1, pt);
            fclose(pt);
            return s.specialite_Index + 1;
        }
    }
}

void ajout_Specialite(Specialite s){
    FILE* pt = NULL;
    pt = fopen(FILE_SPECIALITES, "ab");
    if(pt != NULL){
        fseek(pt, 0, SEEK_END);
        fwrite(&s, sizeof(Specialite), 1, pt);
        fclose(pt);
    }
}

/*--------------------------Gestion Entreprises--------------------------*/

int nb_Entreprises(){
    FILE* pt = NULL;
    pt = fopen(FILE_ENTREPRISES, "rb");
    if( pt != NULL){
        int i = 0;
        Entreprise e;
        while( fread(&e, sizeof(Entreprise), 1, pt) != 0 ){
            i++;
        }
        fclose(pt);
        return i;
    }
}

void tableau_Entreprises(Entreprise* tab, int n){
    FILE* pt = NULL;
    pt = fopen(FILE_ENTREPRISES, "rb");
    if( pt!=NULL ){
        fread(tab, sizeof(Entreprise), n, pt);
        fclose(pt);
    }
}

void afficher_Entreprises(Entreprise* tab, int n){
    int i;
    for(i=0; i<n ; i++){
        printf("%d - %s - %s\n", tab[i].entrep_Index, tab[i].entrep_Nom, tab[i].entrep_Ville);
    }
}

int index_Prochaine_Entreprise(){
    FILE* pt = NULL;
    pt = fopen(FILE_ENTREPRISES, "rb");
    if(pt != NULL){
        int n;
        n = nb_Entreprises();
        if(n == 0){
            return 1;
        }
        Entreprise e;
        fseek(pt, (n-1)*sizeof(Entreprise), SEEK_CUR);
        fread(&e, sizeof(Entreprise), 1, pt);
        fclose(pt);
        return e.entrep_Index + 1;
    }
}

void ajout_Entreprise(Entreprise e){
    FILE* pt = NULL;
    pt = fopen(FILE_ENTREPRISES, "ab");
    if(pt!=NULL){
        fseek(pt, 0, SEEK_END);
        fwrite(&e, sizeof(Entreprise), 1, pt);
        fclose(pt);
    }
}

/*--------------------------Gestion Candidats--------------------------*/

int nb_Candidats(){
    FILE* pt = NULL;
    pt = fopen(FILE_CANDIDATS, "rb");
    int i=0;
    Candidat c;
    if (pt != NULL){
        while( fread(&c, sizeof(Candidat), 1, pt) != 0 ){
            i++;
        }
        fclose(pt);
        return i;
    }
}

void afficher_Candidats(Candidat* tab, int taille, int mode){
    //mode : 0 pour afficher tous les candidats
    //       1 pour afficher les candidats disponibles
    int i;
    printf(" Id - Disponible -      Nom     -    Prenom    -  Specialite1  -  Specialite2  -    Ville    - Addresse\n");
    for(i=0 ; i<taille ; i++){
        if(mode == 0){
            printf("%3d - %10s - %12s - %12s - %13s - %13s - %11s - %s\n", tab[i].ca_Index, tab[i].dispo?"DISPO":"NON DISPO", tab[i].ca_Nom, tab[i].ca_Prenom, tab[i].sp1.specialite_Nom, tab[i].sp2.specialite_Nom, tab[i].ca_Ville, tab[i].ca_Addresse);
        }
        if( (mode == 1) && (tab[i].dispo == 1) ){
            printf("%3d - %10s - %12s - %12s - %13s - %13s - %11s - %s\n", tab[i].ca_Index, tab[i].dispo?"DISPO":"NON DISPO", tab[i].ca_Nom, tab[i].ca_Prenom, tab[i].sp1.specialite_Nom, tab[i].sp2.specialite_Nom, tab[i].ca_Ville, tab[i].ca_Addresse);
        }
    }
}

void tableau_Candidats(Candidat* tab, int n){
    FILE* pt = NULL;
    pt = fopen(FILE_CANDIDATS, "rb");
    if(pt!=NULL){
        fread(tab, sizeof(Candidat), n, pt);
        fclose(pt);
    }
}

int index_Prochain_Candidat(){
    FILE* pt = NULL;
    pt = fopen(FILE_CANDIDATS, "rb");
    if(pt != NULL){
        int n;
        n = nb_Candidats();
        if(n == 0){
            return 1;
        }
        fseek(pt, (n-1)*sizeof(Candidat), SEEK_CUR);
        Candidat c;
        fread(&c, sizeof(Candidat), 1, pt);
        fclose(pt);
        return c.ca_Index + 1;
    }
}

Candidat creer_Candidat(int id){
    char l[2];
    int i,n,choix,x;
    int indice;
    Candidat c;
    c.ca_Index = id;
    c.dispo = 1;
    gets(l);
    printf("Entrez le Nom : "); gets(c.ca_Nom);
    printf("Entrez le Prenom : "); gets(c.ca_Prenom);
    printf("Entrez la Ville : "); gets(c.ca_Ville);
    printf("Entrez l'Addresse : "); gets(c.ca_Addresse);
    n = nb_Specialites();
    if(n==0){
        indice = 1;
    }
    else{
        indice = index_Prochaine_Specialite();
    }
    Specialite* tab = malloc(n * sizeof(Specialite));
    do{
        printf("Entrez le nombre de specialites (min 1 et max 2) : "); scanf("%d", &x);
    } while (x!=1 && x!=2);
    if( x == 1){
        tableau_Specialites(tab, n);
        afficher_Specialites(tab, n);
        printf("Entrez l'indice de votre specialite (0 si la votre ne figure pas parmis eux) : "); scanf("%d", &choix);
        if(choix == 0){
            c.sp1.specialite_Index = indice;
            gets(l);
            printf("Entrez l'intitule de votre specialite : "); gets(c.sp1.specialite_Nom);
            ajout_Specialite(c.sp1);
        }
        else{
            c.sp1.specialite_Index = choix;
            for(i=0;i<n;i++){
                if(tab[i].specialite_Index == choix){
                    strcpy(c.sp1.specialite_Nom, tab[i].specialite_Nom);
                    break;
                }
            }
        }
        c.sp2.specialite_Index = 0;
        strcpy(c.sp2.specialite_Nom, " ");
    }
    if( x == 2){
        tableau_Specialites(tab, n);
        afficher_Specialites(tab, n);
        printf("Entrez l'indice de votre specialite N1 (0 si la votre ne figure pas parmis eux) : "); scanf("%d", &choix);
        if(choix == 0){
            c.sp1.specialite_Index = indice;
            gets(l);
            printf("Entrez l'intitule de votre specialite : "); gets(c.sp1.specialite_Nom);
            ajout_Specialite(c.sp1);
        }
        else{
            c.sp1.specialite_Index = choix;
            for(i=0;i<n;i++){
                if(tab[i].specialite_Index == choix){
                    strcpy(c.sp1.specialite_Nom, tab[i].specialite_Nom);
                    break;
                }
            }
        }
        free(tab);
        n = nb_Specialites();
        Specialite* tab = malloc(n* sizeof(Specialite));
        tableau_Specialites(tab, n);
        afficher_Specialites(tab, n);
        printf("Entrez l'indice de votre specialite N2 (0 si la votre ne figure pas parmis eux) : "); scanf("%d", &choix);
        if(choix == 0){
            c.sp2.specialite_Index = index_Prochaine_Specialite();
            gets(l);
            printf("Entrez l'intitule de votre specialite : "); gets(c.sp2.specialite_Nom);
            ajout_Specialite(c.sp2);
        }
        else{
            c.sp2.specialite_Index = choix;
            for(i=0;i<n;i++){
                if(tab[i].specialite_Index == choix){
                    strcpy(c.sp2.specialite_Nom, tab[i].specialite_Nom);
                    break;
                }
            }
        }
    }
    free(tab);
    return c;
}

void ajout_Candidat(){
    system(CMD_CLEAR);
    printf("\t\t\t Ajout d'un candidat :\n\n");
    FILE* fp = NULL;
    fp = fopen(FILE_CANDIDATS, "ab");
    if(fp != NULL){
        int id;
        id = index_Prochain_Candidat();
        Candidat c = creer_Candidat(id);
        fseek(fp, 0, SEEK_END);
        fwrite(&c, sizeof(Candidat), 1, fp);
        fclose(fp);
        system(CMD_CLEAR);
        printf("Le candidat %s %s a ete ajoute avec succes.\n\n", c.ca_Nom, c.ca_Prenom);
        printf("\t\t\tGestion des Candidats\n\n");
        menu_Gestion_Candidats();
    }
    else printf("Impossible d'ouvrir le fichier des candidats.");
}

void modifier_Candidat(){
    int i,n,id,modifie = 0;
    n = nb_Candidats();
    Candidat* tab = malloc(n*sizeof(Candidat));
    tableau_Candidats(tab, n);
    Candidat c;

    system(CMD_CLEAR);
    printf("\t\t\tModification d'un candidat\n\n");
    afficher_Candidats(tab, n, 0);
    free(tab);
    printf("\nEntrez l'indice du Candidat a modifier : "); scanf("%d", &id);

    Candidat nc = creer_Candidat(id);
    FILE* pt1 = NULL;
    FILE* pt2 = NULL;
    pt1 = fopen(FILE_CANDIDATS, "rb");
    pt2 = fopen("temp.bin", "ab");
    if(pt1!=NULL && pt1!=NULL){
        for(i=0; i<n ; i++){
            fread(&c, sizeof(Candidat), 1, pt1);
            if(c.ca_Index == id){
                fwrite(&nc, sizeof(Candidat), 1, pt2);
                modifie = 1;
            }
            else{
                fwrite(&c, sizeof(Candidat), 1, pt2);
            }
        }
        system(CMD_CLEAR);
        if(modifie == 1) printf("Le candidat %s %s a ete modifie avec succes.\n\n", nc.ca_Nom, nc.ca_Prenom);
        if(modifie == 0) printf("Erreur lors de la modification du candidat d'indice %d.\n\n", id);
        fclose(pt1);
        fclose(pt2);
        remove(FILE_CANDIDATS);
        rename("temp.bin", FILE_CANDIDATS);
        printf("\t\t\tGestion des Candidats\n\n");
        menu_Gestion_Candidats();
    }
}

void supprimer_Candidat(){
    int i,id,n,supp = 0;
    n = nb_Candidats();
    Candidat* tab = malloc(n*sizeof(Candidat));
    Candidat c;
    tableau_Candidats(tab, n);

    system(CMD_CLEAR);
    printf("\t\t\tSuppression d'un candidat\n\n");
    afficher_Candidats(tab, n, 0);
    free(tab);

    printf("\nEntrez l'identifiant du Candidat a supprimer : "); scanf("%d", &id);

    FILE* pt1 = NULL;
    FILE* pt2 = NULL;
    pt1 = fopen(FILE_CANDIDATS, "rb");
    pt2 = fopen("temp.bin", "ab");
    if(pt1!=NULL && pt2!=NULL){
        for(i=0 ; i<n ; i++){
            fread(&c,sizeof(Candidat), 1, pt1);
            if(c.ca_Index == id){
                supp = 1;
            }
            else{
                fwrite(&c, sizeof(Candidat), 1, pt2);
            }
        }
        system(CMD_CLEAR);
        if(supp == 1) printf("Candidat d'indice %d a ete supprime avec succes.\n\n", id);
        if(supp == 0) printf("Erreur lors de la suppression du candidat d'indice %d\n\n", id);
        fclose(pt1);
        fclose(pt2);
        remove(FILE_CANDIDATS);
        rename("temp.bin", FILE_CANDIDATS);
        printf("\t\t\tGestion des Candidats\n\n");
        menu_Gestion_Candidats();
    }
}

void trier_Candidats(Candidat* tab, int n){
    int i,j;
    Candidat aide;
    for(i=0 ; i<n ; i++){
        for(j=0 ; j<n-1 ; j++){
            if( strcmp( tab[j].ca_Nom, tab[j+1].ca_Nom) >= 0 ){
                aide.ca_Index = tab[j].ca_Index;
                tab[j].ca_Index = tab[j+1].ca_Index;
                tab[j+1].ca_Index = aide.ca_Index;

                aide.dispo = tab[j].dispo;
                tab[j].dispo = tab[j+1].dispo;
                tab[j+1].dispo = aide.dispo;

                strcpy( aide.ca_Nom, tab[j].ca_Nom );
                strcpy( tab[j].ca_Nom, tab[j+1].ca_Nom );
                strcpy( tab[j+1].ca_Nom, aide.ca_Nom);
                
                strcpy( aide.ca_Prenom, tab[j].ca_Prenom );
                strcpy( tab[j].ca_Prenom, tab[j+1].ca_Prenom );
                strcpy( tab[j+1].ca_Prenom, aide.ca_Prenom);
                
                aide.sp1.specialite_Index = tab[j].sp1.specialite_Index ;
                tab[j].sp1.specialite_Index = tab[j+1].sp1.specialite_Index ;
                tab[j+1].sp1.specialite_Index = aide.sp1.specialite_Index;

                strcpy( aide.sp1.specialite_Nom, tab[j].sp1.specialite_Nom );
                strcpy( tab[j].sp1.specialite_Nom, tab[j+1].sp1.specialite_Nom );
                strcpy( tab[j+1].sp1.specialite_Nom, aide.sp1.specialite_Nom);

                aide.sp2.specialite_Index = tab[j].sp2.specialite_Index ;
                tab[j].sp2.specialite_Index = tab[j+1].sp2.specialite_Index ;
                tab[j+1].sp2.specialite_Index = aide.sp2.specialite_Index;

                strcpy( aide.sp2.specialite_Nom, tab[j].sp2.specialite_Nom );
                strcpy( tab[j].sp2.specialite_Nom, tab[j+1].sp2.specialite_Nom );
                strcpy( tab[j+1].sp2.specialite_Nom, aide.sp2.specialite_Nom);

                strcpy( aide.ca_Ville, tab[j].ca_Ville );
                strcpy( tab[j].ca_Ville, tab[j+1].ca_Ville );
                strcpy( tab[j+1].ca_Ville, aide.ca_Ville);
                
                strcpy( aide.ca_Addresse, tab[j].ca_Addresse );
                strcpy( tab[j].ca_Addresse, tab[j+1].ca_Addresse );
                strcpy( tab[j+1].ca_Addresse, aide.ca_Addresse);
            }
        }
    }
}

void alpha_Candidats(){
    int n;
    n = nb_Candidats();
    Candidat* candidats = malloc(n*sizeof(Candidat));
    tableau_Candidats(candidats, n);
    trier_Candidats(candidats, n);
    system(CMD_CLEAR);
    printf("\t\t\tCandidats tries par ordre alphabetique\n\n");
    afficher_Candidats(candidats, n, 0);
    free(candidats);
    printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
    system(CMD_CLEAR);
    printf("\t\t\tGestion des Candidats\n\n");
    menu_Gestion_Candidats();
}

void afficher_Offres_Candidat(int id_Specialite){
    int i,n;
    n = nb_Offres();
    Offre* offres = malloc(n*sizeof(Offre));
    tableau_Offres(offres, n);
    for(i=0 ; i<n ; i++){
        if( offres[i].sp.specialite_Index ==  id_Specialite){
            printf("%3d - %10s - %10s - %20s - %20s - %7d - %d/%d/%d\n", offres[i].offre_Index, offres[i].dispo?"DISPO":"NON DISPO", offres[i].sp.specialite_Nom, offres[i].e.entrep_Nom, offres[i].offre_Intitule, offres[i].offre_Salaire, offres[i].date_Debut.jour, offres[i].date_Debut.mois, offres[i].date_Debut.annee);
        }
    }
    free(offres);
}

void menu_Offres_Candidats(){
    int id,i,n;
    n = nb_Candidats();
    system(CMD_CLEAR);
    printf("\t\t\tOffres adequates pour un candidat\n\n");
    Candidat* candidats = NULL;
    candidats = malloc(n*sizeof(Candidat));
    tableau_Candidats(candidats, n);
    afficher_Candidats(candidats, n, 1);
    printf("\nEntrez l'indice du candidat dont vous voulez afficher les offres adequates : "); scanf("%d", &id);
    for(i=0; i<n ; i++){
        if (id == candidats[i].ca_Index){
            system(CMD_CLEAR);
            printf("\t\t\tOffres adequates pour le candidat %s %s\n", candidats[i].ca_Nom, candidats[i].ca_Prenom);
            printf("\t\t\t\t\t\tAddresse : %s, %s\n",candidats[i].ca_Addresse, candidats[i].ca_Ville);
            printf("\t\t\t\t\t\tSpecialite(s) : %s %s\n\n", candidats[i].sp1.specialite_Nom, candidats[i].sp2.specialite_Nom);
            printf(" Id - Disponible - Specialite -       Entreprise     -         Offre        - Salaire - Date debut\n");
            afficher_Offres_Candidat(candidats[i].sp1.specialite_Index);
            afficher_Offres_Candidat(candidats[i].sp2.specialite_Index);
            free(candidats);
            printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
            system(CMD_CLEAR);
            printf("\t\t\tGestion des Candidats\n\n");
            menu_Gestion_Candidats();
        }
    }
}

void menu_Gestion_Candidats(){
    int choice;
    printf("1. Ajouter un candidat\n");
    printf("2. Modifier un candidat\n");
    printf("3. Supprimer un candidat\n");
    printf("4. Afficher la liste des candidats par ordre alphabetique\n");
    printf("5. Afficher les offres adequats pour un candidat\n\n");
    printf("9. Retour vers le menu precedent\n");
    printf("0. Quitter le programme\n\n");

    printf("Entrez votre choix : "); scanf("%d", &choice);
    switch(choice){
        case 1:
            ajout_Candidat();
            break;
        case 2:
            modifier_Candidat();
            break;
        case 3:
            supprimer_Candidat();
            break;
        case 4:
            alpha_Candidats();
            break;
        case 5:
            menu_Offres_Candidats();
            break;
        case 9:
            system(CMD_CLEAR);
            printf("\t\t\tMenu d'acceuil de ManPower\n\n");
            menu();
            break;
        case 0:
            exit(0);
        default:
            system(CMD_CLEAR);
            printf("Votre choix n'est pas valide\n");
            printf("\t\t\tGestion des Candidats\n\n");
            menu_Gestion_Candidats();
    }
}

/*--------------------------Gestion Offres--------------------------*/

void afficher_Offres(Offre* tab, int taille, int mode){
    //mode : 0 pour affichage de toutes les offres
    //       1 pour affichage des offres disponibles
    int i;
    printf(" Id - Disponible - Specialite -       Entreprise     -         Offre        - Salaire - Date debut\n");
    for(i=0 ; i<taille ; i++){
        if( mode == 0 ){
            printf("%3d - %10s - %10s - %20s - %20s - %7d - %d/%d/%d\n", tab[i].offre_Index, tab[i].dispo?"DISPO":"NON DISPO", tab[i].sp.specialite_Nom, tab[i].e.entrep_Nom, tab[i].offre_Intitule, tab[i].offre_Salaire, tab[i].date_Debut.jour, tab[i].date_Debut.mois, tab[i].date_Debut.annee);
        }
        if( (mode == 1) && tab[i].dispo == 1 ){
            printf("%3d - %10s - %10s - %20s - %20s - %7d - %d/%d/%d\n", tab[i].offre_Index, tab[i].dispo?"DISPO":"NON DISPO", tab[i].sp.specialite_Nom, tab[i].e.entrep_Nom, tab[i].offre_Intitule, tab[i].offre_Salaire, tab[i].date_Debut.jour, tab[i].date_Debut.mois, tab[i].date_Debut.annee);
        }
    }
}

int nb_Offres(){
    FILE* pt = NULL;
    pt = fopen(FILE_OFFRES, "rb+");
    if(pt!=NULL){
        int n=0;
        Offre o;
        while( fread(&o, sizeof(Offre), 1, pt) != 0 ){
            n++;
        }
        fclose(pt);
        return n;
    }
}

int index_Prochaine_Offre(){
    FILE* pt = NULL;
    pt = fopen(FILE_OFFRES, "rb+");
    if(pt != NULL){
        int n;
        n = nb_Offres();
        if (n == 0){
            return 1;
        }
        Offre o;
        fseek(pt, (n-1)*sizeof(Offre), SEEK_CUR);
        fread(&o, sizeof(Offre), 1, pt);
        fclose(pt);
        return o.offre_Index + 1;
    }
}

void tableau_Offres(Offre* tab, int n){
    FILE* pt = NULL;
    pt = fopen(FILE_OFFRES, "rb+");
    if(pt!=NULL){
        fread(tab, sizeof(Offre), n, pt);
        fclose(pt);
    }
}

Offre creer_Offre(int id){
    int ns,ne,i;
    int id_s, id_e;
    char l[1], d[10];
    Offre o;
    o.offre_Index = id;
    ne = nb_Entreprises();
    if( ne == 0){
        id_e = 1;
    }
    else{
        id_e = index_Prochaine_Entreprise();
    }
    Entreprise* tabe = (Entreprise*)malloc(ne*sizeof(Entreprise));
    tableau_Entreprises(tabe, ne);
    afficher_Entreprises(tabe, ne);
    printf("Entrez l'indice de votre entreprise (0 si elle ne figure pas si dessus) : "); scanf("%d", &o.e.entrep_Index);
    if(o.e.entrep_Index == 0){
        o.e.entrep_Index = id_e;
        gets(l);
        printf("Entrez le nom de votre Entreprise : "); gets(o.e.entrep_Nom);
        printf("Entrez la ville de votre Entreprise : "); gets(o.e.entrep_Ville);
        ajout_Entreprise(o.e);
    }
    else{
        for(i=0 ; i<ne ; i++){
            if( tabe[i].entrep_Index == o.e.entrep_Index ){
                strcpy(o.e.entrep_Nom, tabe[i].entrep_Nom);
                strcpy(o.e.entrep_Ville, tabe[i].entrep_Ville);
                gets(l);
                break;
            }
        }
    }
    free(tabe);

    ns = nb_Specialites();
    if(ns == 0){
        id_s = 1;
    }
    else{
        id_s = index_Prochaine_Specialite();
    }
    Specialite* tabs = (Specialite*)malloc(ns*sizeof(Specialite));
    printf("Entrez l'intitule de votre offre : "); gets(o.offre_Intitule);
    printf("Entrez la remuneration relative a cette offre : "); scanf("%d",&o.offre_Salaire);
    gets(l);
    printf("Entrez la date du debut de l'offre (jj/mm/aaaa) : "); gets(d);
    o.date_Debut = char_to_date(d);
    o.dispo = 1;
    tableau_Specialites(tabs, ns);
    afficher_Specialites(tabs, ns);
    printf("Entrez l'indice de la specialite relative a votre offre (0 si elle ne figure pas si dessus) : "); scanf("%d", &o.sp.specialite_Index);
    if(o.sp.specialite_Index == 0){
        o.sp.specialite_Index = id_s;
        gets(l);
        printf("Entrez le nom de votre specialite : "); gets(o.sp.specialite_Nom);
        ajout_Specialite(o.sp);
    }
    else{
        for(i=0 ; i<ns ; i++){
            if( tabs[i].specialite_Index == o.sp.specialite_Index ){
                strcpy( o.sp.specialite_Nom, tabs[i].specialite_Nom );
                break;
            }
        }
    }
    free(tabs);
    return o;
}

void ajout_Offre(){
    int id;
    Offre o;
    id = index_Prochaine_Offre();
    if(id == 0){
        o = creer_Offre(1);
    }
    else{
        o = creer_Offre(id);
    }
    FILE* pt = NULL;
    pt = fopen(FILE_OFFRES, "ab+");
    if(pt != NULL){
        fseek(pt, 0, SEEK_END);
        fwrite(&o, sizeof(Offre), 1, pt);
        fclose(pt);
        system(CMD_CLEAR);
        printf("L'offre a ete ajoute avec succes.\n\n");
        printf("\t\t\tGestion des Offres\n\n");
        menu_Gestion_Offres();
    }
}

void modifier_Offre(){
    int i,n,id,modifie = 0;
    n = nb_Offres();
    Offre* tab = malloc(n*sizeof(Offre));
    Offre o;
    tableau_Offres(tab, n);

    system(CMD_CLEAR);
    printf("\t\t\tModification d'une Offre\n\n");
    afficher_Offres(tab, n,0);
    free(tab);
    printf("\nEntrez l'indice de l'offre a modifier : "); scanf("%d", &id);

    Offre no = creer_Offre(id);
    FILE* pt1 = NULL;
    FILE* pt2 = NULL;
    pt1 = fopen(FILE_OFFRES, "rb");
    pt2 = fopen("temp.bin", "ab");
    if(pt1!=NULL && pt2!=NULL){
        for(i=0 ; i<n ; i++){
            fread(&o, sizeof(Offre), 1, pt1);
            if(o.offre_Index == id){
                fwrite(&no, sizeof(Offre), 1, pt2);
                modifie = 1;
            }
            else{
                fwrite(&o, sizeof(Offre), 1, pt2);
            }
        }
        fclose(pt1);
        fclose(pt2);
        system(CMD_CLEAR);
        if(modifie == 1) printf("L'offre d'indice %d a ete modifie avec succes.\n\n", id);
        if(modifie == 0) printf("Erreur lors de la modification de l'offre d'indice %d.\n\n", id);
        remove(FILE_OFFRES);
        rename("temp.bin", FILE_OFFRES);
        printf("\t\t\tGestion des Offres\n\n");
        menu_Gestion_Offres();
    }
}

void trier_Offres_Entreprises(Offre* tab, int taille){
    int i,j;
    Offre aide;
    for(i=0 ; i<taille ; i++){
        for(j=0 ; j<taille-1 ; j++){
            if( strcmp( tab[j].e.entrep_Nom, tab[j+1].e.entrep_Nom) >= 0 ){
                aide.offre_Index = tab[j].offre_Index;
                tab[j].offre_Index = tab[j+1].offre_Index;
                tab[j+1].offre_Index = aide.offre_Index;

                aide.dispo = tab[j].dispo;
                tab[j].dispo = tab[j+1].dispo;
                tab[j+1].dispo = aide.dispo;

                aide.sp.specialite_Index = tab[j].sp.specialite_Index;
                tab[j].sp.specialite_Index = tab[j+1].sp.specialite_Index;
                tab[j+1].sp.specialite_Index = aide.sp.specialite_Index;
                
                strcpy( aide.sp.specialite_Nom, tab[j].sp.specialite_Nom );
                strcpy( tab[j].sp.specialite_Nom, tab[j+1].sp.specialite_Nom );
                strcpy( tab[j+1].sp.specialite_Nom, aide.sp.specialite_Nom );

                aide.e.entrep_Index = tab[j].e.entrep_Index;
                tab[j].e.entrep_Index = tab[j+1].e.entrep_Index;
                tab[j+1].e.entrep_Index = aide.e.entrep_Index;

                strcpy( aide.e.entrep_Nom, tab[j].e.entrep_Nom );
                strcpy( tab[j].e.entrep_Nom, tab[j+1].e.entrep_Nom );
                strcpy( tab[j+1].e.entrep_Nom, aide.e.entrep_Nom );

                strcpy( aide.e.entrep_Ville, tab[j].e.entrep_Ville );
                strcpy( tab[j].e.entrep_Ville, tab[j+1].e.entrep_Ville );
                strcpy( tab[j+1].e.entrep_Ville, aide.e.entrep_Ville );

                strcpy( aide.offre_Intitule, tab[j].offre_Intitule );
                strcpy( tab[j].offre_Intitule, tab[j+1].offre_Intitule );
                strcpy( tab[j+1].offre_Intitule, aide.offre_Intitule );

                aide.offre_Salaire = tab[j].offre_Salaire;
                tab[j].offre_Salaire = tab[j+1].offre_Salaire;
                tab[j+1].offre_Salaire = aide.offre_Salaire;

                aide.date_Debut.jour = tab[j].date_Debut.jour;
                tab[j].date_Debut.jour = tab[j+1].date_Debut.jour;
                tab[j+1].date_Debut.jour = aide.date_Debut.jour;

                aide.date_Debut.mois = tab[j].date_Debut.mois;
                tab[j].date_Debut.mois = tab[j+1].date_Debut.mois;
                tab[j+1].date_Debut.mois = aide.date_Debut.mois;

                aide.date_Debut.annee = tab[j].date_Debut.annee;
                tab[j].date_Debut.annee = tab[j+1].date_Debut.annee;
                tab[j+1].date_Debut.annee = aide.date_Debut.annee;
            }
        }
    }
}

void trier_Offres_Specialites(Offre* tab, int taille){
    int i,j;
    Offre aide;
    for(i=0 ; i<taille ; i++){
        for(j=0 ; j<taille-1 ; j++){
            if( strcmp( tab[j].sp.specialite_Nom, tab[j+1].sp.specialite_Nom) >= 0 ){
                aide.offre_Index = tab[j].offre_Index;
                tab[j].offre_Index = tab[j+1].offre_Index;
                tab[j+1].offre_Index = aide.offre_Index;

                aide.dispo = tab[j].dispo;
                tab[j].dispo = tab[j+1].dispo;
                tab[j+1].dispo = aide.dispo;

                aide.sp.specialite_Index = tab[j].sp.specialite_Index;
                tab[j].sp.specialite_Index = tab[j+1].sp.specialite_Index;
                tab[j+1].sp.specialite_Index = aide.sp.specialite_Index;
                
                strcpy( aide.sp.specialite_Nom, tab[j].sp.specialite_Nom );
                strcpy( tab[j].sp.specialite_Nom, tab[j+1].sp.specialite_Nom );
                strcpy( tab[j+1].sp.specialite_Nom, aide.sp.specialite_Nom );

                aide.e.entrep_Index = tab[j].e.entrep_Index;
                tab[j].e.entrep_Index = tab[j+1].e.entrep_Index;
                tab[j+1].e.entrep_Index = aide.e.entrep_Index;

                strcpy( aide.e.entrep_Nom, tab[j].e.entrep_Nom );
                strcpy( tab[j].e.entrep_Nom, tab[j+1].e.entrep_Nom );
                strcpy( tab[j+1].e.entrep_Nom, aide.e.entrep_Nom );

                strcpy( aide.e.entrep_Ville, tab[j].e.entrep_Ville );
                strcpy( tab[j].e.entrep_Ville, tab[j+1].e.entrep_Ville );
                strcpy( tab[j+1].e.entrep_Ville, aide.e.entrep_Ville );

                strcpy( aide.offre_Intitule, tab[j].offre_Intitule );
                strcpy( tab[j].offre_Intitule, tab[j+1].offre_Intitule );
                strcpy( tab[j+1].offre_Intitule, aide.offre_Intitule );

                aide.offre_Salaire = tab[j].offre_Salaire;
                tab[j].offre_Salaire = tab[j+1].offre_Salaire;
                tab[j+1].offre_Salaire = aide.offre_Salaire;

                aide.date_Debut.jour = tab[j].date_Debut.jour;
                tab[j].date_Debut.jour = tab[j+1].date_Debut.jour;
                tab[j+1].date_Debut.jour = aide.date_Debut.jour;

                aide.date_Debut.mois = tab[j].date_Debut.mois;
                tab[j].date_Debut.mois = tab[j+1].date_Debut.mois;
                tab[j+1].date_Debut.mois = aide.date_Debut.mois;

                aide.date_Debut.annee = tab[j].date_Debut.annee;
                tab[j].date_Debut.annee = tab[j+1].date_Debut.annee;
                tab[j+1].date_Debut.annee = aide.date_Debut.annee;
            }
        }
    }
}

void liste_Offres_Entreprise(){
    int n;
    n = nb_Offres();
    Offre* tab = malloc(n * sizeof(Offre));
    tableau_Offres(tab, n);
    trier_Offres_Entreprises(tab,n);
    system(CMD_CLEAR);
    printf("\t\t\tOffres disponibles selon le nom d'Entreprise\n\n");
    afficher_Offres(tab, n, 0);
    printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
    system(CMD_CLEAR);
    printf("\t\t\tGestion des Offres\n\n");
    menu_Gestion_Offres();
    free(tab);

}

void liste_Offres_Specialite(){
    int n;
    n = nb_Offres();
    Offre* tab = malloc(n * sizeof(Offre));
    tableau_Offres(tab, n);
    trier_Offres_Specialites(tab,n);
    system(CMD_CLEAR);
    printf("\t\t\tOffres disponibles selon les Specialites\n\n");
    afficher_Offres(tab, n, 1);
    printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
    system(CMD_CLEAR);
    printf("\t\t\tGestion des Offres\n\n");
    menu_Gestion_Offres();
    free(tab);
}

void liste_Candidats_Offre(int id){
    int nc, i;

    nc = nb_Candidats();
    Candidat* candidats = malloc(nc*sizeof(Candidat));
    tableau_Candidats(candidats, nc);
    printf(" Id - Disponible -      Nom     -    Prenom    -  Specialite1  -  Specialite2  -    Ville    - Addresse\n");
    for(i=0 ; i<nc ; i++){
        if( (candidats[i].sp1.specialite_Index == id) || (candidats[i].sp2.specialite_Index == id) ){
            printf("%3d - %10s - %12s - %12s - %13s - %13s - %11s - %s\n", candidats[i].ca_Index, candidats[i].dispo?"DISPO":"NON DISPO", candidats[i].ca_Nom, candidats[i].ca_Prenom, candidats[i].sp1.specialite_Nom, candidats[i].sp2.specialite_Nom, candidats[i].ca_Ville, candidats[i].ca_Addresse);
        }
    }
    free(candidats);
    printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
    system(CMD_CLEAR);
    printf("\t\t\tGestion des Offres\n\n");
    menu_Gestion_Offres();
}

void candidats_Potentiels(){
    int n, i, id_offre, id_sp;
    n = nb_Offres();
    Offre* offres = malloc(n*sizeof(Offre));
    tableau_Offres(offres, n);
    system(CMD_CLEAR);
    afficher_Offres(offres, n, 1);
    printf("\nEntrez l'indice de l'offre dont vous voulez afficher les candidats potentiels : "); scanf("%d", &id_offre);
    for(i=0; i<n ; i++){
        if( offres[i].offre_Index == id_offre ){
            id_sp = offres[i].sp.specialite_Index;
            break;
        }
    }
    free(offres);
    system(CMD_CLEAR);
    printf("\t\t\tListe des candidats potentiels pour l'offre d'indice %d\n\n", id_offre);
    liste_Candidats_Offre(id_sp);
}

void menu_Gestion_Offres(){
    int choice;
    printf("1. Ajouter une nouvelle offre\n");
    printf("2. Modifier une offre\n");
    printf("3. Lister les offres disponibles selon les entreprises\n");
    printf("4. Lister les offres disponibles selon les specialites\n");
    printf("5. Lister les candidats potentiels pour une offre\n\n");
    printf("9. Retour vers le menu precedent\n");
    printf("0. Quitter\n\n");
    printf("Entrez votre choix : "); scanf("%d",&choice);
    switch( choice ){
        case 1:
            ajout_Offre();
            break;
        case 2:
            modifier_Offre();
            break;
        case 3:
            liste_Offres_Entreprise();
            break;
        case 4:
            liste_Offres_Specialite();
            break;
        case 5:
            candidats_Potentiels();
            break;
        case 9:
            system(CMD_CLEAR);
            printf("\t\t\tMenu d'acceuil de ManPower\n\n");
            menu();
            break;
        case 0:
            exit(0);
            break;
        default:
            system(CMD_CLEAR);
            printf("Votre choix n'est pas valide\n\n");
            printf("\t\t\tGestion des Offres\n\n");
            menu_Gestion_Offres();

    }
}

/*--------------------------Gestion Recrutements--------------------------*/

void afficher_Recrutements(Recrutement* tab, int taille, int mode){
    //mode : 0 : pour affichage de tous les recrutements
    //       1 : pour affichage des recrutements actifs
    int i;
    printf(" Id - Actif -         Offre        -         Candidat          - Type - Date debut - Date fin\n");
    for(i=0 ; i<taille ; i++){
        if( mode == 0 ){
            printf("%3d - %5s - %20s - %12s %12s - %4s - %2d/%2d/%4d - %2d/%2d/%4d\n", tab[i].rec_Index, tab[i].rec_Actif?"OUI":"NON", tab[i].o.offre_Intitule, tab[i].c.ca_Nom, tab[i].c.ca_Prenom, tab[i].type_Recrutement,tab[i].o.date_Debut.jour, tab[i].o.date_Debut.mois, tab[i].o.date_Debut.annee, tab[i].date_Fin.jour, tab[i].date_Fin.mois, tab[i].date_Fin.annee);
        }
        if( (mode == 1) && tab[i].rec_Actif == 1 ){
            printf("%3d - %5s - %20s - %12s %12s - %4s - %2d/2%d/%4d - %2d/%2d/%4d\n", tab[i].rec_Index, tab[i].rec_Actif?"OUI":"NON", tab[i].o.offre_Intitule, tab[i].c.ca_Nom, tab[i].c.ca_Prenom, tab[i].type_Recrutement,tab[i].o.date_Debut.jour, tab[i].o.date_Debut.mois, tab[i].o.date_Debut.annee, tab[i].date_Fin.jour, tab[i].date_Fin.mois, tab[i].date_Fin.annee);
        }
    }
}

int nb_Recrutements(){
    FILE* pt = NULL;
    pt = fopen(FILE_RECRUTEMENTS, "rb+");
    int n=0;
    if(pt!=NULL){
        Recrutement r;
        while( fread(&r, sizeof(Recrutement), 1, pt) != 0 ){
            n++;
        }
        fclose(pt);
    }
    return n;
}

int index_Prochain_Recrutement(){
    FILE* pt = NULL;
    pt = fopen(FILE_RECRUTEMENTS, "rb+");
    int n;
    n = nb_Recrutements();
    if(pt != NULL){
        if (n == 0){
            return 1;
        }
        Recrutement r;
        fseek(pt, (n-1)*sizeof(Recrutement), SEEK_CUR);
        fread(&r, sizeof(Recrutement), 1, pt);
        fclose(pt);
        return r.rec_Index + 1;
    }
}

void tableau_Recrutements(Recrutement* tab, int n){
    FILE* pt = NULL;
    pt = fopen(FILE_RECRUTEMENTS, "rb+");
    if(pt!=NULL){
        fread(tab, sizeof(Recrutement), n, pt);
        fclose(pt);
    }
}

void ajout_Recrutement(){
    /*
    Cette fonction permet a l'utilisateur d'associer une offre a un candidat tout en mettant a jour leurs disponibilite
    */
    int i,j;
    int nc, no;
    int id_c, id_o;
    
    Recrutement r;
    int n;
    n = nb_Recrutements();
    nc = nb_Candidats();
    no = nb_Offres();
    if( nc == 0 || no == 0){
        printf("La base do donnees n'a encore aucun candidat et/ou offre.\nVeuillez ajouter un candidat et une offre d'abord.");
        printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
        system(CMD_CLEAR);
        printf("\t\t\tGestion des Recrutements\n\n");
     menu_Gestion_Recrutements();
    }
    if( n==0 ){
        r.rec_Index = 1;
    }
    else{
        r.rec_Index = index_Prochain_Recrutement();
    }
    r.rec_Actif = 1;
    r.date_Fin.jour = 0;
    r.date_Fin.mois = 0;
    r.date_Fin.annee = 0;
    system(CMD_CLEAR);
    printf("\t\t\tAjout d'un recrutement\n\n");
    printf("Entrez le type de votre recrutement (CDI/CDD/CA/CNE) : "); scanf("%s", &r.type_Recrutement);

    Candidat* candidats = malloc(nc*sizeof(Candidat));
    system(CMD_CLEAR);
    printf("Liste des candidats disponibles :\n\n");
    tableau_Candidats(candidats, nc);
    afficher_Candidats(candidats, nc, 1);
    printf("\nEntrez l'indice de votre candidat : "); scanf("%d", &id_c);
    for(i=0 ; i<nc ; i++){
        if(candidats[i].ca_Index == id_c){
            r.c.ca_Index = candidats[i].ca_Index;
            r.c.dispo = 0;
            candidats[i].dispo = 0;
            strcpy( r.c.ca_Nom, candidats[i].ca_Nom );
            strcpy( r.c.ca_Prenom, candidats[i].ca_Prenom );
            strcpy( r.c.ca_Ville, candidats[i].ca_Ville );
            strcpy( r.c.ca_Addresse, candidats[i].ca_Addresse );

            r.c.sp1.specialite_Index = candidats[i].sp1.specialite_Index ;
            strcpy( r.c.sp1.specialite_Nom, candidats[i].sp1.specialite_Nom );

            r.c.sp2.specialite_Index = candidats[i].sp2.specialite_Index ;
            strcpy( r.c.sp2.specialite_Nom, candidats[i].sp2.specialite_Nom );
            
            break;
        }
    }
    FILE* pt1 = fopen(FILE_CANDIDATS, "wb");
    if(pt1!=NULL){
        fwrite(candidats, sizeof(Candidat), nc, pt1);
        fclose(pt1);
    }
    free(candidats);

    Offre* offres = malloc( no*sizeof(Offre) );
    system(CMD_CLEAR);
    printf("Liste des offres disponibles :\n\n");
    tableau_Offres(offres, no);
    afficher_Offres(offres, no, 1);
    printf("\nEntrez l'indice de votre offre : "); scanf("%d", &id_o);
    for(j=0 ; j<no ; j++){
        if(offres[j].offre_Index == id_o){
            r.o.offre_Index = offres[j].offre_Index;
            r.o.dispo = 0;
            offres[j].dispo = 0;
            r.o.sp.specialite_Index = offres[j].sp.specialite_Index;
            strcpy( r.o.sp.specialite_Nom, offres[j].sp.specialite_Nom );
            r.o.e.entrep_Index = offres[j].e.entrep_Index;
            strcpy( r.o.e.entrep_Nom, offres[j].e.entrep_Nom );
            strcpy( r.o.e.entrep_Ville, offres[j].e.entrep_Ville );
            strcpy( r.o.offre_Intitule, offres[j].offre_Intitule );
            r.o.offre_Salaire = offres[j].offre_Salaire;
            r.o.date_Debut.jour = offres[j].date_Debut.jour;
            r.o.date_Debut.mois = offres[j].date_Debut.mois;
            r.o.date_Debut.annee = offres[j].date_Debut.annee;
            break;
        }
    }
    FILE* pt2 = fopen(FILE_OFFRES, "wb");
    if(pt2!=NULL){
        fwrite(offres, sizeof(Offre), no, pt2);
        fclose(pt2);
    }
    free(offres);

    FILE* pt3 = fopen(FILE_RECRUTEMENTS, "ab");
    if(pt3!=NULL){
        fwrite(&r, sizeof(Recrutement), 1, pt3);
        fclose(pt3);
    }
    printf("Recrutement ajoute avec succes");
    printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
    system(CMD_CLEAR);
    printf("\t\t\tGestion des Recrutements\n\n");
    menu_Gestion_Recrutements();
}

void liste_Recrutements(){
    int i,nr;
    char s1[CMAX], s2[CMAX];
    Date d1,d2;
    nr = nb_Recrutements();
    Recrutement* tab = malloc(nr*sizeof(Recrutement));
    tableau_Recrutements(tab, nr);
    
    system(CMD_CLEAR);
    printf("\t\t\tRecrutements en un intervalle :\n");
    printf("Entrez la date de debut (jj/mm/aaaa) : "); scanf("%s", &s1);
    printf("Entrez la date de fin (jj/mm/aaaa) : "); scanf("%s", &s2);
    system(CMD_CLEAR);
    printf("\t\t\tListe des recrutements effectues par ManPower entre %s et %s :\n\n", s1, s2);
    printf(" Id - Actif -         Offre        -         Candidat          - Type - Date Debut - Date Fin\n");
    d1 = char_to_date(s1);
    d2 = char_to_date(s2);
    for(i=0 ; i<nr; i++){
        if( ( cmp_Date(tab[i].o.date_Debut , d1) == 1) && ( cmp_Date(d2 , tab[i].date_Fin) == 1) ){
            printf("%3d - %5s - %20s - %12s %12s - %4s - %2d/%2d/%4d - %2d/%2d/%4d\n", tab[i].rec_Index, tab[i].rec_Actif?"OUI":"NON", tab[i].o.offre_Intitule, tab[i].c.ca_Nom, tab[i].c.ca_Prenom, tab[i].type_Recrutement, tab[i].o.date_Debut.jour, tab[i].o.date_Debut.mois, tab[i].o.date_Debut.annee, tab[i].date_Fin.jour, tab[i].date_Fin.mois, tab[i].date_Fin.annee);
        }
    }
    free(tab);

    printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
    system(CMD_CLEAR);
    printf("\t\t\tGestion des Recrutements\n\n");
    menu_Gestion_Recrutements();
}

void annuler_Recrutement(){
    int i,j,k,id;
    int nr, nc, no;
    char s[10];

    nr = nb_Recrutements();
    Recrutement* tab = malloc(nr * sizeof(Recrutement));
    system(CMD_CLEAR);
    printf("\t\t\tListe des recrutements actifs :\n\n");
    tableau_Recrutements(tab, nr);
    afficher_Recrutements(tab, nr, 1);
    printf("Entrez l'indice du recrutement a annuler : "); scanf("%d", &id);
    for(i=0 ; i<nr ; i++){
        if( tab[i].rec_Index == id ){
            tab[i].rec_Actif = 0;
            tab[i].c.dispo = 1;
            tab[i].o.dispo = 1;
            printf("\nEntrer la date d'annulation (jj/mm/aaaa) : ");scanf("%s", &s); 
            tab[i].date_Fin = char_to_date(s);
            break;
        }
    }
    
    nc = nb_Candidats();
    Candidat* candidats = malloc(nc*sizeof(Candidat));
    tableau_Candidats(candidats, nc);
    for(j=0 ; j<nc ; j++){
        if( candidats[j].ca_Index == tab[i].c.ca_Index ){
            candidats[j].dispo = 1;
            break;
        }
    }
    FILE* ptc = fopen(FILE_CANDIDATS, "wb");
    if(ptc != NULL){
        fwrite(candidats, sizeof(Candidat), nc, ptc);
        fclose(ptc);
    }
    free(candidats);

    no = nb_Offres();
    Offre* offres = malloc(no*sizeof(Offre));
    tableau_Offres(offres, no);
    for(k=0 ; k<no ; k++){
        if( offres[k].offre_Index == tab[i].o.offre_Index ){
            offres[k].dispo = 1;
            break;
        }
    }
    FILE* pto = fopen(FILE_OFFRES, "wb");
    if(pto != NULL){
        fwrite(offres, sizeof(Offre), no, pto);
        fclose(pto);
    }
    free(offres);


    FILE* ptr = fopen(FILE_RECRUTEMENTS, "wb");
    if(ptr != NULL){
        fwrite(tab, sizeof(Recrutement), nr, ptr);
        fclose(ptr);
    }
    free(tab);
    printf("Recrutement annule avec succes");
    printf("\nTouchez Entrer pour continuer ..."); getc(stdin); getc(stdin);
    system(CMD_CLEAR);
    printf("\t\t\tGestion des Recrutements\n\n");
    menu_Gestion_Recrutements();
}

void menu_Gestion_Recrutements(){
    int choix;
    printf("1. Ajouter un recrutement\n");
    printf("2. Annuler un recrutement\n");
    printf("3. Liste des recrutements en un intervalle de temps\n\n");
    printf("9. Retour vers le menu precedent\n");
    printf("0. Quitter\n\n");
    printf("Entrez votre choix : "); scanf("%d", &choix);

    switch(choix){
        case 1:
            ajout_Recrutement();
            break;
        case 2:
            annuler_Recrutement();
            break;
        case 3:
            liste_Recrutements();
            break;
        case 9:
            system(CMD_CLEAR);
            printf("\t\t\tMenu d'acceuil de ManPower\n\n");
            menu();
            break;
        case 0:
            exit(0);
        default:
            system(CMD_CLEAR);
            printf("Votre choix n'est pas valide.\n");
            printf("\t\t\tGestion des Recrutements\n\n");
            menu_Gestion_Recrutements();
    }
}

/*-------------------------------Menu Principal-------------------------------*/

void menu(){
    int choice;
    printf("1. Gestion des candidats\n");
    printf("2. Gestion des offres\n");
    printf("3. Gestion recrutements\n\n");
    printf("0. Quitter\n\n");

    printf("Entrez votre choix : "); scanf("%d",&choice);
    switch(choice){
        case 1 :
            system(CMD_CLEAR);
            printf("\t\t\tGestion des Candidats\n\n");
            menu_Gestion_Candidats();
            break;
        case 2 :
            system(CMD_CLEAR);
            printf("\t\tGestion des Offres\n\n");
            menu_Gestion_Offres();
            break;
        case 3 :
            system(CMD_CLEAR);
            printf("\t\t\tGestion des Recrutements\n\n");
            menu_Gestion_Recrutements();
            break;
        case 0 :
            exit(0);
        default:
            system(CMD_CLEAR);
            printf("Votre choix n'est pas valide.\n");
            printf("\t\t\tMenu d'acceuil de ManPower\n\n");
            menu();
    }
}

void login(){
    char mdp[16];
    system(CMD_CLEAR);
    printf("\t\tBonjour dans l'application de gestion d'une societe d'interim\n\n");
    do{
        printf("Entrez le mot de passe d'admin : "); scanf("%s", &mdp);
    }while( strcmp(mdp, MDP_ADMIN) != 0 );
    system(CMD_CLEAR);
    printf("\t\t\tMenu d'acceuil de ManPower\n\n");
    menu();
}

void main(){
    system(CMD_CLEAR);
    login();
}