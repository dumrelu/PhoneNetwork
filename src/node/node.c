/*
 * node.c
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */
#include "node.h"

//================
// Creating a node
//================
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

//==============
// Managing data
//==============
void node_setName(node_t *node, const char *name)
{
	if(strlen(name) < MAX_NODENAME_LENGTH)
		strcpy(node->name, name);
	else
		strcpy(node->name, "Invalid Name");
}

void node_add_one(node_t *node, node_t *neighbor, int n_connections)
{	//Assume that the number of neighbors is valid
	//Add neighbor
	node->neighbors[node->n_neighbors] = neighbor;
	node->n_connections[node->n_neighbors] = n_connections;

	//Update number of neighbors
	node->n_neighbors++;
}

int node_add_neighbor(node_t *node, node_t *neighbor, int n_connections)
{
	//Check if both nodes have an available slot
	if(node->n_neighbors == MAX_NEIGHBORS || neighbor->n_neighbors == MAX_NEIGHBORS)
		return 0;

	//Make the link(both ways)
	node_add_one(node, neighbor, n_connections);
	node_add_one(neighbor, node, n_connections);

	return 1;
}

int node_get_neighbor_index(const node_t *node, const char *neighborName)
{
	//Iterate through the neighbors and check if the names match
	int i;
	for(i = 0; i < node->n_neighbors; i++)
		if(strcmp(node->neighbors[i]->name, neighborName) == 0)
			return i;

	return -1;
}

node_t *node_get_neighbor(const node_t *node, const char *neighborName)
{
	//Get the neighbor's index
	int neighborIndex = node_get_neighbor_index(node, neighborName);

	//Check if the index is valid
	if(neighborIndex == -1)
		return NULL;

	return node->neighbors[neighborIndex];
}

void node_remove_one(node_t *node, int neighborIndex)
{	//Assume the index is valid
	//Swap the last items to the ones at the given index, and then reduce the size
	node->neighbors[neighborIndex] = node->neighbors[node->n_neighbors-1];
	node->n_connections[neighborIndex] = node->n_connections[--node->n_neighbors];
}

int node_remove_neighbor(node_t *node, const char *neighborName)
{
	//Fetch the neighbor index
	int neighborIndex = node_get_neighbor_index(node, neighborName);
	if(neighborIndex == -1)
		return 0;

	//Fetch the neighbor node and then the index to "node" relative to the "neighbor" node
	node_t *neighbor = node->neighbors[neighborIndex];
	int nodeIndex = node_get_neighbor_index(neighbor, node->name);

	//Remove links
	node_remove_one(node, neighborIndex);
	node_remove_one(neighbor, nodeIndex);

	return 1;
}

void node_print(const node_t *node) {
	if(!node) {
		printf("Null Node.\n");
		return;
	}

	printf("%s(%d neighbors)", node->name, node->n_neighbors);

	if(node->n_neighbors > 0) {
		printf(": ");

		int i;
		for(i = 0; i < node->n_neighbors-1; i++)
			printf("%s(%d available), ", node->neighbors[i]->name, node->n_connections[i]);
		printf("%s(%d available)", node->neighbors[i]->name, node->n_connections[i]);
	}

	printf(".\n");
}
