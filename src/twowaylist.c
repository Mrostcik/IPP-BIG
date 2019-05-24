#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "twowaylist.h"
#include "strdup.h"

listElement* getListElement(const char* city, unsigned length, int builtYear){
    listElement* newElement
                = (listElement*)malloc(sizeof(listElement));


    newElement->prev = NULL;
    newElement->next = NULL;
    newElement->city = strDuplicate(city);
    newElement->length = length;
    newElement->builtYear = builtYear;

    return newElement;
}

bool contains(listElement* element, const char* city){
	while(element!=NULL){
		if(strcmp(element->city, city) == 0)
			return true;

		element = element->next;
	}

	return false;
}

bool modifyYear(listElement* element, const char* city, int year){
	while(element!=NULL){
			if(strcmp(element->city, city) == 0){
				if(element->builtYear <= year){
					element->builtYear = year;
				    return true;
				}
				else
					return false;
			}

			element = element->next;
		}

		return false;
}


void deleteListElement(listElement* element){
	free((char*)element->city);
    //odpinamy i przepinamy wskazniki
    if(element->prev != NULL){
        if(element->next != NULL)
            element->prev->next = element->next;
        else
            element->prev->next = NULL;
    }

    if(element->next != NULL){
        if(element->prev != NULL)
            element->next->prev = element->prev;
        else
            element->next->prev = NULL;
    }

    free(element);
}

void deleteList(listElement* element){
	listElement* next = element->next;
	while(element){
		free((char*)element->city);
		free(element);
		element = next;
		if(element == NULL)
			break;

		next = element->next;
	}
}
