/** @file
 * Interfejs funkcji przetwarzających standardowe wejście.
 */
#ifndef INPUT_H_
#define INPUT_H_

#include <stdbool.h>

/** @brief Przetwarza jedną linię standardowego wejścia.
 * @param[in] map - wskaźnik na mapę dróg;
 * @param[in] end - wskaźnik na wartość logiczną, początkowo false, funkcja nada jej wartość true,
 * gdy dana linia była ostatnią na wejściu.
 * @return Wartość @p true, jeśli wykonanie komendy odpowiadającej danej linii powiodło się.
 * Wartość @p false w przeciwnym przypadku.
 */
bool executeLine(Map* map, bool* end);


#endif /* INPUT_H_ */
