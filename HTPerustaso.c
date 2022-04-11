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
/* Harjoitustyö, Main.c */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HTPerusKirjasto.h"


int valikko() {
    int valinta;
    printf("Valitse haluamasi toiminto:\n");
    printf("1) Lue tiedosto\n");
    printf("2) Analysoi tiedot\n");
    printf("3) Kirjoita tulokset\n");
    printf("0) Lopeta\n");
    printf("Anna valintasi: ");
    scanf("%d", &valinta);
    getchar();

    return valinta;
}

int main(int argc, char *argv[]) {
    int valinta = -1;
    char *lista[10000], nimiTiedosto[50];
    lista[0] = '\0';
    Data *data = NULL;
    Tuotanto *tLista = NULL;

    do
    {
        valinta = valikko();

        if ( valinta == 1) {
            printf("Anna luettavan tiedoston nimi: ");
            scanf("%s", nimiTiedosto);
            vapautaMuisti(lista);
            lueTiedosto(lista, nimiTiedosto);
        } 
        
        else if ( valinta == 2 ) {
            if (lista[0] == '\0') {
                printf("Ei analysoitavaa, lue tiedosto ennen analyysiä.\n");
            } else {
                data = analysoiData(lista);
                tLista = analysoiKK(lista);
            }
        }

        else if ( valinta == 3 ) {
            if (data == NULL || tLista == NULL) {
                printf("Ei kirjoitettavia tietoja, analysoi tiedot ennen tallennusta.\n");
            } else {
                printf("Anna kirjoitettavan tiedoston nimi: ");
                scanf("%s", nimiTiedosto);
                kirjoitaTiedosto(data, tLista, nimiTiedosto);
            }
        }

        else if ( valinta == 0 ) {
            continue;
        }

        else {
            printf("Tuntematon valinta, yritä uudestaan.\n");
        }

        printf("\n");
        nimiTiedosto[0] = "\0"; 

    } while (valinta != 0);
    
    vapautaMuisti(lista);
    vapautaTuotanto(tLista);
    free(data);

    printf("\nKiitos ohjelman käytöstä.\n");
    return(0);
}    
