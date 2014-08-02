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
