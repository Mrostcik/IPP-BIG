/** @file
 * Interfejs klasy reprezentującej kolejkę priorytetową.
 */
#ifndef PRIORITYQUEUE_H_
#define PRIORITYQUEUE_H_

/** Struktura węzła kolejki.
 * Trzyma odpowiadające mu miasto i odległość.
 */
typedef struct queueNode queueNode;

/** Struktura kolejki.
 * Trzyma tablicę wszystkich węzłów oraz
 * obecny rozmiar i maksymalny rozmiar kolejki.
 */
typedef struct queue queue;

/** Zwraca obecny rozmiar kolejki.
 * @param[in] queue - wskaźnik na kolejkę;
 * @return Rozmiar kolejki.
 */
int getCount(queue* queue);

/** Tworzy nową kolejkę.
 * @param[in] size - początkowy rozmiar kolejki, kolejka rozszerza sie w miarę potrzeby;
 * @return Wskażnik na nowo-utworzoną kolejkę lub NULL.
 */
queue* createQueue(int size);

/** Dodaje element o etykiecie danego miasto i długości na przód kolejki.
 * @param[in] queue - wskaźnik na kolejkę;
 * @param[in] city - wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length - długość drogi;
 */
void push(queue* queue, const char* city, int length);

/** Usuwa pierwszy element z kolejki.
 * @param[in] queue - wskaźnik na kolejkę;
 * @return Wskaźnik na napis reprezentujący nazwę miasta odpowiadającą usuniętemu elementowi.
 */
const char* pop(queue* queue);

/** Usuwa całą kolejkę.
 * @param[in] queue - wskaźnik na kolejkę;
 */
void deleteQueue(queue* queue);

#endif /* PRIORITYQUEUE_H_ */
