/*
 * connection.c
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */
#include "connection.h"
#include <unistd.h>

//======================
// Creating a connection
//======================
connection_t *connection_new(node_t *source, node_t *destination)
{
	//Allocate memory
	connection_t *connection = (connection_t*) malloc(sizeof(connection_t));
	if(!connection)
		return NULL;

	//Initialize
	connection_init(connection, source, destination);

	return connection;
}

void connection_init(connection_t *connection, node_t *source, node_t *destination)
{
	//Set source and destination
	connection->source = connection->path[0] = source;
	connection->destination = connection->path[1] = destination;

	//Set path size
	connection->path_length = 2;

	//Set initial state
	connection->state = INVALID;
}

//==============
// Managing data
//==============
int connection_push(connection_t *connection, node_t *node)
{
	if(connection->path_length == MAX_PATH)
		return 0;

	connection->path[connection->path_length] = connection->path[connection->path_length-1];
	connection->path[connection->path_length-1] = node;

	connection->path_length++;

	return 1;
}

node_t *connection_pop(connection_t *connection)
{
	//Has to have at least 3 elements to pop(because the first element is the source and the last is the destination)
	if(connection->path_length <= 2)
		return NULL;

	node_t *popped = connection->path[connection->path_length-2];
	connection->path[connection->path_length-2] = connection->path[connection->path_length-1];

	connection->path_length--;

	return popped;
}

//===========
// Operations
//===========
int connection_establish(connection_t *connection)
{
	//Check if in the right state
	if(connection->state != VALID) {
		fprintf(stderr, "Error establishing connection. Connection state is not VALLID.\n");
		return 0;
	}

	//Get connections between nodes
	int i;
	for(i = 1; i < connection->path_length; i++) {
		if(!node_use_connection(connection->path[i], connection->path[i-1]->name)) {
			//If a connection is not available, free the connections already established
			fprintf(stderr, "Error. No available connection between %d and %d.\n", i-1, i);

			for(i = i-1; i > 0; i--)
				node_free_connection(connection->path[i], connection->path[i-1]->name);

			connection->state = INVALID;

			return 0;
		}
	}

	//If established
	connection->state = CONNECTED;

	return 1;
}

int connection_stop(connection_t *connection)
{
	//Check if connected
	if(connection->state != CONNECTED) {
		fprintf(stderr, "Error stopping connection. Connection not in CONNECTED state.\n");
		return 0;
	}

	//Free connections
	int i;
	for(i = 1; i < connection->path_length; i++)
		node_free_connection(connection->path[i], connection->path[i-1]->name);

	//Set state
	connection->state = DISCONNECTED;

	return 1;
}

connection_t *connection_reverse(connection_t *connection)
{
	//Allocate memory
	connection_t *reversed = (connection_t*) malloc(sizeof(connection_t));

	//Copy original
	*reversed = *connection;

	//Reverse
	int i;
	for(i = 0; i < reversed->path_length; i++)
		reversed->path[i] = connection->path[reversed->path_length-1-i];

	//Reset source and destination
	reversed->source = connection->destination;
	reversed->destination = connection->source;

	return reversed;
}

void connection_printPath(const connection_t *connection)
{
	//Print path
	printf("Path from %s to %s: ", connection->source->name, connection->destination->name);
	int i;
	for(i = 0; i < connection->path_length-1; i++) {
		printf("%s->", connection->path[i]->name);
	}
	printf("%s.\n", connection->path[i]->name);
}

int connection_send(const connection_t *connection, const char *message)
{
	if(connection == NULL) {
		fprintf(stderr, "Null connection.\n");
		return 0;
	}

	if(connection->state != CONNECTED) {
		fprintf(stderr, "Send error. Connection is not in the connected state.\n");
		return 0;
	}

	//Pretend that it's sending a message
	printf("Sending message from %s...\n", connection->source->name);

	//Print path
	printf("Message path: ");
	int i;
	for(i = 0; i < connection->path_length-1; i++) {
		printf("%s->", connection->path[i]->name);

		#ifndef NO_SLEEP
			fflush(stdout);
			sleep(1);
		#endif
	}
	printf("%s.\n", connection->path[i]->name);

	//Print message
	printf("%s received message: \"%s\".\n", connection->destination->name, message);

	return 1;
}
