/*
 * connection.c
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */
#include "connection.h"

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

	connection_t *popped = connection->path[connection->path_length-2];
	connection->path[connection->path_length-2] = connection->path[connection->path_length-1];

	connection->path_length--;

	return popped;
}
