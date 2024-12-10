#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 30
#define FILENAME "commandes.txt"

typedef struct {
    char nom[20];
    float prix;
} Plat;

typedef struct Commande {
    char nom_client[20];
    int telephone;
    Plat plats[MAX_SIZE];
    int nb_plats;
    float prix_total;
    int reference;
} Commande;

typedef struct NodeCommande {
    Commande commande;
    struct NodeCommande* suivant;
} NodeCommande;

typedef struct FileCommande {
    NodeCommande* tete;
    NodeCommande* queue;
} FileCommande;

void ajouterPlatAuMenu(Plat menu[], int *taille_menu, char *nom, float prix);
void supprimerPlatDuMenu(Plat menu[], int *taille_menu, char *nom);
void afficherMenu(Plat menu[], int taille_menu);
int rechercherPlat(Plat menu[], int taille_menu, char *nom);
void ajouterCommande(FileCommande* file, char* nom_client, int telephone, Plat plats[], int nb_plats, int reference);
void enregistrerCommandesDansFichier(FileCommande* file);
void afficherCommandes(FileCommande* file);
float calculerPrixTotal(Plat plats[], int nb_plats);

int main() {

    int choix, telephone, nb_plats, reference;
    float prix;
    char plat[20], nom_client[20];
    Plat menu[MAX_SIZE];
    int taille_menu = 0;

    FileCommande fileCommandes;
    fileCommandes.tete = NULL;
    fileCommandes.queue = NULL;

    ajouterPlatAuMenu(menu, &taille_menu, "Couscous", 15.0);
    ajouterPlatAuMenu(menu, &taille_menu, "Brik", 2.5);
    ajouterPlatAuMenu(menu, &taille_menu, "Mechoui", 28);
    ajouterPlatAuMenu(menu, &taille_menu, "Lablabi", 4.5);

    Plat plats_commande1[] = { menu[0], menu[1] };
    ajouterCommande(&fileCommandes, "Ahmed", 12345678, plats_commande1, 2, 1001);

    Plat plats_commande2[] = { menu[3] };
    ajouterCommande(&fileCommandes, "Maya", 98765432, plats_commande2, 1, 1002);

    do {
        printf("1. pour ajouter un plat au Menu \n ");
        printf("2. pour supprimer un plat du Menu \n");
        printf("3. pour afficher le Menu \n ");
        printf("4. pour rechercher un plat \n ");
        printf("5. pour ajouter une commande \n ");
        printf("6. pour enregistrer les commandes dans un fichier \n ");
        printf("7. pour afficher les commandes \n ");
        printf("0. pour quitter \n ");
        printf("entrer votre choix: ");
        scanf("%i", &choix);
        getchar();

        switch (choix) {
            case 1:
                printf("donner le nom du plat: ");
                fgets(plat, 20, stdin);
                plat[strcspn(plat, "\n")] = '\0';
                printf("donner le prix: ");
                scanf("%f", &prix);
                ajouterPlatAuMenu(menu, &taille_menu, plat, prix);
                break;

            case 2:
                printf("donner le nom du plat: ");
                fgets(plat, 20, stdin);
                plat[strcspn(plat, "\n")] = '\0';
                supprimerPlatDuMenu(menu, &taille_menu, plat);
                break;

            case 3:
                afficherMenu(menu, taille_menu);
                break;

            case 4:
                printf("donner le nom du plat: ");
                fgets(plat, 20, stdin);
                plat[strcspn(plat, "\n")] = '\0';
                if (rechercherPlat(menu, taille_menu, plat) != -1) {
                    printf("Plat trouve: %s\n", plat);
                }
                else {
                    printf("Plat non trouve dans le menu.\n");
                }
                break;

            case 5:
                printf("taper le nom du client: ");
                fgets(nom_client, 20, stdin);
                nom_client[strcspn(nom_client, "\n")] = '\0';
                printf("taper le numero de telephone du client: ");
                scanf("%i", &telephone);
                printf("donner le nombre de plats: ");
                scanf("%i", &nb_plats);
                if (nb_plats > MAX_SIZE) {
                    printf("Nombre de plats trop élevé. Limité à %d.\n", MAX_SIZE);
                    nb_plats = MAX_SIZE;
                }
                printf("donner la reference: ");
                scanf("%i", &reference);
                getchar();

                Plat plats_commande[MAX_SIZE];
                int i = 0;
                while (i < nb_plats) {
                    printf("donner le nom du plat: ");
                    fgets(plat, 20, stdin);
                    plat[strcspn(plat, "\n")] = '\0';
                    int index = rechercherPlat(menu, taille_menu, plat);
                    if (index == -1) {
                        printf("Plat non trouvé dans le menu.\n");
                        break;
                    }
                    plats_commande[i] = menu[index];
                    i++;
                }
                ajouterCommande(&fileCommandes, nom_client, telephone, plats_commande, nb_plats, reference);
                break;

            case 6:
                enregistrerCommandesDansFichier(&fileCommandes);
                break;

            case 7:
                afficherCommandes(&fileCommandes);
                break;

            case 0:
                break;
        }
    } while (choix != 0);

    return 0;
}

void ajouterPlatAuMenu(Plat menu[], int *taille_menu, char *nom, float prix) {

    if (*taille_menu < MAX_SIZE) {
        strcpy(menu[*taille_menu].nom, nom);
        menu[*taille_menu].prix = prix;
        (*taille_menu)++;
    }
    else {
        printf("Le menu est plein.\n");
    }
}

void supprimerPlatDuMenu(Plat menu[], int *taille_menu, char *nom) {

    int index = rechercherPlat(menu, *taille_menu, nom);
    if (index != -1) {
        for (int i = index; i < *taille_menu - 1; i++) {
            menu[i] = menu[i + 1];
        }
        (*taille_menu)--;
    }
    else {
        printf("Plat non trouvé dans le menu.\n");
    }
}

void afficherMenu(Plat menu[], int taille_menu) {

    for (int i = 0; i < taille_menu; i++) {
        printf("%d. %s - %.2f TND\n", i + 1, menu[i].nom, menu[i].prix);
    }
}

int rechercherPlat(Plat menu[], int taille_menu, char *nom) {

    for (int i = 0; i < taille_menu; i++) {
        if (strcmp(menu[i].nom, nom) == 0) {
            return i;
        }
    }
    return -1;
}

float calculerPrixTotal(Plat plats[], int nb_plats) {

    float total = 0.0;
    for (int i = 0; i < nb_plats; i++) {
        total += plats[i].prix;
    }
    return total;
}

void ajouterCommande(FileCommande* file, char* nom_client, int telephone, Plat plats[], int nb_plats, int reference) {

    NodeCommande* nouveauNode = malloc(sizeof(NodeCommande));
    strcpy(nouveauNode->commande.nom_client, nom_client);
    nouveauNode->commande.telephone = telephone;
    for (int i = 0; i < nb_plats; i++) {
        nouveauNode->commande.plats[i] = plats[i];
    }

    nouveauNode->commande.nb_plats = nb_plats;
    nouveauNode->commande.prix_total = calculerPrixTotal(plats, nb_plats);
    nouveauNode->commande.reference = reference;
    nouveauNode->suivant = NULL;

    if (file->tete == NULL && file->queue == NULL) {
        file->tete = nouveauNode;
        file->queue = nouveauNode;
    }
    else {
        file->queue->suivant = nouveauNode;
        file->queue = nouveauNode;
    }
}

void enregistrerCommandesDansFichier(FileCommande* file) {

    FILE* fichier = fopen(FILENAME, "w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }
    NodeCommande* temp = file->tete;
    while (temp != NULL) {
        fprintf(fichier, "Reference de la commande: %d\n", temp->commande.reference);
        fprintf(fichier, "Commande pour %s (Telephone: %d):\n", temp->commande.nom_client, temp->commande.telephone);
        for (int i = 0; i < temp->commande.nb_plats; i++) {
            fprintf(fichier, "  %d. %s - %.2f TND\n", i + 1, temp->commande.plats[i].nom, temp->commande.plats[i].prix);
        }
        fprintf(fichier, "Prix total a payer: %.2f TND\n\n", temp->commande.prix_total);
        temp = temp->suivant;
    }
    fclose(fichier);
}

void afficherCommandes(FileCommande* file) {

    NodeCommande* temp = file->tete;
    while (temp != NULL) {
        printf( "Reference de la commande: %d\n", temp->commande.reference);
        printf( "Commande pour %s (Telephone: %d):\n", temp->commande.nom_client, temp->commande.telephone);
        for (int i = 0; i < temp->commande.nb_plats; i++) {
            printf( "  %d. %s - %.2f TND\n", i + 1, temp->commande.plats[i].nom, temp->commande.plats[i].prix);
        }
        printf("Prix total a payer: %.2f TND\n\n", temp->commande.prix_total);
        temp = temp->suivant;
    }
}

