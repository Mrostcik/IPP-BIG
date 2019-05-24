#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "map.h"
#include "input.h"

int main(){
	Map* map = newMap();
	int counter = 1;
	bool end = 0;

	while(1){
		bool result = executeLine(map, &end);
		if(end == 1)
			break;

		if(result == false){
			fprintf(stderr, "%s%d\n","ERROR " ,counter);
		}

		counter++;
	}
	
	deleteMap(map);
}
