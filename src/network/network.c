/*
 * network.c
 *
 *  Created on: Aug 3, 2014
 *      Author: relu
 */
#include "network.h"

network_t *network_create(const char *filename, findAlgorithm algorithm)
{
	//Allocate memory
	network_t *network = (network_t*) malloc(sizeof(network_t));
	if(!network)
		return NULL;

	//Set the file name
	if(strlen(filename) < MAX_FILENAME_LENGTH)
		strcpy(network->filename, filename);
	else
		return NULL;

	//Set the counters to 0
	network->n_nodes = 0;
	network->n_connections = 0;

	//Read nodes from file
	network_read(network);

	return network;
}

void network_read(network_t *network)
{
	//Buffers
	char nodeName1[MAX_NODENAME_LENGTH];
	char nodeName2[MAX_NODENAME_LENGTH];
	int n_connections;

	//Open file
	FILE *file = fopen(network->filename, "r");
	if(!file)
		return;

	while(fscanf(file, "%s %s %d", nodeName1, nodeName2, &n_connections) > 0) {
		//Lookup the first node, if it doesn't exist then create it
		node_t *node1 = network_lookup(network, nodeName1);
		if(!node1) {	//Create it and add it to the network
			node1 = node_new(nodeName1);
			network_add_node(network, node1);
		}

		//Lookup the second node, if it doesn't exist then create it
		node_t *node2 = network_lookup(network, nodeName2);
		if(!node2) { 	//Create it and add it to the network
			node2 = node_new(nodeName2);
			network_add_node(network, node2);
		}

		//Add a link between them, if it doesn't exist already
		if(node_get_neighbor(node1, node2->name) == NULL) {
			node_add_neighbor(node1, node2, n_connections);
		}
	}

	//Close file
	fclose(file);
}



int network_index(const network_t *network, const char *nodename)
{
	//Iterate through the nodes and see if the names match
	int i;
	for(i = 0; i < network->n_nodes; i++)
		if(strcmp(network->nodes[i]->name, nodename) == 0)
			return i;

	return -1;
}

node_t *network_lookup(const network_t *network, const char *nodename)
{
	//Get the index
	int index = network_index(network, nodename);
	if(index == -1)
		return NULL;

	return network->nodes[index];
}

int network_add_node(network_t *network, node_t *node)
{
	//Check if there are any more slots available
	if(network->n_nodes == MAX_NODES)
		return 0;

	//Add to array
	network->nodes[network->n_nodes++] = node;

	return 1;
}
