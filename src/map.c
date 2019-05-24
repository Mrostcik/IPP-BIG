#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "twowaylist.h"
#include "hashMap.h"
#include "priorityQueue.h"
#include "map.h"
#include "dijkstra.h"
#include "strdup.h"

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map{
	///liczba miast
	int numberOfCities;
	///wskaźnik na hashMapę trzymającą potrzebne dane dla miast
	hashMap* graph;
	///tablica wskaźników na głowy dróg krajowych
	listElement* routes[999];
	///tablica wskaźników na głowy dróg, w celu uzupełnienia dróg krajowych
	///używana w extendRoute
	listElement* routesExtension[999];
} Map;

Map* newMap(){
	Map* map = (Map*)malloc(sizeof(Map));

	map->numberOfCities = 0;
	//Tworzymy hashMapę, która będzie przechowywać dane o miastach.
	//Można podać inny rozmiar niż 8, w miarę potrzeby hashMapa rozszerzy się.
	map->graph = createMap(8);

	for(int i = 0; i<999; i++){
		map->routes[i] = NULL;
		map->routesExtension[i] = NULL;
	}

	return map;
}

void deleteMap(Map* map){
	clearMap(map->graph);
	for(int i = 0; i<999; i++){
		if(map->routes[i] != NULL)
			deleteList(map->routes[i]);

		if(map->routesExtension[i] != NULL)
			deleteList(map->routesExtension[i]);
	}
	free(map);
}

bool checkCorectness(const char* city){
	if(strlen(city) == 0)
		return false;
	while(*city){
		if((*city >=0 && *city <=31) || *city == ';')
			return false;

		city++;
	}

	if(*city != '\0')
		return false;

	return true;
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear){
	if(map == NULL || city1 == NULL || city2 == NULL || builtYear == 0 || length <1)
		return false;
	if(strcmp(city1, city2) == 0 || checkCorectness(city1) == 0 || checkCorectness(city2) == 0)
		return false;

	if(getMap(map->graph, city1) == NULL){
		cityNeighbours* newCity = (cityNeighbours*)malloc(sizeof(cityNeighbours));
		if(newCity == NULL)
			return false;

		newCity->head = NULL;
		newCity->length = INT_MAX;
		newCity->previousCity = NULL;
		newCity->explicitLength = 0;
		newCity->explicitYear = 0;
		newCity->prohibited = 0;
		newCity->routeYear = INT_MAX;

		insertMap(map->graph, city1, newCity);
		map->numberOfCities++;
	}

	if(getMap(map->graph, city2) == NULL){
		cityNeighbours* newCity = (cityNeighbours*)malloc(sizeof(cityNeighbours));
		if(newCity == NULL)
			return false;

		newCity->head = NULL;
		newCity->length = INT_MAX;
		newCity->previousCity = NULL;
		newCity->explicitLength = 0;
		newCity->explicitYear = 0;
		newCity->prohibited = 0;
		newCity->routeYear = INT_MAX;

		insertMap(map->graph, city2, newCity);
		map->numberOfCities++;
	}
	if(map->graph == NULL)
		return false;

	listElement* head = getMap(map->graph, city1)->head;
	listElement* head2 = getMap(map->graph, city2)->head;

	if(contains(head, city2))
		return false;

	listElement* newNeighbour = getListElement(city2, length, builtYear);
	listElement* newNeighbour2 = getListElement(city1, length, builtYear);
	if(newNeighbour == NULL || newNeighbour2 == NULL)
		return false;

	if(head != NULL){
	    newNeighbour->next = head;
	    head->prev = newNeighbour;
	}
	getMap(map->graph, city1)->head = newNeighbour;

	if(head2 != NULL){
	    newNeighbour2->next = head2;
	    head2->prev = newNeighbour2;
	}
	getMap(map->graph, city2)->head = newNeighbour2;

	return true;

}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear){
	if(map == NULL || city1 == NULL || city2 == NULL || repairYear == 0)
		return false;
	if(checkCorectness(city1) == 0 || checkCorectness(city2) == 0 || strcmp(city1, city2) == 0)
		return false;

	cityNeighbours* town1 = getMap(map->graph, city1);
	cityNeighbours* town2 = getMap(map->graph, city2);
	if(town1 == NULL || town2 == NULL)
		return false;

	listElement* head = town1->head;
	listElement* head2 = town2->head;

	if(modifyYear(head, city2, repairYear) == 0 || modifyYear(head2, city1, repairYear) == 0)
		return false;

	return true;

}

bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2){

	if(map == NULL || city1 == NULL || city2 == NULL || routeId<1 || routeId>999)
		return false;
	if(checkCorectness(city1) == 0 || checkCorectness(city2) == 0 || strcmp(city1, city2) == 0)
		return false;
	if(map->routes[routeId-1] != NULL)
		return false;

	cityNeighbours* town1 = getMap(map->graph, city1);
	cityNeighbours* town2 = getMap(map->graph, city2);
	if(town1 == NULL || town2 == NULL)
		return false;

	if(dijkstra(map->graph, city1, city2) == 0){
		resetForDijkstra(map->graph);
		return false;
	}

	if(checkDijkstra(map->graph, city1, city2 ) == 0){
		resetForDijkstra(map->graph);
		return false;
	}

	//Rekonstruujemy drogę krajową.
	cityNeighbours* townBacking = town2;
	const char* city = city2;
	listElement* element = getListElement(city2, town2->length, town2->routeYear);

	while(strcmp(city, city1) != 0){
		city = townBacking->previousCity;
		townBacking = getMap(map->graph, townBacking->previousCity);
		listElement* element2 = getListElement(city, town2->length, town2->routeYear);
		element2->next = element;
		element->prev = element2;
		element = element2;
	}

	map->routes[routeId-1] = element;

	resetForDijkstra(map->graph);

	return true;
}

bool extendRoute(Map *map, unsigned routeId, const char *city){
	if(map == NULL || routeId<1 || routeId>999 || getMap(map->graph, city) == NULL || checkCorectness(city) == 0)
		return false;
	if(map->routes[routeId-1] == NULL)
		return false;

	//Ustawiamy element na ostatnie miasto z drogi krajowej.
	listElement* element = map->routes[routeId-1];
	while(element){
		if(element->prev != NULL && element->next != NULL)
			getMap(map->graph, element->city)->prohibited = true;
		if(strcmp(element->city, city) == 0){
			resetForDijkstra(map->graph);
			return false;
		}

		if(element->next == NULL)
			break;

		element = element->next;
	}

	dijkstra(map->graph, city, NULL );
	listElement* element2 = map->routes[routeId-1];
	//Bierzemy dane pierwszego i ostatniego miasta z drogi krajowej, bo
	//rozszerzenie drogi krajowej jest możliwe z obu stron.
	cityNeighbours* startTown = getMap(map->graph, element2->city);
	cityNeighbours* endTown = getMap(map->graph, element->city);
	//0 = miasto początkowe, 1 = miasto końcowe
	bool whichTown = 0;

	//Sprawdzamy kilka warunków, które wykluczają nam pętle i niejednoznaczność trasy.
	if(checkDijkstra(map->graph, city, element2->city) == 0 && checkDijkstra(map->graph, city, element->city) == 0){
		resetForDijkstra(map->graph);
		return false;
	}
	else if(checkDijkstra(map->graph, city, element2->city) == 0 && startTown->prohibited == 1 &&
			startTown->length < endTown->length){
		resetForDijkstra(map->graph);
		return false;
	}
	else if(checkDijkstra(map->graph, city, element2->city) == 0 && startTown->prohibited == 1 &&
			startTown->length == endTown->length && startTown->routeYear >= endTown->routeYear){
		resetForDijkstra(map->graph);
		return false;
	}
	else if(checkDijkstra(map->graph, city, element->city) == 0 && endTown->prohibited == 1 &&
			startTown->length > endTown->length){
		resetForDijkstra(map->graph);
		return false;

	}
	else if(checkDijkstra(map->graph, city, element->city) == 0 && endTown->prohibited == 1 &&
			startTown->length == endTown->length && startTown->routeYear <= endTown->routeYear){
		resetForDijkstra(map->graph);
		return false;
	}
	else if(checkDijkstra(map->graph, city, element2->city) == 0)
		whichTown = 1;
	else if(checkDijkstra(map->graph, city, element->city) == 0){
		whichTown = 0;
	}
	else{
		if(startTown->length > endTown->length)
			whichTown = 1;
		else if(startTown->length == endTown->length && endTown->routeYear == startTown->routeYear){
			resetForDijkstra(map->graph);
			return false;
		}
		else if(startTown->length == endTown->length && endTown->routeYear > startTown->routeYear)
			whichTown = 1;
		else
			whichTown = 0;
	}

	cityNeighbours* actualTown = endTown;
	//Rozszerzamy drogę krajową od początku(whichTown = 0) lub od końca(whichTown = 1).
	if(whichTown == 1){
		const char* cityBacking = NULL;

		while(cityBacking == NULL || strcmp(cityBacking, city) != 0){
		    cityBacking = actualTown->previousCity;
		    listElement* newElement = getListElement(cityBacking, endTown->length, endTown->routeYear);
		    newElement->prev = element;
		    element->next = newElement;
		    element = newElement;
		    actualTown = getMap(map->graph, actualTown->previousCity);
		}

	}

	actualTown = startTown;
	if(whichTown == 0){
		const char* cityBacking = NULL;

		listElement* newElement = NULL;
		while(cityBacking == NULL || strcmp(cityBacking, city) != 0){
			cityBacking = actualTown->previousCity;
			newElement = getListElement(cityBacking, startTown->length, startTown->routeYear);
			newElement->next = element2;
			element2->prev = newElement;
			element2 = newElement;
			actualTown = getMap(map->graph, actualTown->previousCity);
		}

		map->routes[routeId-1] = newElement;
	}

	resetForDijkstra(map->graph);
	return true;

}


bool removeRoad(Map *map, const char *city1, const char *city2){
	if(map == NULL || checkCorectness(city1) == 0 || checkCorectness(city2) == 0 )
		return false;
	if(getMap(map->graph, city1) == NULL || getMap(map->graph, city2) == NULL)
		return false;

	cityNeighbours* town1 = getMap(map->graph, city1);
	cityNeighbours* town2 = getMap(map->graph, city2);
	listElement* element = town1->head;
	bool existsRoad = 0;
	unsigned lengthCopy;
	int builtYearCopy;
	//Usuwamy odcinek drogi pomiędzy city1, a city2.
	while(element){
		if(strcmp(element->city, city2) == 0){
			existsRoad = 1;
			lengthCopy = element->length;
			builtYearCopy = element->builtYear;
			if(element->prev == NULL){
				if(element->next == NULL)
					town1->head = NULL;
				else
					town1->head = element->next;
			}

			deleteListElement(element);
			break;
		}

		element = element->next;
	}

	//Nie można usunąć drogi, bo nie istnieje.
	if(existsRoad == 0)
		return false;

	element = town2->head;

	//Usuwamy odcinek drogi pomiędzy city2, a city1.
	while(element){
		if(strcmp(element->city, city1) == 0){
			if(element->prev == NULL){
				if(element->next == NULL)
					town2->head = NULL;
				else
					town2->head = element->next;
			}

			deleteListElement(element);
			break;
		}

		element = element->next;
	}
	//1 jeśli usunięcie drogi jest możliwe, 0 w przeciwnym wypadku
	bool possibleRemoval = 1;
	//1 jeśli dana droga wymaga uzupełnienia po usunięciu drogi
	bool toExtend[999];
	for(int i = 0; i<999; i++){
		listElement* startRoute = map->routes[i];
		bool containsRoad = 0;
		toExtend[i] = 0;
		bool direction = 0;
		//Sprawdzamy, czy odcinek drogi należy do i-tej drogi krajowej.
		if(startRoute != NULL){
		    while(startRoute->next){
			    if((strcmp(startRoute->city, city1) == 0 && strcmp(startRoute->next->city, city2) == 0)
			    		|| (strcmp(startRoute->city, city2 ) == 0 && strcmp(startRoute->next->city, city1) == 0)){
			    	if(strcmp(startRoute->city, city1) == 0 && strcmp(startRoute->next->city, city2) == 0)
			    		direction = 0;
			    	else
			    		direction = 1;
				    containsRoad = 1;
				    toExtend[i] = 1;
				    break;
			    }
			    startRoute = startRoute->next;
		    }
		}
		//Jeżeli należy.
		if(containsRoad == 1){
		    startRoute = map->routes[i];

		    //Ustalamy miasta z i-tej drogi krajowej jako zakazane, aby zapobiec pętlom.
		    while(startRoute){
			    if(strcmp(startRoute->city, city1) != 0 && strcmp(startRoute->city, city2) != 0){
				    getMap(map->graph, startRoute->city)->prohibited = 1;
			    }
			    startRoute = startRoute->next;
		    }
		    cityNeighbours* city = NULL;
		    listElement* extensionRoute = NULL;
		    const char* actualCity = NULL;
		    //Tworzymy uzupełnienie drogi krajowej, zachowując kolejność.
		    //direction = 0 - city1 przed city2, direction = 1 city2 przed city1.
		    if(direction == 0){
		    	dijkstra(map->graph, city1, city2);
		    	city = getMap(map->graph, city2);

		    	if(checkDijkstra(map->graph, city1, city2) == 0){
		    		possibleRemoval = 0;
		    		resetForDijkstra(map->graph);
		    		break;
		    	}
		    	extensionRoute = getListElement(city2, city->length, city->routeYear);
		    	actualCity = city2;

		    	while(strcmp(city1, actualCity) != 0){
		    		actualCity = city->previousCity;
		    		city = getMap(map->graph, actualCity);
		    		listElement* element = getListElement(actualCity, extensionRoute->length, extensionRoute->builtYear);
		    		element->next = extensionRoute;
		    		extensionRoute->prev = element;
		    		extensionRoute = element;
		    	}
		    }
		    else{
			    dijkstra(map->graph, city2, city1);
			    city = getMap(map->graph, city1);

			    if(checkDijkstra(map->graph, city2, city1) == 0){
			    	possibleRemoval = 0;
			    	resetForDijkstra(map->graph);
			    	break;
			    }

			    extensionRoute = getListElement(city1, city->length, city->routeYear);
			    actualCity = city1;
			    while(strcmp(city2, actualCity) != 0){
			    	actualCity = city->previousCity;
			    	city = getMap(map->graph, actualCity);
			    	listElement* element = getListElement(actualCity, extensionRoute->length, extensionRoute->builtYear);
			    	element->next = extensionRoute;
			    	extensionRoute->prev = element;
			    	extensionRoute = element;

			    }
		    }

		    map->routesExtension[i] = extensionRoute;
		    resetForDijkstra(map->graph);
		}
	}
	//Nie da się usunąć odcinka, dodajemy go z powrotem i czyścimy zbudowane rozszerzenia dróg krajowych.
	if(possibleRemoval == 0){
		addRoad(map, city1, city2, lengthCopy, builtYearCopy);
		for(int i = 0; i<999; i++){
			if(map->routesExtension[i] != NULL){
				deleteList(map->routesExtension[i]);
				map->routesExtension[i] = NULL;

			}
		}

		return false;
	}
	//Można usunąć odcinek drogi, dla każdej drogi krajowej uzupełniamy ją jeżeli trzeba.
	//Polega to na złączeniu list map->routes[i] i map->routesExtension[i].
	for(int i = 0; i<999; i++){
		if(toExtend[i] == 1){
			listElement* element = map->routes[i];
			while(strcmp(element->city, city1) != 0 && strcmp(element->city, city2) !=0){
				element = element->next;
			}

			listElement* elementCopy = element;
			listElement* elementCopyNext = NULL;
			listElement* elementCopyNext2 = element->next;
			if(element->next->next != NULL)
				elementCopyNext = element->next->next;
			listElement* element2 = map->routesExtension[i]->next;
			listElement* newElement = getListElement(map->routesExtension[i]->next->city,0 ,0);

			while(element2){
				elementCopy->next = newElement;
				newElement->prev = elementCopy;
				elementCopy = newElement;
				element2 = element2->next;
				if(element2 != NULL)
					newElement = getListElement(element2->city, 0 , 0);
			}
			elementCopy->next = elementCopyNext;
			if(elementCopyNext != NULL){
				elementCopyNext->prev = elementCopy;
			}
			free((char*) elementCopyNext2->city);
			free(elementCopyNext2);
			deleteList(map->routesExtension[i]);
			map->routesExtension[i] = NULL;
		}
	}

	return true;
}


char const* getRouteDescription(Map *map, unsigned routeId){
	if(map == NULL || routeId<1 || routeId>999)
		return strDuplicate("");

	if(map->routes[routeId-1] == NULL)
		return strDuplicate("");


	char* description = (char*)malloc(sizeof(char) * 16);
	size_t size = 16, length = 0;

	listElement* element = map->routes[routeId-1];
	sprintf(description, "%d", routeId);
	strcat(description, ";");
	length = strlen(description)+1;

	while(element){
		if(length + strlen(element->city) + 1>= size){
			size = size*2 + strlen(element->city) + 1;
			description = realloc(description, sizeof(char) * size);
		}

		strcat(description, element->city);
		length += strlen(element->city)+1;
		cityNeighbours* city = getMap(map->graph, element->city);
		if(element->next == NULL)
			break;

		strcat(description, ";");

		listElement* neighbour = city->head;

		char* lengthNeigh = (char*)malloc(sizeof(char)*14);
		char* year = (char*)malloc(sizeof(char)*14);

		//Szukamy długości i roku odcinka drogi pomiędzy dwoma sąsiadami z drogi krajowej.
		while(neighbour){
			if(strcmp(neighbour->city, element->next->city) == 0){
				sprintf(lengthNeigh, "%u", neighbour->length);
				sprintf(year, "%d", neighbour->builtYear);
				break;
			}

			neighbour = neighbour->next;
		}

		if(length + strlen(lengthNeigh) + strlen(year) + 2 >= size){
			size = size*2 + strlen(lengthNeigh) + strlen(year) + 2;
			description = realloc(description, sizeof(char) * size);
		}

		strcat(description, lengthNeigh);
		strcat(description, ";");
		strcat(description, year);
		strcat(description, ";");
		length += strlen(lengthNeigh) + strlen(year) + 2;
		free(year);
		free(lengthNeigh);

		element = element->next;
	}

	return (const char*) description;
}

/** Dodaje miasto do mapy.
 * @param[in] map - wskaźnik na mapę dróg;
 * @param[in] city - wskaźnik na napis reprezentujący nazwę miasta;
 * @return Wartość @p true, jeśli miasto zostało dodane do mapy
 * Wartość @p false, jeśli alokacja pamięci była nieudana.
 */
static bool addCity(Map* map, const char* city){
	if(getMap(map->graph, city) == NULL){
		cityNeighbours* newCity = (cityNeighbours*)malloc(sizeof(cityNeighbours));
		if(newCity == NULL)
			return false;

		newCity->head = NULL;
		newCity->length = INT_MAX;
		newCity->previousCity = NULL;
		newCity->explicitLength = 0;
		newCity->explicitYear = 0;
		newCity->prohibited = 0;
		newCity->routeYear = INT_MAX;

		insertMap(map->graph, city, newCity);
		if(map->graph == NULL)
			return false;
		map->numberOfCities++;
	}

	return true;
}

listElement* newSpecificRoute(Map* map, const char* city1, const char* city2, unsigned length, int builtYear,
						unsigned routeId, listElement* tail){


	if(map->routes[routeId-1] != NULL && tail == NULL){
		resetForDijkstra(map->graph);
		return NULL;
	}

	//Dodajemy miasta, aby ułatwić sprawdzanie wszystkich warunków.
	//Jeżeli tworzenie drogi krajowej się nie powiedzie zostaną one usunięte.
	if(addCity(map, city1) == 0 || addCity(map, city2) == 0){
		clearRoute(map, routeId);
		return NULL;
	}

	cityNeighbours* city1Data = getMap(map->graph, city1);
	cityNeighbours* city2Data = getMap(map->graph, city2);
	if(strcmp(city1, city2) == 0){
		clearRoute(map, routeId);
		return NULL;
	}
	//Sprawdzamy czy jest pętla.
	if(city2Data->prohibited == 1){
		clearRoute(map, routeId);
		return NULL;
	}

	city2Data->previousCity = strDuplicate(city1);
	city2Data->length = length;
	city2Data->routeYear = builtYear;

	listElement* neighbour = city1Data->head;

	while(neighbour != NULL && strcmp(neighbour->city, city2) != 0  ){
		neighbour = neighbour->next;
	}

	//Jeżeli odcinek pomiędzy city1 i city2 już istnieje w mapie,
	//sprawdzamy, czy dane długość i rok nie powodują konfliktu.
	if(neighbour != NULL){
		if(neighbour->length != length){
			clearRoute(map, routeId);
			return NULL;
		}


		if(neighbour->builtYear > builtYear){
			clearRoute(map, routeId);
			return NULL;
		}

	}

	//Dodajemy odcinek do drogi krajowej.
	if(map->routes[routeId-1] == NULL){
		map->routes[routeId-1] = getListElement(city1, 0, 0);
		tail = map->routes[routeId-1];
	}

	listElement* element = getListElement(city2, 0, 0);
	if(tail == NULL || element == NULL){
		clearRoute(map, routeId);
		return NULL;
	}
	tail->next = element;
	element->prev = tail;

	city1Data->prohibited = 1;
	city2Data->prohibited = 1;
	//Zwracamy wskaźnik na ogon listy drogi krajowej, aby efektywnie dołączać do niej kolejne miasta.
	return element;

}

void supplementRoads(Map* map, const char* city){
	cityNeighbours* cityData = getMap(map->graph, city);
	while(cityData->previousCity != NULL){
		addRoad(map, cityData->previousCity, city, cityData->length, cityData->routeYear);
		repairRoad(map, cityData->previousCity, city, cityData->routeYear);
		city = cityData->previousCity;
		cityData = getMap(map->graph, cityData->previousCity);
	}
	resetForDijkstra(map->graph);
}

void clearRoute(Map* map, unsigned routeId){
	listElement* element = map->routes[routeId-1];
	while(element != NULL){
		if(getMap(map->graph, element->city) != NULL){
			if(getMap(map->graph, element->city)->head == NULL){
				removeMap(map->graph, element->city);
			}
		}

		element = element->next;
	}

	if(map->routes[routeId-1] != NULL)
		deleteList(map->routes[routeId-1]);
	map->routes[routeId-1] = NULL;
	resetForDijkstra(map->graph);
}
