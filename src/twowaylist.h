/** @file
 * Interfejs klasy reprezentującej listę.
 */
#ifndef TWOWAYLIST_H_
#define TWOWAYLIST_H_

/** Struktura elementu listy.
 *  Służy do trzymania sąsiada danego miasta,
 *  jako trójki (miasto sąsiedzkie, długość, rok remontu drogi).
 */
typedef struct listElement{
	///wskaźnik na następny element
    struct listElement* next;
    ///wskaźnik na poprzedni element
    struct listElement* prev;
    ///wskaźnik na napis reprezentujący miasto
    const char* city;
    ///długość
    unsigned length;
    ///rok budowy/remontu
    int builtYear;
} listElement;

/** Tworzy nowy element listy.
 * @param[in] city  - wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length - długość drogi;
 * @param[in] builtYear - rok budowy/remontu drogi;
 * @return Wskaźnik na nowy element listy.
 */
listElement* getListElement(const char* city, unsigned length, int builtYear);

/** Sprawdza czy lista zawiera element odpowiadający danemu miastu.
 * @param[in] element - wskaźnik na głowę listy;
 * @param[in] city - wskażnik na napis reprezentujący nazwę miasta;
 * @return Wartość @p true, jeśli lista zawiera element odpowiadający danemu miastu.
 * Wartość @p false, jeśli nie zawiera.
 */
bool contains(listElement* element, const char* city);

/** Zmienia rok elementu listy odpowiadającemu danemu miastu.
 * @param[in] element - wskaźnik na głowę listy;
 * @param[in] city - wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] year - rok remontu drogi;
 * @return Wartość @p true, jeśli rok został zmieniony.
 * @return Wartość @p true, gdy dany rok jest mniejszy niż obecny.
 */
bool modifyYear(listElement* element, const char* city, int year);

/** Usuwa element listy.
 * @param[in] element - wskaźnik na element listy;
 */
void deleteListElement(listElement* element);

/** Usuwa całą listę.
 * @param[in] element - wskaźnik na głowę listy;
 */
void deleteList(listElement* element);
#endif /* TWOWAYLIST_H_ */
