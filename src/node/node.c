/*
 * node.c
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */
#include "node.h"

node_t *node_new(const char *name)
{
	//Allocate memory for the new node
	node_t *node = (node_t*) malloc(sizeof(node_t));

	//Initialize node
	node_init(node, name);

	return node;
}

void node_init(node_t *node, const char *name)
{
	//Set the name
	node_setName(node, name);

	//No neighbors
	node->n_neighbors = 0;

	//Prepare neighbors and n_connections arrays
	int i;
	for(i = 0; i < MAX_NEIGHBORS; i++) {
		node->neighbors[i] = NULL;
		node->n_connections[i] = 0;
	}
}
