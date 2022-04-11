# Jeremias Wahlsten - 488363
# 26.3.2022
# Vakuutan että olen itse kirjoittanut kaiken koodin kyseisessä tiedostossa
HTPerustaso: HTPerustaso.o HTPerusKirjasto.o
	gcc HTPerustaso.o HTPerusKirjasto.o -o HT
HTPerustaso.o: HTPerustaso.c HTPerusKirjasto.h
	gcc HTPerustaso.c -c -std=c99 -Wall -pedantic
HTPerusKirjasto.o: HTPerusKirjasto.c HTPerusKirjasto.h
	gcc HTPerusKirjasto.c -c -std=c99 -Wall -pedantic
