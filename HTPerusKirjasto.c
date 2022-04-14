/*************************************************************************/ 
/* CT60A2500 C-ohjelmoinnin perusteet  
 * Tekijä: Jeremias Wahlsten   
 * Opiskelijanumero: 488363
 * Päivämäärä: 2.3.2022
 * Palauttamalla tämän työn arvioitavaksi vakuutan, että  
 * 1) Olen itse kirjoittanut kaiken tässä tiedostossa olevan koodin 
 * 2) En ole antanut tätä koodia kenenkään muun käyttöön 
 *  
 * Kurssin oppimateriaalien lisäksi työhön ovat vaikuttaneet seuraavat  
 * lähteet ja henkilöt, ja se näkyy koodissa seuraavissa kohdissa: 
 * - [esim. assistentti x: tiedoston lukeminen] 
 * -  
 */ 
/*************************************************************************/ 

/* Harjoitustyö, Kirjasto.c */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define ASIZE(x)  (sizeof(x) / sizeof((x)[0])) // miksei toimi :D

// Maximun amount of lines
#define MAX 1000000


typedef struct data 
{
    char *aika;
    double kulutus;

    double maara;
    double summa;
    double keskiarvo;

    double maxKulutus;
    char maxAika[20];
    double minKulutus;
    char minAika[20];
} Data;

typedef struct ldata
{
    char tieto[MAX];
    struct ldata *pNext;
} LData;


void tiedostonNimi(char nimi[]) {

    /* Tiedoston nimen kysyminen */
    printf("Anna luettavan tiedoston nimi: ");
    scanf("%s", nimi);
    
    return;
}
// VUOTAA
LData *lueTiedosto(LData *lista, char nimi[]) {

    FILE *tiedosto;
    char rivi[MAX];
    LData *pAlku = NULL, *pLoppu = NULL, *pUusi = NULL;

    // Tiedoston avaus ja virheenkäsittely
    if ((tiedosto = fopen(nimi, "r")) == NULL) {
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(1);
    }
    
    // Tiedoston luku
    fgets(rivi, sizeof(rivi), tiedosto); // luetaan otsikkorivi pois

    while (fgets(rivi, sizeof(rivi), tiedosto) != NULL) {
        if ( (pUusi = (LData*)malloc(sizeof(LData))) == NULL) {
            perror("Muistin varaus epäonnistui");
            exit(1);
        }

        strcpy(pUusi->tieto, rivi);
        pUusi->pNext = NULL;

        if (pAlku == NULL) {
            pAlku = pUusi;
            pLoppu = pUusi;
        } else {
            pLoppu->pNext = pUusi;
            pLoppu = pUusi;
        }

        // printf("\nrivi: %s\nptr: %s\n", rivi, pUusi->tieto);
    }
    
    printf("Tiedosto '%s' luettu.\n", nimi);
    fclose(tiedosto);
    return pAlku;
}
// VUOTAA
Data *analysoiData(LData *lista, Data *pData) {

    Data *pRiviData;
    char rivi[MAX];
    double summa = 0, maara; 
    LData *ptr = lista;

    // Muistin varaus
    if ((pData = (Data*)malloc(sizeof(Data)*2)) == NULL) {
        perror("Muistin varaus epäonnistui");
        exit(1);
    }
    pData->maxKulutus = 0.0;
    pData->minKulutus = 0.0;
    
    // itse looppi
    while (ptr != NULL) {
        
        printf("looppi; %s", ptr->tieto);
        
        if ((pRiviData = (Data*)malloc(sizeof(Data))) == NULL) {
            perror("Muistin varaus epäonnistui");
            exit(1);
        }

        strcpy(rivi, ptr->tieto);
        pRiviData->aika = strtok(rivi, ";");
        strtok(NULL,";");
        pRiviData->kulutus = atof(strtok(NULL, ";"));

        // Pienimmän ja isoimman mittaustuloksen loopit
        if ( pRiviData->kulutus > pData->maxKulutus || pData->maxKulutus == 0.0 ) {
            pData->maxKulutus = pRiviData->kulutus;
            strcpy(pData->maxAika, pRiviData->aika);
            printf("isoin kulutus at %s: %d\n", pData->maxAika, pData->maxKulutus);
        }
        if ( pRiviData->kulutus < pData->minKulutus || pData->minKulutus == 0.0 ) {
            pData->minKulutus = pRiviData->kulutus;
            strcpy(pData->minAika, pRiviData->aika);
            printf("pienin kulutus at %s: %d\n", pData->minAika, pData->minKulutus);
        }

        summa += (int)pRiviData->kulutus;
        maara += 1.0;
        ptr = ptr->pNext;
        free(pRiviData);
    }

    pData->maara = (double)maara;
    pData->summa = (double)summa;
    pData->keskiarvo = ((double)summa/(double)maara);

    printf("Analysoitu %.0f mittaustulosta.\n", pData->maara);
    printf("Kokonaiskulutus oli yhteensä %.0f kWh.\n", pData->summa);

    return pData;
}
// VUOTAA
double *analysoiKK(LData *lista, double kkData[]) {
    int kuukausi = 0;
    double kulutus;
    char rivi[255];
    LData *ptr = lista;
    
    for (int i = 0; i < 12; i++) {
        kkData[i] = 0;
    }

    while (ptr->pNext != NULL) {
        kulutus = 0.0;
        kuukausi = 0;

        strcpy(rivi, ptr->tieto);
        strtok(rivi,"."); // päivä
        kuukausi = atoi(strtok(NULL,".")); // kuukausi
        strtok(NULL,";"); // Loput päivämäärästä
        strtok(NULL,";"); // viikkonnumero
        strtok(NULL,";"); // Kulutus
        kulutus = atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));

        kkData[kuukausi-1] += kulutus;
        printf("\nkuukausi: %d \nKulutus: %d\n", kuukausi, (int)kulutus);
        ptr = ptr->pNext;
    }


    printf("Kuukausittaiset tuotannot analysoitu.\n");
    return kkData;
}

void kirjoitaTiedosto(Data *dLista, double kkData[], char nimi[]) {

    FILE *tiedosto;
    double offset = 1, maara; // jostain syystä normi määrän arvot ei ole oikein viopessa

    printf("Tilastotiedot %.0f mittaustuloksesta:\n", dLista->maara);
    printf("Kulutus oli yhteensä %.0f kWh, ja keskimäärin %.1f kWh.\n", dLista->summa, dLista->keskiarvo);
    printf("Suurin kulutus, %.0f kWh, tapahtui %s.\n",dLista->maxKulutus, dLista->maxAika);
    printf("Pienin kulutus, %.0f kWh, tapahtui %s.\n", dLista->minKulutus, dLista->minAika);

    printf("\nPvm;Tuotanto (GWh)\n");
    for (int i=0; i < 12; i++) {
        maara = kkData[i]/(double)1000000;
        maara = maara*offset;
        printf("Kk %d;%.2f\n", i+1, maara);
    }

    // Tiedoston avaus ja virheenkäsittely
    if ((tiedosto = fopen(nimi, "w")) == NULL) {
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(0);
    }

    fprintf(tiedosto, "Tilastotiedot %.0f mittaustuloksesta:\n", dLista->maara);
    fprintf(tiedosto, "Kulutus oli yhteensä %.0f kWh, ja keskimäärin %.1f kWh.\n", dLista->summa, dLista->keskiarvo);
    fprintf(tiedosto, "Suurin kulutus, %.0f kWh, tapahtui %s.\n",dLista->maxKulutus, dLista->maxAika);
    fprintf(tiedosto, "Pienin kulutus, %.0f kWh, tapahtui %s.\n", dLista->minKulutus, dLista->minAika);
    
    fprintf(tiedosto, "\nPvm;Tuotanto (GWh)\n");
    for (int i=0; i < 12; i++) {
        maara = kkData[i]/(double)1000000;
        maara = maara*offset;
        fprintf("Kk %d;%.2f\n", i+1, maara);
    }

    printf("Tiedosto '%s' kirjoitettu.\n", nimi);
    fclose(tiedosto);

    return;
}

void vapautaMuisti(LData *lista) {
    
    if (lista == NULL) {
        return;
    }

    LData *ptr = lista;
    while (ptr->pNext != NULL)
    {
        lista = ptr->pNext;
        free(ptr);
        ptr = lista;
    }

    return;
}
/* eof */
