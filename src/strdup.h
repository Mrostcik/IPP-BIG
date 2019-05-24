/** @file
 * Interfejs funkcji operujących na napisach.
 */
#ifndef strDuplicate_H_
#define strDuplicate_H_
#include <string.h>

/** @brief Kopiuje podany napis
 * @param[in] s  -  wskaźnik na napis
 * @return Wskaźnik na nowy napis lub NULL, jeśli nie udało się zalokować pamięci.
 */
char *strDuplicate(const char *s) ;

#endif /* strDuplicate_H_ */
