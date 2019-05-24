#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dijkstra.h"
#include "hashMap.h"
#include "twowaylist.h"
#include "priorityQueue.h"
#include "strdup.h"

/** @brief Zwraca minimum z dwóch liczb.
 * @param[in] x - pierwsza liczba;
 * @param[in] y - druga liczba;
 * @return Mniejsza liczba.
 */
static int min(int x, int y){
	if(x<y)
		return x;

	return y;
}

/** @brief Zwraca maksimum z dwóch liczb.
 * @param[in] x - pierwsza liczba;
 * @param[in] y - druga liczba;
 * @return Większa liczba.
 */
static int max(int x, int y){
	if(x>y)
		return x;

	return y;
}

bool dijkstra(hashMap* map, const char* city1, const char* city2){
	cityNeighbours* town1 = getMap(map, city1);
	town1->length = 0;

	//Można podać dowolny rozmiar kolejki zamiast 50.
	//W miarę potrzeby rozszerzy się.
	queue* queue = createQueue(50);
	if(queue == NULL){
		deleteQueue(queue);
		return false;
	}

	push(queue, city1, 0);
	while(getCount(queue) != 0){
		const char* actualTown = pop(queue);
		cityNeighbours* townData = getMap(map,actualTown);
		if(townData->prohibited == false){
		townData->prohibited = true;
		listElement* element = townData->head;

		if(city2 != NULL && strcmp(actualTown, city2) == 0){
			free((void*)actualTown);
			break;
		}
		while(element){
			cityNeighbours* neighbour = getMap(map, element->city);
			//Znaleźliśmy krótszą drogę, modyfikujemy odległość i wrzucamy na kolejkę.
			if(neighbour->prohibited == 0 && neighbour->length > townData->length + element->length){
				neighbour->length = townData->length + element->length;
				free((void*)neighbour->previousCity);
				neighbour->previousCity = strDuplicate(actualTown);
				neighbour->routeYear = min(townData->routeYear, element->builtYear);
				push(queue, element->city, neighbour->length);
			}
			//Droga o takiej samej długości.
			else if(neighbour->prohibited == 0 && neighbour->length == townData->length + element->length){
				//Nowocześniejsza droga, modyfikujemy rok i zmienne pomocnicze, żeby ustalić potem jednoznaczność trasy.
				if(min(townData->routeYear, element->builtYear) > neighbour->routeYear){
					free((void*)neighbour->previousCity);
					neighbour->previousCity = strDuplicate(actualTown);
					if(neighbour->explicitYear != 0 && neighbour->explicitLength == neighbour->length)
						neighbour->explicitYear = max(neighbour->routeYear, neighbour->explicitYear);
					else
						neighbour->explicitYear = neighbour->routeYear;
					neighbour->routeYear = min(townData->routeYear, element->builtYear);
					neighbour->explicitLength = neighbour->length;
				}
				//Modyfikujemy zmienne pomocnicze, aby ustalić potem jednoznaczność trasy.
				else{
					if(neighbour->explicitLength == neighbour->length)
						neighbour->explicitYear = max(min(townData->routeYear, element->builtYear), neighbour->explicitYear);
					else
						neighbour->explicitYear = min(townData->routeYear, element->builtYear);
					neighbour->explicitLength = neighbour->length;
				}
			}
			element = element->next;
		}
	}
		free((char*)actualTown);
	}
	deleteQueue(queue);
	return true;
}

bool checkDijkstra(hashMap* map, const char* city1, const char* city2){
	cityNeighbours* townData = getMap(map, city2);
	const char* city = city2;
	if(townData->prohibited == 0)
		return false;

	int year = townData->routeYear;

	do{
		if(townData->explicitLength == townData->length){
			if(townData->explicitYear >= year)
				return false;
		}

		city = townData->previousCity;
		townData = getMap(map, townData->previousCity);
	}while(strcmp(city, city1) != 0);

	return true;
}
