/*
 * backtracking.h
 *
 *  Created on: Aug 3, 2014
 *      Author: relu
 */

#ifndef BACKTRACKING_H_
#define BACKTRACKING_H_

#include "../network/network.h"

//If the currentSolution is better that the global solution, then copy the current solution into the global one
void backtracking_compareSolutions(
	connection_t *globalSolution,
	const connection_t* currentSolution
);

//Apply backtracking algorithm
void backtracking(
	node_t *currentNode,
	connection_t *currentSolution,
	connection_t *globalSolution
);

//The algorithm
connection_t *backtracking_algorithm(
	const network_t *network,
	node_t *source,
	node_t *destination
);

#endif /* BACKTRACKING_H_ */
