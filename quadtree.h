typedef struct {
    unsigned char valoarea_pentru_rosu;
    unsigned char valoarea_pentru_verde;
    unsigned char valoarea_pentru_albastru;
} RGB;

typedef struct nod {
    RGB info;
    unsigned char tipul_nodului;
    struct nod *left, *right, *down_right, *down_left;
} TNod, *TArb;

typedef struct celula {
    TArb nod;
    struct celula* urm;
} TCelula, *TLista;

typedef struct coada {
    TLista inc, sf; /* adresa primei si ultimei celule */
} TCoada;
TArb ConstrFr(TArb x); /* -> adresa frunza cu informatia x, sau NULL daca nu
                         exista spatiu */

void DistrugeNoduri(TArb r); /* functie auxiliara - distruge toate nodurile
                              */

void DistrugeArbore(
    TArb* a); /* distruge toate nodurile arborelui de la adresa a */

RGB* Should_I_DivideQuadTree(RGB** imagine, unsigned long long x, unsigned long long y,
                             unsigned long long size, int prag, unsigned long long* numar_de_blocuri, int* latura_patrat_maxima);

TArb DivideQuadTree(RGB** imagine, TArb arb, unsigned long long x, unsigned long long y,
                    unsigned long long size, int prag,
                    unsigned long long* numar_de_blocuri,
                    int* latura_patrat_maxima);

int Inaltime_QuadTree(TArb root);
TCoada* InitQ();
TArb IntrQ(TCoada* c, TArb x);
TArb ExtrQ(TCoada* c, TArb x);
void adauga_in_coada(TCoada* coada, TArb root);
TArb extrage_din_coada(TCoada* coada);
int Numara_Noduri(TArb root);
void DistrQ(TCoada** c);
void Constructie_imagine(RGB** imagine, TArb arbore, int x, int y, int size);
