#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priorityQueue.h"
#include "strdup.h"

/** Struktura węzła kolejki.
 * Trzyma odpowiadające mu miasto i odległość.
 */
typedef struct queueNode{
	///napis reprezentujący miasto
	const char* city;
	///odległość
	int length;
} queueNode;

/** Struktura kolejki.
 * Trzyma tablicę wszystkich węzłów oraz
 * obecny rozmiar i maksymalny rozmiar kolejki.
 */
typedef struct queue{
	///tablica węzłów
	queueNode** nodes;
	///obecny rozmiar kolejki
	int count;
	///maksymalny rozmiar kolejki
	int size;
} queue;

int getCount(queue* queue){
	return queue->count;
}

queue* createQueue(int size){
	queue* newQueue = (queue*)malloc(sizeof(queue));
	newQueue->count = 0;
	newQueue->size = size;
	newQueue->nodes = (queueNode**)malloc(sizeof(queueNode*) * size);
	return newQueue;
}

void push(queue* queue, const char* city, int length){
	if(queue->count + 1 >= queue->size){
		queue->size *= 2;
		queue->nodes = (queueNode**)realloc(queue->nodes, sizeof(queueNode*) * queue->size);
	}

	queueNode* newNode = (queueNode*)malloc(sizeof(queueNode));
	int index = queue->count;
	queue->count++;
	int parentIndex = (index-1)/2;

	while(index>0 && queue->nodes[parentIndex]->length > length){
		queue->nodes[index] = queue->nodes[parentIndex];
		index = parentIndex;
		parentIndex = (index-1)/2;
	}

	newNode->city = strDuplicate(city);
	newNode->length = length;
	queue->nodes[index] = newNode;



}

const char* pop(queue* queue){
	if(queue->count == 0 )
		return NULL;

	queue->count -= 1;
	queueNode* root = queue->nodes[0];
	const char* city = strDuplicate(root->city);
	queueNode* node = queue->nodes[queue->count];
	int index = 0;
	int leftSon = 1;

	while(leftSon < queue->count){
		if(leftSon + 1 < queue->count && queue->nodes[leftSon+1]->length < queue->nodes[leftSon]->length)
			leftSon = leftSon + 1;
		if(node->length <= queue->nodes[leftSon]->length)
			break;
		queue->nodes[index] = queue->nodes[leftSon];
		index = leftSon;
		leftSon = leftSon * 2 + 1;
	}

	queue->nodes[index] = node;
	free((void*) root->city);
	free(root);

	return city;


}

void deleteQueue(queue* queue){
	for(int i = 0; i < queue->count; i++){
		free((void*) queue->nodes[i]->city);
		free(queue->nodes[i]);
	}
	free(queue->nodes);
	free(queue);
}


