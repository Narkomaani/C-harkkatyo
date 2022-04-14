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
/* Harjoitustyö, Kirjasto.h */ 
#ifndef Kirjasto_h
#define Kirjasto_h

// typedef struct data DATA;
typedef struct tuotanto Tuotanto;
typedef struct data Data;
typedef struct ldata LData;

void tiedostonNimi(char nimi[]);
LData *lueTiedosto(LData *lista, char nimi[]);
Data *analysoiData(LData *lista, Data *pToinenData);
Tuotanto *analysoiKK(LData *lista, Tuotanto *pAlku);
void kirjoitaTiedosto(Data *dLista, Tuotanto *tLista, char nimi[]);
void vapautaTuotanto(Tuotanto *tLista);
void vapautaMuisti(LData *lista);


#endif
/* eof */
