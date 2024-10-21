/*Bolfa Robert-Ion 314 CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quadtree.h"
int main(int argc, char const *argv[]) {
    if (strcmp(argv[1], "-c1") == 0) {
        FILE *in = fopen(argv[3], "rb");
        if (in == NULL) {
            printf("Eroare: Nu a reusit deschiderea fisierului 'in'");
            return 1;
        }
        FILE *out = fopen(argv[4], "wb");
        if (out == NULL) {
            printf("Eroare: Nu a reusit deschiderea fisierului 'out'");
            return 1;
        }

        int prag = atoi(argv[2]);
        char file_type[3];
        fread(&file_type, sizeof(file_type), 1, in);
        unsigned width, height, valoare_maxima_culoare;
        fscanf(in, "%d %d %d", &width, &height, &valoare_maxima_culoare);
        getc(in);
        RGB **imagine;
        imagine = (RGB **)malloc(sizeof(RGB *) * height);
        int i = 0, j = 0;
        for (i = 0; i < height; i++) {
            imagine[i] = (RGB *)malloc(sizeof(RGB) * width);
        }
        RGB pixel;
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                fread(&pixel, sizeof(RGB), 1, in);
                imagine[i][j] = pixel;
            }
        }
        unsigned long long numar_de_blocuri = 0;
        int latura_patrat_maxima = 0;
        TArb arbore = ConstrFr(arbore);
        DivideQuadTree(imagine, arbore, 0, 0, width, prag, &numar_de_blocuri,
                       &latura_patrat_maxima);
        int Inaltime = Inaltime_QuadTree(arbore);
        fprintf(out, "%d\n", Inaltime);
        fprintf(out, "%lld\n", numar_de_blocuri);
        fprintf(out, "%d\n", latura_patrat_maxima);
        for (i = 0; i < height; i++) {
            free(imagine[i]);
        }
        DistrugeNoduri(arbore);
        free(imagine);
        fclose(in);
        fclose(out);
    }

    if (strcmp(argv[1], "-c2") == 0) {
        FILE *in = fopen(argv[3], "rb");
        if (in == NULL) {
            printf("Eroare: Nu a reusit deschiderea fisierului 'in'");
            return 1;
        }
        FILE *out = fopen(argv[4], "wb");
        if (out == NULL) {
            printf("Eroare: Nu a reusit deschiderea fisierului 'out'");
            return 1;
        }
        TCoada *Coada = InitQ();
        int prag = atoi(argv[2]);
        char file_type[3];
        fread(&file_type, sizeof(file_type), 1, in);
        unsigned width, height, valoare_maxima_culoare;
        fscanf(in, "%d %d %d", &width, &height, &valoare_maxima_culoare);
        getc(in);
        int i = 0, j = 0;
        RGB **imagine;
        imagine = (RGB **)malloc(sizeof(RGB *) * height);
        for (i = 0; i < height; i++) {
            imagine[i] = (RGB *)malloc(sizeof(RGB) * width);
        }
        RGB pixel;
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                fread(&pixel, sizeof(RGB), 1, in);
                imagine[i][j] = pixel;
            }
        }
        unsigned long long numar_de_blocuri = 0;
        int latura_patrat_maxima = 0;
        TArb arbore = ConstrFr(arbore);
        DivideQuadTree(imagine, arbore, 0, 0, width, prag, &numar_de_blocuri,
                       &latura_patrat_maxima);
        int Numar_noduri = Numara_Noduri(arbore);
        TArb root;
        fwrite(&width, sizeof(width), 1, out);
        if (arbore->tipul_nodului == 0) {
            adauga_in_coada(Coada, arbore);
            while (Numar_noduri != 0) {
                root = extrage_din_coada(Coada);
                if (root->tipul_nodului == 1) {
                    fwrite(&root->tipul_nodului, sizeof(root->tipul_nodului), 1, out);
                    fwrite(&root->info.valoarea_pentru_rosu, sizeof(root->info.valoarea_pentru_rosu), 1, out);
                    fwrite(&root->info.valoarea_pentru_verde, sizeof(root->info.valoarea_pentru_verde), 1, out);
                    fwrite(&root->info.valoarea_pentru_albastru, sizeof(root->info.valoarea_pentru_albastru), 1, out);
                } else {
                    fwrite(&root->tipul_nodului, sizeof(root->tipul_nodului), 1, out);
                    adauga_in_coada(Coada, root->left);
                    adauga_in_coada(Coada, root->right);
                    adauga_in_coada(Coada, root->down_right);
                    adauga_in_coada(Coada, root->down_left);
                }
                Numar_noduri = Numar_noduri - 1;
            }
        } else {
            fwrite(&arbore->tipul_nodului, sizeof(arbore->tipul_nodului), 1, out);
            fwrite(&arbore->info.valoarea_pentru_rosu, sizeof(arbore->info.valoarea_pentru_rosu), 1, out);
            fwrite(&arbore->info.valoarea_pentru_verde, sizeof(arbore->info.valoarea_pentru_verde), 1, out);
            fwrite(&arbore->info.valoarea_pentru_albastru, sizeof(arbore->info.valoarea_pentru_albastru), 1, out);
        }
        for (i = 0; i < height; i++) {
            free(imagine[i]);
        }
        DistrQ(&Coada);
        DistrugeNoduri(arbore);
        free(imagine);
        fclose(in);
        fclose(out);
    }
    if (strcmp(argv[1], "-d") == 0) {
        FILE *in = fopen(argv[2], "rb");
        if (in == NULL) {
            printf("Eroare: Nu a reusit deschiderea fisierului 'in'");
            return 1;
        }
        FILE *out = fopen(argv[3], "wb");
        if (out == NULL) {
            printf("Eroare: Nu a reusit deschiderea fisierului 'out'");
            return 1;
        }
        unsigned int size = 0;
        fread(&size, sizeof(unsigned int), 1, in);
        int MaxColor = 255;
        fprintf(out, "P6\n%d %d\n%d\n", size, size, MaxColor);
        TCoada *Coada = InitQ();
        TArb arbore = ConstrFr(arbore);
        fread(&arbore->tipul_nodului, sizeof(unsigned char), 1, in);
        adauga_in_coada(Coada, arbore);
        while (Coada->inc != NULL) {
            TArb root = extrage_din_coada(Coada);
            if (root->tipul_nodului == 0) {
                root->left = ConstrFr(root->left);
                root->right = ConstrFr(root->right);
                root->down_right = ConstrFr(root->down_right);
                root->down_left = ConstrFr(root->down_left);
                if (!feof(in)) {
                    fread(&root->left->tipul_nodului, sizeof(unsigned char), 1, in);
                    if (root->left->tipul_nodului == 1) {
                        fread(&root->left->info.valoarea_pentru_rosu, sizeof(unsigned char), 1, in);
                        fread(&root->left->info.valoarea_pentru_verde, sizeof(unsigned char), 1, in);
                        fread(&root->left->info.valoarea_pentru_albastru, sizeof(unsigned char), 1, in);
                    } else if (root->left->tipul_nodului == 0) {
                        adauga_in_coada(Coada, root->left);
                    }
                }
                if (!feof(in)) {
                    fread(&root->right->tipul_nodului, sizeof(unsigned char), 1, in);
                    if (root->right->tipul_nodului == 1) {
                        fread(&root->right->info.valoarea_pentru_rosu, sizeof(unsigned char), 1, in);
                        fread(&root->right->info.valoarea_pentru_verde, sizeof(unsigned char), 1, in);
                        fread(&root->right->info.valoarea_pentru_albastru, sizeof(unsigned char), 1, in);
                    } else if (root->right->tipul_nodului == 0) {
                        adauga_in_coada(Coada, root->right);
                    }
                }
                if (!feof(in)) {
                    fread(&root->down_right->tipul_nodului, sizeof(unsigned char), 1, in);
                    if (root->down_right->tipul_nodului == 1) {
                        fread(&root->down_right->info.valoarea_pentru_rosu, sizeof(unsigned char), 1, in);
                        fread(&root->down_right->info.valoarea_pentru_verde, sizeof(unsigned char), 1, in);
                        fread(&root->down_right->info.valoarea_pentru_albastru, sizeof(unsigned char), 1, in);
                    } else if (root->down_right->tipul_nodului == 0) {
                        adauga_in_coada(Coada, root->down_right);
                    }
                }
                if (!feof(in)) {
                    fread(&root->down_left->tipul_nodului, sizeof(unsigned char), 1, in);
                    if (root->down_left->tipul_nodului == 1) {
                        fread(&root->down_left->info.valoarea_pentru_rosu, sizeof(unsigned char), 1, in);
                        fread(&root->down_left->info.valoarea_pentru_verde, sizeof(unsigned char), 1, in);
                        fread(&root->down_left->info.valoarea_pentru_albastru, sizeof(unsigned char), 1, in);
                    } else if (root->down_left->tipul_nodului == 0) {
                        adauga_in_coada(Coada, root->down_left);
                    }
                }
            }
        }
        RGB **imagine;
        int i = 0, j = 0;
        imagine = malloc(size * sizeof(RGB *));
        for (i = 0; i < size; i++) {
            imagine[i] = (RGB *)malloc(size * sizeof(RGB));
        }
        Constructie_imagine(imagine, arbore, 0, 0, size);
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                fwrite(&imagine[i][j].valoarea_pentru_rosu, sizeof(imagine[i][j].valoarea_pentru_rosu), 1, out);
                fwrite(&imagine[i][j].valoarea_pentru_verde, sizeof(imagine[i][j].valoarea_pentru_verde), 1, out);
                fwrite(&imagine[i][j].valoarea_pentru_albastru, sizeof(imagine[i][j].valoarea_pentru_albastru), 1, out);
            }
        }
        for (i = 0; i < size; i++) {
            free(imagine[i]);
        }
        DistrugeNoduri(arbore);
        free(imagine);
        fclose(in);
        fclose(out);
        DistrQ(&Coada);
    }
}