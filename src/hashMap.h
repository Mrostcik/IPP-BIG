/** @file
 * Interfejs klasy reprezentującej hashMapę.
 */
#ifndef HASHMAP_H_
#define HASHMAP_H_
#include <stdbool.h>
#include "twowaylist.h"

/** Struktura przechowująca wszystkie potrzebne dane dla danego miasta tj. w kolejności
 * wskaźnik na głowę listy sąsiadów oraz zmienne potrzebne do tworzenia dróg krajowych(przede wszystkim Dijkstry),
 * czyli wskaźnik na napis poprzedniego miasta w celu odtworzenia drogi, długość oraz rok tworzonej drogi krajowej,
 * prohibited - w celu wyłapania ewentualnych pętli, oraz explicitLength i explicitYear - w celu wyłapania
 * niejednoznaczności danej drogi krajowej.
 */
typedef struct cityNeighbours{
	///wskaźnik na głowę listy sąsiadów
	listElement* head;
	///napis reprezentujący poprzednie miasto na wyznaczonej drodze
	const char* previousCity;
	///długość drogi
	unsigned length;
	///rok drogi
	int routeYear;
	///czy miasto jest zakazane
	bool prohibited;
	///pomaga w ustaleniu jednoznaczności drogi
	unsigned explicitLength;
	///pomaga w ustaleniu jednoznaczności drogi
	int explicitYear;
} cityNeighbours;

/** Struktura reprezentująca element hashMapy.
 * Trzyma klucz, odpowiadająca mu wartość typu cityNeighbours* oraz wskaźnik na sąsiada.
 */
typedef struct hashMapElement hashMapElement;

/** Struktura reprezentująca hashMapę.
 * Trzyma rozmiar oraz tablicę elementów.
 */
typedef struct hashMap hashMap;

/** Tworzy nową hashMapę o zadanym rozmiarze.
 * @param[in] size - rozmiar hashMapy;
 * @return Wskaźnik na utworzoną hashMapę.
 */
hashMap* createMap(int size);

/** Dodaje do danej hashMapy w miejscu odpowiadającym danemu kluczowi
 * zadaną wartość.
 * @param[in] map - wskaźnik na hashMapę;
 * @param[in] key - wskaźnik na napis reprezentujący klucz(miasto)
 * @param[in] value - wskaźnik na strukturę odpowiadająca wartości, którą
 * chcemy wstawić do mapy dla danego klucza
 */
void insertMap(hashMap* map, const char* key, cityNeighbours* value);

/** Zwraca wartość odpowiadającą danemu kluczowi.
 * @param[in] map - wskaźnik na hashMapę;
 * @param[in] key - wskaźnik na napis reprezentujący klucz;
 * @return Wskaźnik na wartość - strukturę cityNeighbours* odpowiadającą danemu kluczowi.
 */
cityNeighbours* getMap(hashMap* map, const char* key);

/** Usuwa z hashMapy element o zadanym kluczu.
 * @param[in] map - wskaźnik na hashMapę;
 * @param[in] key - wskaźnik na napis reprezentujący klucz;
 */
void removeMap(hashMap* map, const char* key);

/** Usuwa całą hashMapę.
 * @param[in] map - wskaźnik na hashMapę;
 */
void clearMap(hashMap* map);

/** Dla wszystkich elementów hashMapy resetuje jej wartości na początkowe,
 * w celu dalszego pomyślnego działania innych funkcji, głównie tych tworzących drogi krajowe.
 * @param[in] map - wskaźnik na hashMapę;
 */
void resetForDijkstra(hashMap* map);
#endif /* HASHMAP_H_ */
