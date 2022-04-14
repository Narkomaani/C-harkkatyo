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


typedef struct tuontanto
{
    int kuukausi;
    double maara;
    struct tuontanto *pNext;
} Tuotanto;

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
    //fgets(rivi, sizeof(rivi), tiedosto); // luetaan otsikkorivi pois
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

        //printf("\nrivi: %s\nptr: %s\n", rivi, pUusi->tieto);
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

    // itse looppi
    while (ptr->pNext != NULL) {
        
        //printf("looppi; %s\n", ptr->tieto);
        
        if ((pRiviData = (Data*)malloc(sizeof(Data))) == NULL) {
            perror("Muistin varaus epäonnistui");
            exit(1);
        }

        strcpy(rivi, ptr->tieto);
        pRiviData->aika = strtok(rivi, ";");
        strtok(NULL,";");
        pRiviData->kulutus = atof(strtok(NULL, ";"));

        // Pienimmän ja isoimman mittaustuloksen loopit
        if ( pRiviData->kulutus > pData->kulutus ) {
            pData->maxKulutus = pRiviData->kulutus;
            strcpy(pData->maxAika, pRiviData->aika);
        }
        if ( pRiviData->kulutus < pData->kulutus ) {
            pData->minKulutus = pRiviData->kulutus;
            strcpy(pData->minAika, pRiviData->aika);
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
Tuotanto *analysoiKK(LData *lista, Tuotanto *pAlku) {
    
    Tuotanto *pLoppu = NULL, *pUusi;
    int i = 1, kuukausi = 0, intLista[12];
    double kulutus;
    char rivi[255];
    LData *ptr = lista;

    while (ptr->pNext != NULL) {
        kulutus = 0;
        strcpy(rivi, ptr->tieto);
        strtok(rivi,".");
        kuukausi = atoi(strtok(NULL,"."));
        strtok(NULL,";"); // Loput kuukaudesta
        strtok(NULL,";"); // viikko
        strtok(NULL,";"); // Kulutus
        kulutus = atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));
        kulutus = kulutus + atof(strtok(NULL,";"));

        intLista[kuukausi-1] += kulutus;
    }

    /*
    while (kuukausi < 13) {
        

        // Rivin kopiointi ja pilkkominen muuttujiin
        if (ptr->pNext != NULL) {
            kulutus = 0;
            strcpy(rivi, ptr->tieto);
            strtok(rivi,".");
            kuukausi = atoi(strtok(NULL,"."));
            strtok(NULL,";"); // Loput kuukaudesta
            strtok(NULL,";"); // viikko
            strtok(NULL,";"); // Kulutus
            kulutus = atof(strtok(NULL,";"));
            kulutus = kulutus + atof(strtok(NULL,";"));
            kulutus = kulutus + atof(strtok(NULL,";"));
            kulutus = kulutus + atof(strtok(NULL,";"));
            kulutus = kulutus + atof(strtok(NULL,";"));
            kulutus = kulutus + atof(strtok(NULL,";"));
        } else {
            kuukausi = 0;
            kulutus = 0;
        }

        // Tarkistetaan kuukausi
        if ( kuukausi == pUusi->kuukausi) {
            pUusi->maara += kulutus;
            i++;
        } else {
            
            if ((pUusi = (Tuotanto*)malloc(sizeof(Tuotanto))) == NULL) {
                perror("Muistin varaus epäonnistui");
                exit(0);
            }
         
            pUusi->pNext = NULL;

            // Tarkastetaan onko tyhjä lista, luodaan uusi 
            if (pAlku == NULL) {
                pUusi->kuukausi = kuukausi;
                pUusi->maara = 0;
                pAlku = pUusi;
                pLoppu = pUusi;

            } else {
                pUusi->kuukausi = pLoppu->kuukausi+1;
                if ( pUusi->kuukausi == 13) {
                    break;
                }
                pUusi->maara = 0;
                pLoppu->pNext = pUusi;
                pLoppu = pUusi;
                kuukausi = pUusi->kuukausi;
            }

            ptr = ptr->pNext;
        }
    } */

    printf("Kuukausittaiset tuotannot analysoitu.\n");
    for (int i = 0; i<sizeof(intLista); i++) {
        printf("index %d: %d", i, intLista[i]);
    }
    return pAlku;
}

void kirjoitaTiedosto(Data *dLista, Tuotanto *tLista, char nimi[]) {

    FILE *tiedosto;
    Tuotanto *ptr = tLista;
    double offset = 1, maara; // jostain syystä normi määrän arvot ei ole oikein viopessa


    printf("Tilastotiedot %.0f mittaustuloksesta:\n", dLista->maara);
    printf("Kulutus oli yhteensä %.0f kWh, ja keskimäärin %.1f kWh.\n", dLista->summa, dLista->keskiarvo);
    printf("Suurin kulutus, %.0f kWh, tapahtui %s.\n",dLista->maxKulutus, dLista->maxAika);
    printf("Pienin kulutus, %.0f kWh, tapahtui %s.\n", dLista->minKulutus, dLista->minAika);

    printf("\nPvm;Tuotanto (GWh)\n");
    while (ptr != NULL) {
        maara = (double)ptr->maara/(double)1000000;
        maara = maara*offset;
        printf("Kk %d;%.2f\n", ptr->kuukausi, maara);
        ptr = ptr->pNext;
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
    ptr = tLista;
    while (ptr != NULL) {
        maara = (double)ptr->maara/(double)1000000;
        maara = maara*offset;
        fprintf(tiedosto, "Kk %d;%.2f\n", ptr->kuukausi, maara);
        ptr = ptr->pNext;
    }

    printf("Tiedosto '%s' kirjoitettu.\n", nimi);
    fclose(tiedosto);

    return;
}

void vapautaTuotanto(Tuotanto *tLista) {
    Tuotanto *tPtr;
    tPtr = tLista;
    while (tPtr != NULL) {
        tLista = tPtr->pNext;
        free(tPtr);
        tPtr = tLista;
    }

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
