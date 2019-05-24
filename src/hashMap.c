#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "hashMap.h"
#include "strdup.h"

/** Struktura reprezentująca element hashMapy.
 * Trzyma klucz, odpowiadająca mu wartość typu cityNeighbours* oraz wskaźnik na sąsiada.
 */
static void changeMapSize(hashMap* map, int size);

/** Struktura reprezentująca element hashMapy.
 * Trzyma klucz, odpowiadająca mu wartość typu cityNeighbours* oraz wskaźnik na sąsiada.
 */
typedef struct hashMapElement{
	///klucz
	const char* key;
	///wartość typu cityNeighbours*
	cityNeighbours* value;
	///wskażnik na sąsiada
	struct hashMapElement* next;
} hashMapElement;

/** Struktura reprezentująca hashMapę.
 * Trzyma rozmiar oraz tablicę elementów.
 */
typedef struct hashMap{
	///rozmiar
	int size;
	///tablica elementów
	hashMapElement** table;
	///liczba elementów
	int count;
} hashMap;

hashMap* createMap(int size){
	hashMap* newMap = (hashMap*)malloc(sizeof(hashMap));

	newMap->size = size;
	newMap->count = 0;
	newMap->table = (hashMapElement**)malloc(sizeof(hashMapElement*) * size);

	for(int i=0; i<size; i++){
		newMap->table[i] = NULL;
	}
	return newMap;
}

/** Zwraca liczbę obliczoną za pomocą funkcji hashującej,
 * odpowiadająca danemu kluczowi. Jest to funkcja hashująca
 * Daniela J. Bernsteina, stąd występujące tu stałe w celu
 * zmninimalizowania liczb kolizji.
 * @param[in] map - wskaźnik na hashMapę;
 * @param[in] key - wskaźnik na napis reprezentujący klucz;
 * @return Wartość liczbowa wyliczona dla danego klucza.
 */
static unsigned int calculateHash(hashMap* map, const char* key){
	int size = map->size;
	unsigned int hash = 5381;

	while(*key){
		hash = ((hash << 5) + hash) + (*key);
		hash = hash%size;
		key++;
	}

	return hash;
}

void insertMap(hashMap* map, const char* key, cityNeighbours* value){
	if(map->count+1 >= map->size)
		changeMapSize(map, map->size*2);

	if(map == NULL)
		return;

	unsigned hash = calculateHash(map, key);
	hashMapElement* element = map->table[hash];

	while(element){
		if(strcmp(element->key, key) == 0){
			element->value = value;
			return;
		}
		else
			element = element->next;
	}

	hashMapElement* newElement = (hashMapElement*)malloc(sizeof(hashMapElement));
	newElement->key = strDuplicate(key);
	newElement->value = value;
	newElement->next = map->table[hash];
	map->table[hash] = newElement;
	map->count++;

}

cityNeighbours* getMap(hashMap* map, const char* key){
	unsigned hash = calculateHash(map, key);
	hashMapElement* element = map->table[hash];

	while(element){
		if(strcmp(element->key, key) == 0){
			return element->value;
		}

		element = element->next;
	}

	return NULL;

}

void removeMap(hashMap* map, const char* key){
	unsigned hash = calculateHash(map, key);
	hashMapElement* element = map->table[hash];
	hashMapElement* previousElement = NULL;

	while(element){
		if(strcmp(element->key, key) == 0){
			if(previousElement == NULL)
				map->table[hash] = element->next;
			else
				previousElement->next = element->next;

			map->count--;
			free((char*)element->value->previousCity);
			free((char*)element->key);

			free(element->value);
			free(element);
			break;
		}
		previousElement = element;
		element = element->next;
	}
}

/** Zmienia rozmiar hashMapy na podany.
 * @param[in] map - wskaźnik na hashMapę
 * @param[in] size - nowy rozmiar mapy
 */
static void changeMapSize(hashMap* map, int size){
    struct hashMap* newHM = NULL;
    struct hashMapElement* currLink = NULL;

    newHM = createMap(size);


    for(int i=0; i<map->size; i++){
        currLink = map->table[i];
        while(currLink != NULL){
            insertMap(newHM, currLink->key, currLink->value);
            currLink = currLink->next;
        }
    }

	for(int i=0; i<map->size; i++){
		if(map->table[i] != NULL){
			while(map->table[i] != NULL){
				hashMapElement* next = map->table[i]->next;
				free((char*)map->table[i]->key);
				free(map->table[i]);
				map->table[i] = next;
			}
		}

	}

	free(map->table);

    map->table = newHM->table;
    map->size = newHM->size;
    map->count = newHM->count;
    free(newHM);
}

void clearMap(hashMap* map){
	if(map == NULL)
		return;

	for(int i=0; i<map->size; i++){
		if(map->table[i] != NULL){
			while(map->table[i] != NULL){
				hashMapElement* next = map->table[i]->next;
				if(map->table[i]->value->head != NULL)
					deleteList(map->table[i]->value->head);
				free((char*)map->table[i]->value->previousCity);
				free((char*) map->table[i]->key);
				free(map->table[i]->value);
				free(map->table[i]);
				map->table[i] = next;
			}
		}

	}

	free(map->table);
	free(map);
}

void resetForDijkstra(hashMap* map){
	for(int i=0; i<map->size; i++){
		if(map->table[i] != NULL){
			hashMapElement* next = map->table[i];
			while(next != NULL){
				cityNeighbours* city = next->value;
				city->length = INT_MAX;
				free((void*)city->previousCity);
				city->previousCity = NULL;
				city->explicitLength = 0;
				city->explicitYear = 0;
				city->prohibited = 0;
				city->routeYear = INT_MAX;
				next = next->next;
			}
		}
	}
}
