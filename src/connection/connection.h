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

/**
 * Used for the send function to identify which is the sender and which is the receiver.
 */
typedef enum {
	FIRST,
	SECOND
} connection_destination_t;

//Maximum path between 2 nodes
#define MAX_PATH 50

//===========================
// Connection between 2 nodes
//===========================
typedef struct {
	//The connected nodes
	node_t *first, node_t *second;

	//Path between the nodes
	node_t *path[MAX_PATH];		//The path from the first node to the second
	int path_length;			//The length of the path
	//Node: path[0] = first, path[path_length-1] = second

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
	node_t *first,
	node_t *second
);

/**
 * Initialize a connection
 */
void connection_init(
	connection_t *connection,
	node_t *first,
	node_t *second
);



//==============
// Managing data
//==============
/**
 * Add a new node to the path.
 */
void connection_push(
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
 * Sends a message from FIRST to SECOND, or from SECOND to FIRST using a connection in the CONNECTED state.
 */
int connection_send(
	const connection_t *connection,
	connection_destination_t destination
);

#endif /* CONNECTION_H_ */
