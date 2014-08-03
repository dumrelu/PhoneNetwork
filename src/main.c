/*
 * main.c
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */
#include <stdio.h>
#include "network/network.h"
#include "algorithm/backtracking.h"

void make_connection(network_t* network);
void stop_connection(network_t* network);
void print_network(const network_t *network);

int main(int argc, char *argv[])
{
	//Load network from a file, and set the algorithm to backtracking
	//The network file contains the network from the challenge.
	network_t *network = network_create("test_network.txt", backtracking_algorithm);

	//Indicates if a connection should be established or stopped
	int what_do;
	enum {
		CONNECT = 1,
		DISCONNECT,
		NETWORK_STATE
	};
	int condition = 1;

	while(condition) {
		//Get instruction
		printf("Next instruction(1=Connect, 2=Disconnect, 3=Network state, other=Exit): ");
		scanf("%d", &what_do);

		switch(what_do) {
		case CONNECT:
			make_connection(network);
			break;

		case DISCONNECT:
			stop_connection(network);
			break;

		case NETWORK_STATE:
			print_network(network);
			break;

		default:
			condition = 0;
			break;
		}
	}

	//Shut down network
	network_shutdown(network);

	return 0;
}

void make_connection(network_t* network)
{
	//Read source
	char sourceName[MAX_NODENAME_LENGTH];

	printf("Source name: ");
	scanf("%s", sourceName);

	node_t *source = network_lookup(network, sourceName);
	if(!source) {
		printf("Invalid source(%s).\n", sourceName);
		return;
	}

	//Read destination
	char destinationName[MAX_NODENAME_LENGTH];

	printf("Destination name: ");
	scanf("%s", destinationName);

	node_t *destination = network_lookup(network, destinationName);
	if(!destination) {
		printf("Invalid destination(%s).\n", destinationName);
		return;
	}

	//Make or get connection (if NO_STORE is defined then it will make a new connection every time, else it will reuse and existing connection)
	connection_t *connection = connect(network, source, destination);
	if(!connection) {
		printf("No path from %s and %s.\n", sourceName, destinationName);
		return;
	}

	//Send message between nodes
	connection_printPath(connection);
}

void stop_connection(network_t *network)
{
	//Read source
	char sourceName[MAX_NODENAME_LENGTH];

	printf("Source name: ");
	scanf("%s", sourceName);

	node_t *source = network_lookup(network, sourceName);
	if(!source) {
		printf("Invalid source(%s).\n", sourceName);
		return;
	}

	//Read destination
	char destinationName[MAX_NODENAME_LENGTH];

	printf("Destination name: ");
	scanf("%s", destinationName);

	node_t *destination = network_lookup(network, destinationName);
	if(!destination) {
		printf("Invalid destination(%s).\n", destinationName);
		return;
	}

	//Find the connections with the same source and destination and stop them
	int i;
	for(i = 0; i < network->n_connections; i++)
		if(network->connections[i]->source == source && network->connections[i]->destination == destination) {
			printf("Disconnecting %s and %s of length %d: ", sourceName, destinationName, network->connections[i]->path_length);
			printf("%d.\n", disconnect(network, network->connections[i]));
			i--;
		}
}

void print_network(const network_t *network)
{
	//Print the nodes
	printf("Nodes:\n");
	int i;
	for(i = 0; i < network->n_nodes; i++) {
		printf("\t->");
		node_print(network->nodes[i]);
	}

	//Print connections
	printf("Connections:\n");
	for(i = 0; i < network->n_connections; i++) {
		printf("\t->Connection from %s to %s of length %d.\n",
				network->connections[i]->source->name,
				network->connections[i]->destination->name,
				network->connections[i]->path_length);
	}
}
