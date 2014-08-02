/*
 * node.h
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */

#ifndef NODE_H_
#define NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//The maximum length of a node name
#define MAX_NODENAME_LENGTH 30

//The maximum number of neighbors a node can have
#define MAX_NEIGHBORS 20


//===============================================
// The structure used to represent a network node
//===============================================
struct node {
	/*The node name*/
	char name[MAX_NODENAME_LENGTH];

	/*The neighbors*/
	struct node *neighbors[MAX_NEIGHBORS];	//Array of pointers to the neighbors
	int n_neighbors;						//Current number of neighbors

	/*The number of connections that can be established between the node and its neighbors*/
	int n_connections[MAX_NEIGHBORS];		//n_connections[i] = number of available connections from the
											//current node to neighbors[i]
};



//=========
// Typedefs
//=========
typedef struct node node_t;



//================
// Creating a node
//================
/**
 * Allocates memory for a node and then initializes it.
 */
node_t *node_new(
	const char *name
);

/**
 * Initializes a node with the given name.
 */
void node_init(
	node_t *node,
	const char *name
);



//==============
// Managing data
//==============
/**
 * Sets the name of the node to the given name.
 */
void node_setName(
	node_t *node,
	const char *name
);

/**
 * Adds a new neighbor(if a slot is available).
 * Note: node_add_neighbor(A, B) = node_add_neighbor(B, A)
 */
int node_add_neighbor(
	node_t *node,
	node_t *neighbor,
	int n_connections
);

/**
 * Retrieves the index of the neighbor with the given name.
 * Returns -1 if it has no neighbor with that name.
 * Note: The index of a neighbor might change.
 */
int node_get_neighbor_index(
	const node_t *node,
	const char *neighborName
);

/**
 * Retrieves the neighbor with the given name.
 * Returns NULL if it has no neighbor with that name.
 */
node_t *node_get_neighbor(
	const node_t *node,
	const char *neighborName
);

/**
 * Removes the neighbor with the given name.
 * Return 1 if removed, 0 otherwise.
 */
int node_remove_neighbor(
	node_t *node,
	const char *neighborName
);

/**
 * Prints information about the node.
 */
void node_print(
	const node_t *node
);



//===========
// Operations
//===========
/**
 * Use a connection to a network.
 * Returns 1 if successful, 0 otherwise.
 */
int node_use_connection(
	node_t *node,
	const char *neighborName
);

/**
 * Frees(aka adds) a connection.
 */
void node_free_connection(
	node_t *node,
	const char *neighborName
);

//=============
// Finalization
//=============
/**
 * Uninits a node, and then frees the memory.
 */
void node_free(
	node_t *node
);

/**
 * Removes all the neighbor links.
 * Aka: If node A has a neighbor node B => B has A as a neighbor. After node_uninit(A), node B also removes A as
 * a neighbor.
 */
void node_uninit(
	node_t *node
);

#endif /* NODE_H_ */
