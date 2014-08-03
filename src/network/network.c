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

	//Set the algoritm
	network->algorithm = algorithm;

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

int network_remove_node(network_t *network, const char *nodename)
{
	//Get the node index
	int index = network_index(network, nodename);
	if(index == -1)
		return 0;

	//Get the node
	node_t *node = network->nodes[index];

	//Iterate through the connections and recalculate the ones that contain the soon to be removed node
	int i, j;
	for(i = 0;i < network->n_connections; i++) {
		//Get the connection
		connection_t *connection = network->connections[i];

		//First check if the soon to be removed node is the source or destination of the connection
		if(connection->source == node || connection->destination == node) {
			//Disconnect the connection and continue to the next connection
			disconnect(network, connection);
			continue;
		}

		//Check if the soon to be removed node is contained in the path of the connection
		//Node: path[0] = source and path[path_length-1] = destination => skip them
		for(j = 1; j < connection->path_length-1; j++)
			if(connection->path[j] == node) { //Recalculate the path
				network_recalculate(network, connection);
				break;
			}
	}

	//Remove the node from the network
	network->nodes[index] = network->nodes[--network->n_nodes];

	return 1;

}

int network_recalculate(network_t *network, connection_t *connection)
{
	//Recalculate the path
	connection_t *recalculated = network->algorithm(network, connection->source, connection->destination);

	//Stop the original connection
	connection_stop(connection);

	//Copy the recalculated into connection and free recalculated
	*connection = *recalculated;
	free(recalculated);

	//If the state is not valid or the state is valid and connection cannot be established
	if(connection->state != VALID || !connection_establish(connection)) {
		disconnect(network, connection);
		return 0;
	}

	return 1;
}

void network_refresh(network_t *network)
{
	//Will hold the current connection
	connection_t *connection;

	//Iterate through the connections and recalculate them
	int i;
	for(i = 0; i < network->n_connections; i++) {
		//Fetch
		connection = network->connections[i];

		//Recalculate
		if(!network_recalculate(network, connection))
			i--;
	}
}

connection_t *connect(network_t *network, node_t *source, node_t *destination)
{
	//Check if the network cand hold more connections
	if(network->n_connections == MAX_CONNECTIONS)
		return NULL;

	#ifndef NO_STORE
	//Iterate through the connections to find a connection from source to destination
	int i;
	for(i = 0; i < network->n_connections; i++) {
		//Fetch it
		connection_t * connection = network->connections[i];

		//Check if same source and destination
		if(connection->source == source && connection->destination == destination)
			return connection;
	}
	#endif

	//Calculate path
	connection_t *connection = network->algorithm(network, source, destination);

	//Check if valid
	if(connection->state == VALID) {
		//Establish it
		if(connection_establish(connection)) {
			//Add it to the array
			network->connections[network->n_connections++] = connection;
			return connection;
		}
	}

	//Free connection if not established
	free(connection);

	return NULL;
}

int disconnect(network_t *network, connection_t *connection)
{
	//Iterate through the connections to find the given connection
	int i;
	for(i = 0; i < network->n_connections; i++)
		if(network->connections[i] == connection)
			break;

	//Check if found
	if(i == network->n_connections)
		return 0;

	//Set state to disconnected
	connection_stop(connection);

	//Remove from array
	network->connections[i] = network->connections[--network->n_connections];

	return 1;
}



int network_write(const network_t *network)
{
	//Open file
	FILE *file = fopen(network->filename, "w");
	if(!file)
		return 0;

	//Write links to disk
	int i, j;
	//Iterate through nodes
	for(i = 0; i < network->n_nodes; i++) {
		//Fetch current node
		node_t *node = network->nodes[i];

		//Iterate through neighbors
		for(j = 0; j < node->n_neighbors; j++) {
			//Fetch current neighbor
			node_t *neighbor = node->neighbors[j];

			//Fetch available connections for current neighbor
			int n_connections = node->n_connections[j];

			//Write to disk
			fprintf(file, "%s %s %d\n", node->name, neighbor->name, n_connections);
		}
	}

	//Close file
	fclose(file);

	return 1;
}

void network_shutdown(network_t *network) {
	//Disconnect and free all the connections
	while(network->n_connections) {
		connection_t *connection = network->connections[0];
		disconnect(network, connection);
		free(connection);
	}

	//Write network to disk
	network_write(network);

	//Free nodes
	int i;
	for(i = 0; i < network->n_nodes; i++)
		free(network->nodes[i]);

	//Free network
	free(network);
}
