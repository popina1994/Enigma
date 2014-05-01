#ifndef ENIGMA_UTILS_H
#define ENIGMA_UTILS_H

/**
 * @brief Funkcija prebacuje hex string u binarnu reprezentaciju i pise ga u niz bin_key
 * @details Funkciji se prosledi kljuc kao hex string, kao i duzina kljuca. Posle provere da li string odgovara formatu, ona u bin_key upisuje binarnu reprezentaciju kljuca.
 * 
 * @param hex_key String sa kljucem u hex formatu. Njegova duzina mora odgovarati key_length parametru, koji je u bitovima. Npr. a6be1f duzine24.
 * @param bin_key Niz karaktera gde ce se upisati kljuc posle konverzije. Njegova velicina mora biti ista kao key_length u bitovima, inace dolazi do buffer overflow-a. Funkcija ne proverava da li je ovo ispunjeno.
 * @param key_length Duzina kljuca u bitima.
 * @return Kod greske do koje je doslo:\n
 * 0: Nema greske\n
 * 1: key_length nije deljiv sa 8\n
 * 2: Duzina hex_key-a ne odgovara key_length-u\n
 * 3: U hex_key-u se nalaze karakteri koji nisu hex cifre\n
 */
int hex_2_bin(const char * const hex_key, char bin_key[], int key_length);

/**
 * @brief Prima karakter iz hex stringa i vraca 4 bita koja predstavlja.
 * @details Funkcija prima bilo koju hex cifru 0-9, a-f, A-F i vraca karakter sa kodom 0x0Y u hexu, ili 0xff, ako karakter nije validan.
 * 
 * @param hex Hex cifra za konverziju.
 * @return Karakter cija su prva 4 bita 0, a ostali odredjeni argumentom hex, ili 0xff, ako karakter nije hex cifra
 */
char hex_2_code(char hex);

#endif