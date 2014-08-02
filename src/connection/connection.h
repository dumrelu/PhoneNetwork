/*
 * connection.h
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "../node/node.h"

/**
 * Connection states
 */
typedef enum {
	INVALID, 		//Bad path
	VALID, 			//Good path
	DISCONNECTED,	//Was once connected, then it normally disconnected
	CONNECTED		//Ready to send messages
} connection_state_t;

//Maximum path between 2 nodes
#define MAX_PATH 50

//===========================
// Connection between 2 nodes
//===========================
typedef struct {
	//The connected nodes
	node_t *source, *destination;

	//Path between the nodes
	node_t *path[MAX_PATH];		//The path from the first node to the second
	int path_length;			//The length of the path
	//Node: path[0] = source, path[path_length-1] = destination

	//Connection state
	connection_state_t state;
} connection_t;



//======================
// Creating a connection
//======================
/**
 * Allocate memory for a new connection and initialize it.
 */
connection_t *connection_new(
	node_t *source,
	node_t *destination
);

/**
 * Initialize a connection
 */
void connection_init(
	connection_t *connection,
	node_t *source,
	node_t *destination
);



//==============
// Managing data
//==============
/**
 * Add a new node to the path.
 */
int connection_push(
	connection_t *connection,
	node_t *node
);

/**
 * Remove the last node added from the path.
 */
node_t *connection_pop(
	connection_t *connection
);



//===========
// Operations
//===========
/**
 * Establishes a connection in the VALID state.
 */
int connection_establish(
	connection_t *connection
);

/**
 * Stops a connection in the CONNECTED state.
 */
int connection_stop(
	connection_t *connection
);

/**
 * Reverses a connection(aka source will become destination and vice versa)
 */
connection_t *connection_reverse(
	connection_t *connection
);

/**
 * Sends a message from FIRST to SECOND, or from SECOND to FIRST using a connection in the CONNECTED state.
 */
int connection_send(
	const connection_t *connection,
	const char *message
);

#endif /* CONNECTION_H_ */
