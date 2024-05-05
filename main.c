#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LEN 100
#define FILENAME "stock_data.csv"
#define TRANSACTION_FILENAME "transactions.csv"

typedef struct {
    int id;
    char nom[MAX_LEN];
    char description[MAX_LEN];
    char categorie[MAX_LEN];
    float prix_unitaire;
    int quantite_stock;
    int seuil_alerte;
    char date_derniere_entree[MAX_LEN];
    char date_derniere_sortie[MAX_LEN];
    int id_fournisseur;
    float prix_achat;
    float benefice;
    int quantite_achetee;
} Produit;

void afficherMenuClient() {
    printf("\nMenu Client:\n");
    printf("1. Afficher le stock\n");
    printf("2. Afficher les categories disponibles et leurs produits\n");
    printf("3. Acheter un produit\n");
    printf("4. Rechercher un produit par nom\n");
    printf("5. Afficher les produits tries par prix croissant\n");
    printf("6. Afficher les produits tries par prix decroissant\n");
    printf("7. Quitter\n");
}
void afficherMenuGerant() {
    printf("\n Menu Gerant:\n");
    printf("1. Ajouter un produit\n");
    printf("2. Supprimer un produit\n");
    printf("3. Afficher le stock\n");
    printf("4. Modifier un produit\n");
    printf("5. Afficher les benefices d'argent de chaque produit et la quantite achetee\n");
    printf("6. Quitter\n");
}

bool verifierMotDePasse() {
    char mot_de_passe[MAX_LEN];
    printf("Veuillez entrer le mot de passe : ");
    fgets(mot_de_passe, MAX_LEN, stdin);
    mot_de_passe[strcspn(mot_de_passe, "\n")] = 0;
    if (strcmp(mot_de_passe, "1234") == 0) {
        printf("Mot de passe correct.\n");
        return true;
    } else {
        printf("Mot de passe incorrect. Acces refuse.\n");
        return false;
    }
}

float calculerBeneficeTotal() {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit p;
    float total_benefice = 0.0;

    fp = fopen(TRANSACTION_FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier des transactions.\n");
        return 0.0;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%f,%d,%f", &p.id, p.nom, &p.prix_unitaire, &p.quantite_achetee, &p.benefice);
        total_benefice += p.benefice;
    }

    fclose(fp);

    return total_benefice;
}

void afficherBeneficesEtQuantiteAchete() {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit p;
    float total_benefice = 0.0;
    int total_quantite_achetee = 0;

    fp = fopen(TRANSACTION_FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier des transactions.\n");
        return;
    }

    printf("\nBenefices d'argent de chaque produit et quantite achetee:\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("| ID  | Nom          | Prix unitaire | Quantité achetée | Bénéfice d'argent |\n");
    printf("----------------------------------------------------------------------------------\n");

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%f,%d,%f", &p.id, p.nom, &p.prix_unitaire, &p.quantite_achetee, &p.benefice);
        printf("| %-3d | %-12s | %-13.2f | %-16d | %-17.2f |\n", p.id, p.nom, p.prix_unitaire, p.quantite_achetee, p.benefice);
        total_benefice += p.benefice;
        total_quantite_achetee += p.quantite_achetee;
    }

    printf("----------------------------------------------------------------------------------\n");
    printf("Total bénéfices d'argent: %.2f\n", total_benefice);
    printf("Total quantité achetée: %d\n", total_quantite_achetee);

    fclose(fp);
}

void afficherCategoriesDisponibles() {
    FILE *fp;
    char buffer[MAX_LEN];
    char categorie[MAX_LEN];
    char categoriesDejaAffichees[MAX_LEN][MAX_LEN];
    int categoriesCount = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("\nCatégories disponibles:\n");
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%*d,%*d,%[^,],%[^,],%*d", categorie);
        bool categorieDejaAffichee = false;
        for (int i = 0; i < categoriesCount; i++) {
            if (strcmp(categoriesDejaAffichees[i], categorie) == 0) {
                categorieDejaAffichee = true;
                break;
            }
        }
        if (!categorieDejaAffichee) {
            printf("%s\n", categorie);
            strcpy(categoriesDejaAffichees[categoriesCount], categorie);
            categoriesCount++;
        }
    }

    fclose(fp);
}


void afficherProduitsParCategorie(char categorie[]) {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit p;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("\nProduits de la categorie \"%s\":\n", categorie);
    printf("------------------------------------------------------------------------------------\n");
    printf("| ID  | Nom          | Description        | Categorie | Prix   | Quantite | Seuil | Fournisseur |\n");
    printf("------------------------------------------------------------------------------------\n");
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%d,%d,%[^,],%[^,],%d", &p.id, p.nom, p.description, p.categorie, &p.prix_unitaire, &p.quantite_stock, &p.seuil_alerte, &p.id_fournisseur);
        if (strcmp(p.categorie, categorie) == 0) {
            printf("| %-3d | %-12s | %-18s | %-9s | %-5.2f | %-8d | %-5d | %-11d |\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire, p.quantite_stock, p.seuil_alerte, p.id_fournisseur);
        }
    }
    printf("------------------------------------------------------------------------------------\n");

    fclose(fp);
}

void acheterProduit(int id, int quantite) {
    FILE *fp, *temp;
    char buffer[MAX_LEN];
    Produit p;
    bool produitTrouve = false;

    fp = fopen(FILENAME, "r");
    temp = fopen("temp.csv", "w");
    if (fp == NULL || temp == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%d,%d,%[^,],%[^,],%d", &p.id, p.nom, p.description, p.categorie, &p.prix_unitaire,
               &p.quantite_stock, &p.seuil_alerte, p.date_derniere_entree, p.date_derniere_sortie,
               &p.id_fournisseur);
        if (p.id == id) {
            produitTrouve = true;
            if (quantite <= p.quantite_stock) {
                float prix_achat = quantite * p.prix_unitaire;
                float benefice = prix_achat - (quantite * p.prix_unitaire);
                p.quantite_stock -= quantite;
                p.prix_achat = prix_achat;
                p.benefice = benefice;
                p.quantite_achetee = quantite;
            }
        }
        fprintf(temp, "%d,%s,%s,%s,%.2f,%d,%d,%s,%s,%d\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire,
                p.quantite_stock, p.seuil_alerte, p.date_derniere_entree, p.date_derniere_sortie,
                p.id_fournisseur);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if (!produitTrouve) {
        printf("Le produit avec l'ID %d n'existe pas dans le stock.\n", id);
    }

    fp = fopen(TRANSACTION_FILENAME, "a");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier des transactions.\n");
        return;
    }
    fprintf(fp, "%d,%s,%f,%d,%f\n", p.id, p.nom, p.prix_unitaire, p.quantite_achetee, p.benefice);
    fclose(fp);
}



void ajouterProduit(Produit p) {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit existingProduct;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%d,%d,%[^,],%[^,],%d", &existingProduct.id, existingProduct.nom, existingProduct.description, existingProduct.categorie, &existingProduct.prix_unitaire,
               &existingProduct.quantite_stock, &existingProduct.seuil_alerte, existingProduct.date_derniere_entree, existingProduct.date_derniere_sortie, &existingProduct.id_fournisseur);
        if (existingProduct.id == p.id) {
            printf("Un produit avec l'ID %d existe deja dans le stock. Impossible d'ajouter un produit avec le meme ID.\n", p.id);
            fclose(fp);
            return;
        }
    }

    fclose(fp);

    if (p.quantite_stock > p.seuil_alerte) {
        printf("Impossible d'ajouter le produit \"%s\" car la quantité en stock dépasse le seuil d'alerte.\n", p.nom);
        return;
    }

    fp = fopen(FILENAME, "a");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fp, "%d,%s,%s,%s,%.2f,%d,%d,%s,%s,%d\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire,
            p.quantite_stock, p.seuil_alerte, p.date_derniere_entree, p.date_derniere_sortie,
            p.id_fournisseur);

    fclose(fp);

    printf("Produit ajouté avec succes.\n");
}

void supprimerProduit(int id) {
    FILE *fp, *temp;
    char buffer[MAX_LEN];
    char *token;
    Produit p;

    fp = fopen(FILENAME, "r");
    temp = fopen("temp.csv", "w");
    if (fp == NULL || temp == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        token = strtok(buffer, ",");
        p.id = atoi(token);
        if (p.id != id) {
            fprintf(temp, "%s", buffer);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.csv", FILENAME);
    printf("Produit supprime avec succes.\n");
}

void afficherStock() {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit p;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("\nStock:\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("| ID  | Nom          | Description        | Categorie | Prix   | Quantite | Seuil | Fournisseur |\n");
    printf("------------------------------------------------------------------------------------\n");
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%d,%d,%[^,],%[^,],%d", &p.id, p.nom, p.description, p.categorie, &p.prix_unitaire,
               &p.quantite_stock, &p.seuil_alerte, p.date_derniere_entree, p.date_derniere_sortie, &p.id_fournisseur);
        printf("| %-3d | %-12s | %-18s | %-9s | %-6.2f | %-8d | %-5d | %-10d |\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire,
               p.quantite_stock, p.seuil_alerte, p.id_fournisseur);

    }
    printf("------------------------------------------------------------------------------------\n");

    fclose(fp);
}

void rechercherProduitParNom() {
    FILE *fp;
    char buffer[MAX_LEN];
    char nomRecherche[MAX_LEN];
    Produit p;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("Entrez le nom du produit a rechercher : ");
    scanf("%s", nomRecherche);

    printf("\nRésultats de la recherche :\n");
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%d,%d,%[^,],%[^,],%d,%f,%f,%d",
               &p.id, p.nom, p.description, p.categorie, &p.prix_unitaire,
               &p.quantite_stock, &p.seuil_alerte, p.date_derniere_entree,
               p.date_derniere_sortie, &p.id_fournisseur, &p.prix_achat,
               &p.benefice, &p.quantite_achetee);
        if (strcmp(p.nom, nomRecherche) == 0) {
            printf("| ID  | Nom          | Description        | Categorie | Prix   | Quantite | Seuil | Fournisseur |\n");
            printf("------------------------------------------------------------------------------------\n");
            printf("| %-3d | %-12s | %-18s | %-9s | %-6.2f | %-8d | %-5d | %-10d |\n",
                   p.id, p.nom, p.description, p.categorie, p.prix_unitaire,
                   p.quantite_stock, p.seuil_alerte, p.id_fournisseur);
        }
    }

    fclose(fp);
}

int comparerPrix(const void *a, const void *b) {
    Produit *produitA = (Produit *)a;
    Produit *produitB = (Produit *)b;
    return (produitA->prix_unitaire - produitB->prix_unitaire);
}

void afficherProduitsTriesParPrixCroissant() {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit produits[MAX_LEN];
    int count = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%d,%d,%[^,],%[^,],%d,%f,%f,%d",
               &produits[count].id, produits[count].nom, produits[count].description, produits[count].categorie,
               &produits[count].prix_unitaire, &produits[count].quantite_stock, &produits[count].seuil_alerte,
               produits[count].date_derniere_entree, produits[count].date_derniere_sortie, &produits[count].id_fournisseur,
               &produits[count].prix_achat, &produits[count].benefice, &produits[count].quantite_achetee);
        count++;
    }

    qsort(produits, count, sizeof(Produit), comparerPrix);

    printf("\nProduits triés par prix croissant:\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("| ID  | Nom          | Description        | Categorie | Prix   | Quantite | Seuil | Fournisseur |\n");
    printf("------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-12s | %-18s | %-9s | %-6.2f | %-8d | %-5d | %-10d |\n",
               produits[i].id, produits[i].nom, produits[i].description, produits[i].categorie, produits[i].prix_unitaire,
               produits[i].quantite_stock, produits[i].seuil_alerte, produits[i].id_fournisseur);
    }
    printf("------------------------------------------------------------------------------------\n");

    fclose(fp);
}

void afficherProduitsTriesParPrixDecroissant() {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit produits[MAX_LEN];
    int count = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%d,%d,%[^,],%[^,],%d,%f,%f,%d",
               &produits[count].id, produits[count].nom, produits[count].description, produits[count].categorie,
               &produits[count].prix_unitaire, &produits[count].quantite_stock, &produits[count].seuil_alerte,
               produits[count].date_derniere_entree, produits[count].date_derniere_sortie, &produits[count].id_fournisseur,
               &produits[count].prix_achat, &produits[count].benefice, &produits[count].quantite_achetee);
        count++;
    }

    qsort(produits, count, sizeof(Produit), comparerPrix);
    for (int i = 0; i < count / 2; i++) {
        Produit temp = produits[i];
        produits[i] = produits[count - i - 1];
        produits[count - i - 1] = temp;
    }

    printf("\nProduits tries par prix decroissant:\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("| ID  | Nom          | Description        | Catégorie | Prix   | Quantite | Seuil | Fournisseur |\n");
    printf("------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-12s | %-18s | %-9s | %-6.2f | %-8d | %-5d | %-10d |\n",
               produits[i].id, produits[i].nom, produits[i].description, produits[i].categorie, produits[i].prix_unitaire,
               produits[i].quantite_stock, produits[i].seuil_alerte, produits[i].id_fournisseur);
    }
    printf("------------------------------------------------------------------------------------\n");

    fclose(fp);
}

void modifierProduit(char nom[]) {
    FILE *fp, *temp;
    char buffer[MAX_LEN];
    Produit p;

    fp = fopen(FILENAME, "r");
    temp = fopen("temp.csv", "w");
    if (fp == NULL || temp == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    bool produitTrouve = false;

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%f,%d,%d,%[^,],%[^,],%d", &p.id, p.nom, p.description, p.categorie, &p.prix_unitaire,
               &p.quantite_stock, &p.seuil_alerte, p.date_derniere_entree, p.date_derniere_sortie,
               &p.id_fournisseur);
        if (strcmp(p.nom, nom) == 0) {
            produitTrouve = true;
            printf("Entrez les nouvelles informations pour le produit \"%s\":\n", nom);
            printf("ID: %d\n", p.id);
            printf("Nouveau nom: ");
            scanf("%s", p.nom);
            printf("Nouvelle description: ");
            scanf("%s", p.description);
            printf("Nouvelle categorie: ");
            scanf("%s", p.categorie);
            printf("Nouveau prix unitaire: ");
            scanf("%f", &p.prix_unitaire);
            printf("Nouvelle quantité en stock: ");
            scanf("%d", &p.quantite_stock);
            printf("Nouveau seuil d'alerte de stock: ");
            scanf("%d", &p.seuil_alerte);
            printf("Nouvelle date de derniere entree: ");
            scanf("%s", p.date_derniere_entree);
            printf("Nouvelle date de derniere sortie: ");
            scanf("%s", p.date_derniere_sortie);
            printf("Nouvel ID de fournisseur: ");
            scanf("%d", &p.id_fournisseur);
        }
        fprintf(temp, "%d,%s,%s,%s,%.2f,%d,%d,%s,%s,%d\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire,
                p.quantite_stock, p.seuil_alerte, p.date_derniere_entree, p.date_derniere_sortie,
                p.id_fournisseur);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if (!produitTrouve) {
        printf("Le produit avec le nom \"%s\" n'existe pas dans le stock.\n", nom);
    } else {
        printf("Produit modifie avec succes.\n");
    }
}

int main() {
    int choix, id, quantite;
    Produit p;
    bool est_gerant = false;

    printf("Êtes-vous un client (c) ou un gerant (g)? ");
    char statut;
    fgets(&statut, MAX_LEN, stdin);

    if (statut == 'g') {
        est_gerant = true;
        if (!verifierMotDePasse()) {
            return 0;
        }
    }

    do {
        if (est_gerant) {
            afficherMenuGerant();
        } else {
            afficherMenuClient();
        }
        printf("Entrez votre choix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                if (est_gerant) {
                    printf("Entrez les informations du produit:\n");
                    printf("ID: ");
                    scanf("%d", &p.id);
                    printf("Nom: ");
                    scanf("%s", p.nom);
                    printf("Description: ");
                    scanf("%s", p.description);
                    printf("Categorie: ");
                    scanf("%s", p.categorie);
                    printf("Prix unitaire: ");
                    scanf("%f", &p.prix_unitaire);
                    printf("Quantite en stock: ");
                    scanf("%d", &p.quantite_stock);
                    printf("Seuil d'alerte de stock: ");
                    scanf("%d", &p.seuil_alerte);
                    printf("Date de derniere entrée: ");
                    scanf("%s", p.date_derniere_entree);
                    printf("Date de derniere sortie: ");
                    scanf("%s", p.date_derniere_sortie);
                    printf("ID de fournisseur: ");
                    scanf("%d", &p.id_fournisseur);
                    ajouterProduit(p);
                } else {
                    afficherStock();
                }
                break;
            case 2:
                if (!est_gerant) {
                    afficherCategoriesDisponibles();
                    printf("Entrez le nom de la categorie choisie: ");
                    char categorie_choisie[MAX_LEN];
                    scanf("%s", categorie_choisie);
                    afficherProduitsParCategorie(categorie_choisie);
                } else {
                    printf("Entrez l'ID du produit a supprimer: ");
                    scanf("%d", &id);
                    supprimerProduit(id);
                }
                break;
            case 3:
                if (!est_gerant) {
                    printf("Entrez l'ID du produit a acheter: ");
                    scanf("%d", &id);
                    printf("Entrez la quantité a acheter: ");
                    scanf("%d", &quantite);
                    acheterProduit(id, quantite);
                } else {
                    afficherStock();
                }
                break;
             case 4:
                if (!est_gerant) {
                    rechercherProduitParNom();
                } else {
                    printf("Entrez le nom du produit a modifier: ");
                char nomProduit[MAX_LEN];
                scanf("%s", nomProduit);
                modifierProduit(nomProduit);
                }
                break;
            case 5:
                if (!est_gerant) {
                    afficherProduitsTriesParPrixCroissant();
                } else {
                    afficherBeneficesEtQuantiteAchete();
                }
                break;
            case 6:
                if (!est_gerant) {
                    afficherProduitsTriesParPrixDecroissant();
                } else {
                    printf("Programme termine.\n");
                    return 0;
                }
                break;
            case 7:
                    printf("Programme terminé.\n");
                break;
            default:
                printf("Choix invalide. Veuillez choisir un nombre entre 1 et 7.\n");
        }
    } while (choix != 7);

    return 0;
}
