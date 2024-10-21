
#include <stdio.h>
#include <stdlib.h>

#include "quadtree.h"

TArb ConstrFr(TArb arb) /* -> adresa frunza cu informatia x, sau
                                    NULL daca nu exista spatiu */
{
    arb = (TArb)malloc(sizeof(TNod));
    if (!arb) return NULL;
    arb->tipul_nodului = 0;
    arb->left = arb->right = arb->down_right = arb->down_left = NULL;
    return arb;
}

void DistrugeNoduri(TArb r) /* functie auxiliara - distruge toate nodurile */
{
    if (!r) return;
    DistrugeNoduri(r->left);      /* distruge subarborele stang */
    DistrugeNoduri(r->down_left); /* distruge subarborele drept */
    DistrugeNoduri(r->right);
    DistrugeNoduri(r->down_right);
    free(r);
}

void DistrugeArbore(
    TArb* a) {          /* distruge toate nodurile arborelui de la adresa a */
    if (!(*a)) return;  /* arbore deja vid */
    DistrugeNoduri(*a); /* distruge toate nodurile */
    *a = NULL;          /* arborele este vid */
}

RGB* Should_I_DivideQuadTree(RGB** imagine, unsigned long long x, unsigned long long y,
                             unsigned long long size, int prag, unsigned long long* numar_de_blocuri, int* latura_patrat_maxima) {
    /*functie care imi returneaza o structura RGB
    daca sunt pe o frunza si nu mai trebuie sa impart matricea*/
    unsigned long long i, j;
    unsigned long long red = 0, green = 0, blue = 0, mean = 0;
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            red += imagine[i][j].valoarea_pentru_rosu;
            green += imagine[i][j].valoarea_pentru_verde;
            blue += imagine[i][j].valoarea_pentru_albastru;
        }
    }
    red = red / (size * size);
    green = green / (size * size);
    blue = blue / (size * size);
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            mean += (red - imagine[i][j].valoarea_pentru_rosu) *
                        (red - imagine[i][j].valoarea_pentru_rosu) +
                    (green - imagine[i][j].valoarea_pentru_verde) *
                        (green - imagine[i][j].valoarea_pentru_verde) +
                    (blue - imagine[i][j].valoarea_pentru_albastru) *
                        (blue - imagine[i][j].valoarea_pentru_albastru);
        }
    }
    mean = mean / (3 * (size * size));
    if (mean <= prag) {
        if (size > *latura_patrat_maxima)
            *latura_patrat_maxima = size;
        (*numar_de_blocuri)++;
        RGB* valori_medii;
        valori_medii = malloc(sizeof(RGB));
        valori_medii->valoarea_pentru_rosu = red;
        valori_medii->valoarea_pentru_verde = green;
        valori_medii->valoarea_pentru_albastru = blue;
        return valori_medii;
    } else {
        return NULL;
    }
}

TArb DivideQuadTree(RGB** imagine, TArb arb, unsigned long long x, unsigned long long y,
                    unsigned long long size, int prag,
                    unsigned long long* numar_de_blocuri,
                    int* latura_patrat_maxima) {
    RGB* valori = Should_I_DivideQuadTree(imagine, x, y, size, prag, numar_de_blocuri, latura_patrat_maxima);
    if (valori != NULL) {
        arb->info.valoarea_pentru_albastru = valori->valoarea_pentru_albastru;
        arb->info.valoarea_pentru_rosu = valori->valoarea_pentru_rosu;
        arb->info.valoarea_pentru_verde = valori->valoarea_pentru_verde;
        arb->tipul_nodului = 1;
        free(valori);
    } else {
        /*daca functia returneaza null impart matricea
        dupa fiecare zona in parte recursiv pana
        nu mai este nevoie de impartire*/
        arb->left = ConstrFr(arb->left);
        arb->left->tipul_nodului = 0;
        DivideQuadTree(imagine, arb->left, x, y, size / 2, prag,
                       numar_de_blocuri, latura_patrat_maxima);
        arb->right = ConstrFr(arb->right);
        arb->right->tipul_nodului = 0;
        DivideQuadTree(imagine, arb->right, x, y + (size / 2), size / 2, prag,
                       numar_de_blocuri, latura_patrat_maxima);
        arb->down_right = ConstrFr(arb->down_right);
        arb->down_right->tipul_nodului = 0;
        DivideQuadTree(imagine, arb->down_right, x + (size / 2), y + (size / 2),
                       size / 2, prag, numar_de_blocuri, latura_patrat_maxima);
        arb->down_left = ConstrFr(arb->down_left);
        arb->down_left->tipul_nodului = 0;
        DivideQuadTree(imagine, arb->down_left, x + (size / 2), y, size / 2,
                       prag, numar_de_blocuri, latura_patrat_maxima);
    }
}
void Constructie_imagine(RGB** imagine, TArb arb, int x, int y, int size) {
    int i = 0, j = 0;
    if (arb->tipul_nodului == 0) {
        Constructie_imagine(imagine, arb->left, x, y, size / 2);
        Constructie_imagine(imagine, arb->right, x, y + (size / 2), size / 2);
        Constructie_imagine(imagine, arb->down_right, x + (size / 2), y + (size / 2),
                            size / 2);
        Constructie_imagine(imagine, arb->down_left, x + (size / 2), y, size / 2);
    } else {
        for (i = x; i < x + size; i++) {
            for (j = y; j < y + size; j++)
                imagine[i][j] = arb->info;
        }
    }
}
int Inaltime_QuadTree(TArb root) {
    /*calculez inaltimea arborelui*/
    int inaltime1 = 0, inaltime2 = 0, inaltime3 = 0, inaltime4 = 0;
    if (!root) return 0;
    inaltime1 = Inaltime_QuadTree(root->left);
    inaltime2 = Inaltime_QuadTree(root->right);
    inaltime3 = Inaltime_QuadTree(root->down_right);
    inaltime4 = Inaltime_QuadTree(root->down_left);
    int max1 = (inaltime1 >= inaltime2 ? inaltime1 : inaltime2);
    int max2 = (inaltime3 >= inaltime4 ? inaltime3 : inaltime4);
    return 1 + (max1 >= max2 ? max1 : max2);
}

TCoada* InitQ() /* creeaza coada vida cu elemente de dimensiune d;
                                 anumite implementari pot necesita si alti parametri */
{
    TCoada* c; /* spatiu pentru descriptor coada */
    c = (TCoada*)malloc(sizeof(TCoada));
    c->inc = NULL;
    c->sf = NULL;
}

void adauga_in_coada(TCoada* coada, TArb root) {
    TCelula* celula = (TCelula*)malloc(sizeof(TCelula));
    celula->nod = root;
    celula->urm = NULL;
    if (coada->inc == NULL) {
        coada->inc = celula;
        coada->sf = celula;
    } else {
        coada->sf->urm = celula;
        coada->sf = celula;
    }
    /*adaug in coada un element de tipul
    TArbore*/
}

TArb extrage_din_coada(TCoada* coada) {
    TArb root = coada->inc->nod;
    TCelula* celula_stearsa = coada->inc;
    coada->inc = coada->inc->urm;
    if (coada->inc == NULL) {
        coada->sf = NULL;
    }
    free(celula_stearsa);
    return root;
    /*extrag din coada un element
    de tipul TArbore*/
}
void DistrQ(TCoada** c) /* distruge coada */
{
    TLista p, aux;
    p = (*c)->inc;
    while (p) {
        aux = p;
        p = p->urm;
        free(aux);
    }
    free(*c);
    *c = NULL;
}

int Numara_Noduri(TArb root) {
    if (!root) return 0;
    return 1 + Numara_Noduri(root->left) + Numara_Noduri(root->right) + Numara_Noduri(root->down_right) + Numara_Noduri(root->down_left);
}