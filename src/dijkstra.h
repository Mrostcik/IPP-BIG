/** @file
 * Interfejs algorytmów przeszukiwania grafów.
 */
#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <stdbool.h>
#include "hashMap.h"

/** @brief Wyznacza najkrótszą drogę pomiędzy city1, a city2 i
 *  zapisuje ją w strukturach cityNeighbours* dla danych miast.
 *  @param[in] map - wskaźnik na hashMapę;
 *  @param[in] city1 - wskaźnik na napis reprezentujący nazwę miasta startowego;
 *  @param[in] city2 - wskaźnik na napis reprezentujący nazwę miasta docelowego,
 *  można wywołać z NULL, wtedy zostaną znalezione najkrótsze drogi z city1 do wszystkich miast;
 *  @return Wartość @p true, jeśli najkrótsza droga została znaleziona.
 *  Wartość @p false, jeśli alokacja pamięci była nieudana.
 */
bool dijkstra(hashMap* map, const char* city1, const char* city2);

/** @brief Sprawdza czy znaleziona najkrótsza droga jest jednoznaczna i bez pętli.
 * @param[in] map - wskaźnik na hashMapę;
 * @param[in] city1 - wskaźnik na napis reprezentujący nazwę miasta startowego;
 * @param[in] city2 - wskaźnik na napis reprezentujący nazwę miasta docelowego;
 * @return Wartość @p true, jeśli droga jest jednoznaczna i bez pętli.
 * Wartość @p false w przeciwnym przypadku.
 */
bool checkDijkstra(hashMap* map, const char* city1, const char* city2);

#endif /* DIJKSTRA_H_ */
