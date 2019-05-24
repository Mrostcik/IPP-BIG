#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "strdup.h"
#include "map.h"
#include "twowaylist.h"

/** @brief Wczytuje jedno słowo z standardowego wejścia
 * Jedno słowo, znaczy napis aż do napotkania średnika lub końca linii.
 * @param[in] last - wartość logiczna: true, jeśli wczytane słowo jest ostatnim w linii,
 * false jeśli nie;
 * @return Wskaźnik na wczytane słowo lub NULL jeśli alokacja pamięci była nieudana.
 */
static char* getWord(bool *last){
	char* str = NULL;
	int character;
	size_t size = 0, length = 0;

	while((character = getchar()) != EOF && character != ';'){
		if(character == '\n'){
			*last = 1;
			break;
		}

		if(character == '#' && str == NULL){
			while((character = getchar()) != '\n');
			if(character == '\n')
				*last = 1;
			break;
		}

        if(length+1 >= size){
            size = size*2+1;
            str = realloc(str, sizeof(char)*size);

            if(str == NULL)
            	return NULL;

        }

        str[length++] = character;
	}

    if(length+1 >= size){
        size++;
        str = realloc(str, sizeof(char)*size);

        if(str == NULL)
        	return NULL;
    }
	str[length] = '\0';

	//Zwracamy ';' w celu odróżnienia słowa pustego, gdy na wejściu jest ...;;...
	//od linii kończącej plik wejściowy.
	if(strlen(str) == 0 && character == ';'){
		while(getchar() != '\n');
		free(str);
		return strDuplicate(";");
	}

	return str;
}

/** @brief Sprawdza czy dany napis symbolizuje liczbę i czy liczba ta jest mniejsza od zadanego maksimum.
 * @oaram[in] str - wskaźnik na napis reprezentujący prawdopodobną liczbę;
 * @param[in] range - wskaźnik na napis reprezentujący liczbę-maksimum;
 * @return Wartość @p true, jeśli dany napis jest liczbą i mieści się w danym zakresie.
 * @Wartość @p false w przeciwnym przypadku.
 */
bool checkRange(char* str, char* range){
	for(unsigned i = 0; i < strlen(str); i++){
		if(isdigit(str[i]) == 0)
			return false;
	}

	if(strlen(str) < strlen(range))
		return true;
	else if(strlen(str) == strlen(range)){
		unsigned i = 0;

		while(i<strlen(str)){
			if(str[i] > range[i])
				return false;
			else if(str[i] < range[i])
				return true;
			else
				i++;
		}

		return true;
	}

	return false;

}

/** @brief Wczytuje, sprawdza poprawność i wykonuje linię odpowiadającą komendzie repairRoad.
 * @param[in] map - wskaźnik na mapę dróg;
 * @return Wartość @p true, jeśli cała komenda była poprawna i została poprawnie wykonana.
 * Wartość @p false, jeśli wystąpił błąd - któryś z parametrów był niepoprawny, alokacja pamięci
 * była nieudana, reperowanie drogi się nie udało.
 */
static bool inputRepairRoad(Map* map){
	bool last = 0;
	char* city1 = getWord(&last);
	if(last == 1 || *city1 == ';'){
		free(city1);
		return false;
	}

	char* city2 = getWord(&last);
	if(last == 1 || *city2 == ';'){
		free(city1);
		free(city2);
		return false;
	}

	char* year = getWord(&last);
	if(*year == ';'){
		free(city1);
		free(city2);
		free(year);
		return false;
	}
	if(last == 0){
		while(getchar() != '\n');
		free(city1);
		free(city2);
		free(year);
		return false;
	}

	if(city1 == NULL || city2 == NULL || year == NULL){
		while(getchar() != '\n');
		free(city1);
		free(city2);
		free(year);
		deleteMap(map);
		exit(0);
	}

	bool yearSign = 0;
	char* yearC = year;
	if(strlen(year) > 0){
		if(year[0] == '-'){
			yearSign = 1;
			year++;
		}
	}

	if(strlen(city1) == 0 || strlen(city2) == 0 || strlen(year) == 0 ||checkCorectness(city1) == 0
			|| checkCorectness(city2) == 0 || (yearSign == 0 && checkRange(year, "2147483647") == 0) ||
			 (yearSign == 1 && checkRange(year, "2147483648") == 0) || strcmp(year, "0") == 0){
		free(city1);
		free(city2);
		free(yearC);
		return false;
	}

	int yearInt = strtoul(year, NULL, 10);
	if(yearSign == 1)
		yearInt *= -1;

	bool result = repairRoad(map, city1, city2, yearInt);
	free(city1);
	free(city2);
	free(yearC);

	return result;
}

/** @brief Wczytuje, sprawdza poprawność i wykonuje linię odpowiadającą komendzie addRoad.
 * @param[in] map - wskaźnik na mapę dróg;
 * @return  Wartość @p true, jeśli cała komenda była poprawna i została poprawnie wykonana.
 * Wartość @p false, jeśli wystąpił błąd - któryś z parametrów był niepoprawny, alokacja pamięci
 * była nieudana, dodanie drogi się nie udało.
 */
static bool inputAddRoad(Map* map){
	bool last = 0;
	char* city1 = getWord(&last);
	if(last == 1 || *city1 == ';'){
		free(city1);
		return false;
	}

	char* city2 = getWord(&last);
	if(last == 1 || *city2 == ';'){
		free(city1);
		free(city2);
		return false;
	}

	char* length = getWord(&last);
	if(last == 1 || *length == ';'){
		free(city1);
		free(city2);
		free(length);
		return false;
	}

	char* year = getWord(&last);
	if(*year == ';'){
		free(city1);
		free(city2);
		free(length);
		free(year);
		return false;
	}
	if(last == 0){
		while(getchar() != '\n');
		free(city1);
		free(city2);
		free(length);
		free(year);
		return false;
	}

	if(city1 == NULL || city2 == NULL || length == NULL || year == NULL){
		while(getchar() != '\n');
		free(city1);
		free(city2);
		free(length);
		free(year);
		deleteMap(map);
		exit(0);
	}

	bool yearSign = 0;
	char* yearC = year;
	if(strlen(year) > 0){
		if(year[0] == '-'){
			yearSign = 1;
			year++;
		}
	}

	if(strlen(city1) == 0 || strlen(city2) == 0 || strlen(length) == 0|| strlen(year) == 0 ||
			checkCorectness(city1) == 0 || checkCorectness(city2) == 0 ||
				checkRange(length, "4294967295") == 0 || (yearSign == 0 && checkRange(year, "2147483647") == 0) ||
				 (yearSign == 1 && checkRange(year, "2147483648") == 0) || strcmp(year, "0") == 0 ||
				   strcmp(length, "0") == 0){
		free(city1);
		free(city2);
		free(length);
		free(yearC);
		return false;
	}

	unsigned lengthInt = strtoul(length, NULL, 10);
	int yearInt = strtoul(year, NULL, 10);
	if(yearSign == 1)
		yearInt *= -1;

	bool result = addRoad(map, city1, city2, lengthInt, yearInt);
	free(city1);
	free(city2);
	free(length);
	free(yearC);

	return result;
}

/** @brief Wczytuje, sprawdza poprawność i wykonuje linię odpowiadającą komendzie getRouteDescription.
 * @param[in] map - wskaźnik na mapę dróg;
 * @return  Wartość @p true, jeśli cała komenda była poprawna i została poprawnie wykonana.
 * Wartość @p false, jeśli wystąpił błąd - któryś z parametrów był niepoprawny lub alokacja pamięci
 * była nieudana.
 */
static bool inputGetRouteDescription(Map* map){
	bool last = 0;
	char* routeId = getWord(&last);
	if(*routeId == ';'){
		free(routeId);
		return false;
	}
	if(routeId == NULL){
		deleteMap(map);
		exit(0);
	}

	if(last == 0){
		while(getchar() != '\n');
		free(routeId);
		return false;
	}

	if(strlen(routeId) == 0){
		free(routeId);
		return false;
	}

	if(checkRange(routeId, "4294967295") == 0){
		free(routeId);
		return false;
	}

	int routeIdInt = strtoul(routeId, NULL, 10);
	const char* description = getRouteDescription(map, routeIdInt);

	if(description == NULL){
		free(routeId);
		return false;
	}

	printf("%s\n", description);
	free((char*) description);
	free(routeId);
	return true;
}

/** @brief Wczytuje, sprawdza poprawność i wykonuje linię odpowiadającą dodanie nowej drogi krajowej.
 * @param[in] map - wskaźnik na mapę dróg;
 * @return  Wartość @p true, jeśli cała komenda była poprawna i została poprawnie wykonana.
 * Wartość @p false, jeśli wystąpił błąd - któryś z parametrów był niepoprawny, alokacja pamięci
 * była nieudana lub dodanie nowej drogi krajowej się nie powiodło.
 */
static bool inputNewRoute(Map* map, unsigned routeId){
	bool last = 0;
	char* city1 = getWord(&last);
	char* length = NULL;
	char* year = NULL;
	char* city2 = NULL;
	listElement* tail = NULL;

	if(city1 == NULL){
		deleteMap(map);
		exit(0);
	}
	if(last == 1 || *city1 == ';'){
		free(city1);
		return false;
	}

	char* yearC = NULL;
	while(last != 1){
		length = getWord(&last);
		if(last == 1 || *length == ';'){
			if(tail != NULL)
				clearRoute(map, routeId);
			free(city1);
			free(length);
			return false;
		}

		year = getWord(&last);
		if(last == 1 || *year == ';'){
			if(tail != NULL)
				clearRoute(map, routeId);
			free(city1);
			free(length);
			free(year);
			return false;
		}

		city2 = getWord(&last);

		if(*city2 == ';'){
			if(tail != NULL)
				clearRoute(map, routeId);
			free(city1);
			free(city2);
			free(length);
			free(year);
			return false;
		}

		if(length == NULL || year == NULL || city2 == NULL){
			while(getchar() != '\n');
			if(tail != NULL)
				clearRoute(map, routeId);
			free(city1);
			free(length);
			free(year);
			free(city2);
			deleteMap(map);
			exit(0);
		}

		bool yearSign = 0;
		yearC = year;
		if(strlen(year) > 0){
			if(year[0] == '-'){
				yearSign = 1;
				year++;
			}
		}
		if(strlen(city1) == 0 || strlen(city2) == 0 || strlen(length) == 0|| strlen(year) == 0 ||
				checkCorectness(city1) == 0 || checkCorectness(city2) == 0 ||
					checkRange(length, "4294967295") == 0 || (yearSign == 0 && checkRange(year, "2147483647") == 0) ||
					 (yearSign == 1 && checkRange(year, "2147483648") == 0) || strcmp(year, "0") == 0 ||
					   strcmp(length, "0") == 0){
			if(last != 1)
				while(getchar() != '\n');
			if(tail != NULL)
				clearRoute(map, routeId);
			free(city1);
			free(city2);
			free(length);
			free(yearC);
			return false;
		}

		unsigned lengthInt = strtoul(length, NULL, 10);
		int yearInt = strtoul(year, NULL, 10);
		if(yearSign == 1)
			yearInt *= -1;
		tail = newSpecificRoute(map, city1, city2, lengthInt, yearInt, routeId, tail);
		//Jeżeli otrzymany ogon listy drogi krajowej jest NULLem,
		//to znaczy, że dodanie drogi krajowej nie powiodło się.
		if(tail == NULL){
			if(last != 1)
				while(getchar() != '\n');
			free(city1);
			free(city2);
			free(length);
			free(yearC);
			return false;
		}
		free(city1);
		free(yearC);
		free(length);
		city1 = city2;
	}

	//Uzupełnianie drogi krajowej powiodło się.
	//Dodajemy lub remontujemy potrzebne do danej drogi krajowej odcinki.
	supplementRoads(map, city1);
	free(city1);
	return true;

}

bool executeLine(Map* map, bool* end){
	bool last = 0;
	char* str = getWord(&last);
	bool result = 0;
	if(strlen(str) == 0 && last == 0){
		*end = 1;
		free(str);
		return false;
	}
	if(str == NULL){
		deleteMap(map);
		exit(0);
	}

	if(str == NULL || *str == ';'){
		free(str);
		return false;
	}
	if(strlen(str) == 0){
		free(str);
		return true;
	}

	if(last == 1){
		free(str);
		return false;
	}

	//Wywołujemy odpowiadająca 1. słowu komendę
	if(strcmp("addRoad", str) == 0){
		free(str);
		return inputAddRoad(map);
	}
	else if(strcmp("repairRoad", str) == 0){
		free(str);
		return inputRepairRoad(map);
	}
	else if(strcmp("getRouteDescription", str) == 0){
		free(str);
		return inputGetRouteDescription(map);
	}
	else if(strlen(str) <= 3 && strlen(str) > 0){
		if(str[0] == '0'){
			free(str);
			while(getchar() != '\n');
			return false;
		}

		for(unsigned i = 0; i < strlen(str); i++)
			if(isdigit(str[i]) == 0){
				free(str);
				while(getchar() != '\n');
				return false;
			}

		result = inputNewRoute(map, strtoul(str, NULL, 10));
		free(str);
		return result;
	}

	while(getchar() != '\n');
	free(str);
	return false;
}
