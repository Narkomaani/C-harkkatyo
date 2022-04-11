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

void tiedostonNimi(char nimi[]) {

    /* Tiedoston nimen kysyminen */
    printf("Anna luettavan tiedoston nimi: ");
    scanf("%s", nimi);
    
    return;
}

void lueTiedosto(char *sPtr[], char nimi[]) {

    FILE *tiedosto;
    char rivi[255];
    int i = 0;

    // Tiedoston avaus ja virheenkäsittely
    if ((tiedosto = fopen(nimi, "r")) == NULL) {
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(0);
    }

    // Tiedoston luku
    while (fgets(rivi, sizeof(rivi), tiedosto) != NULL) {
        if ( (sPtr[i] = malloc(sizeof(rivi)+1)) == NULL ) {
            perror("Muistin varaus epäonnistui");
            exit(0);
        }
        
        strcpy(sPtr[i], rivi);
        i++;
        
    }
    
    printf("Tiedosto '%s' luettu.\n", nimi);
    fclose(tiedosto);
    return;
}

Data *analysoiData(char *lista[]) {

    Data *pToinenData;
    Data *pRiviData, *pMax = NULL, *pMin = NULL;
    char rivi[MAX];
    int i = 1; // ensimmäinen alkio on otsikko, skip
    double summa = 0, maara; 

    // Muistin varaus
    if ((pToinenData = (Data*)malloc(sizeof(Data)*2)) == NULL) {
        perror("Muistin varaus epäonnistui");
        exit(0);
    }

    // itse looppi
    while (lista[i] != NULL) {

        if ((pRiviData = (Data*)malloc(sizeof(Data))) == NULL) {
            perror("Muistin varaus epäonnistui");
            exit(0);
        }

        strcpy(rivi, lista[i]);
        pRiviData->aika = strtok(rivi, ";");
        strtok(NULL,";");
        pRiviData->kulutus = atof(strtok(NULL, ";"));

        // Pienimmän ja isoimman mittaustuloksen loopit
        if (pMax == NULL || pRiviData->kulutus > pMax->kulutus ) {
            pMax = pRiviData;
            pMax->aika = pRiviData->aika;
            pMax->kulutus = pRiviData->kulutus;
            pToinenData->maxKulutus = pMax->kulutus;
            strcpy(pToinenData->maxAika, pRiviData->aika);
            
        }
        if ( pMin == NULL || pRiviData->kulutus < pMin->kulutus ) {
            pMin = pRiviData;
            pToinenData->minKulutus = pMin->kulutus;
            strcpy(pToinenData->minAika, pMin->aika);

        }

        summa += (int)pRiviData->kulutus;
        i++;
    }

    maara = i-1;

    pToinenData->maara = (double)maara;
    pToinenData->summa = (double)summa;
    pToinenData->keskiarvo = ((double)summa/(double)maara);

    printf("Analysoitu %.0f mittaustulosta.\n", pToinenData->maara);
    printf("Kokonaiskulutus oli yhteensä %.0f kWh.\n", pToinenData->summa);

    return pToinenData;
}

Tuotanto *analysoiKK(char *lista[]) {
    
    Tuotanto *pAlku = NULL, *pLoppu = NULL;
    Tuotanto *pUusi;
    int i = 1, kuukausi = 0;
    double kulutus;
    char rivi[255];

    while (kuukausi < 13) {
        

        // Rivin kopiointi ja pilkkominen muuttujiin
        if (lista[i] != NULL) {
            kulutus = 0;
            strcpy(rivi, lista[i]);
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
        }

        
    }

    printf("Kuukausittaiset tuotannot analysoitu.\n");
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

void vapautaMuisti(char *lista[]) {
    for (int i=0; i < sizeof(lista);i++) {
        free(lista[i]);
    }

    return;
}
/* eof */
