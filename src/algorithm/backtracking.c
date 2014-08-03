/*
 * backtracking.c
 *
 *  Created on: Aug 3, 2014
 *      Author: relu
 */
#include "backtracking.h"

void backtracking_compareSolutions(connection_t *globalSolution, const connection_t* currentSolution)
{
	//If this is the first solution or if it's a better solution, copy
	if(globalSolution->state != VALID || currentSolution->path_length < globalSolution->path_length) {
		*globalSolution = *currentSolution;
		globalSolution->state = VALID;
	}
}

void backtracking(node_t *currentNode, connection_t *currentSolution, connection_t *globalSolution)
{
	//Move to the neighbors
	int i;
	for(i = 0; i < currentNode->n_neighbors; i++) {
		//Check if solution
		if(currentNode->neighbors[i] == currentSolution->destination &&
				currentNode->n_connections[i]) {
			backtracking_compareSolutions(globalSolution, currentSolution);
			continue;
		}

		//If no more connections, skip to next neighbor
		if(!node_use_connection(currentNode, currentNode->neighbors[i]->name))
			continue;

		//Move forward
		connection_push(currentSolution, currentNode->neighbors[i]);
		backtracking(currentNode->neighbors[i], currentSolution, globalSolution);

		//Move backwards
		connection_pop(currentSolution);
		node_free_connection(currentNode, currentNode->neighbors[i]->name);
	}
}

connection_t *backtracking_algorithm(const network_t *network, node_t *source, node_t *destination)
{
	//Used as a buffer
	connection_t *currentSolution = connection_new(source, destination);

	//The solution will be found here
	connection_t *globalSolution = connection_new(source, destination);

	//Apply algorithm
	backtracking(source, currentSolution, globalSolution);

	//Free the buffer
	free(currentSolution);

	//Return solution, which can be either VALID(if a solution was found) or INVALID(otherwise)
	return globalSolution;
}
