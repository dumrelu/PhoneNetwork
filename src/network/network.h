/*
 * network.h
 *
 *  Created on: Aug 3, 2014
 *      Author: relu
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../connection/connection.h"

//Maximum number of nodes in a network
#define MAX_NODES 100

//Maximum length of a filename
#define MAX_FILENAME_LENGTH 40

//Maximum number of connections
#define MAX_CONNECTIONS 50

//If NO_STORE is not defined, every time a new connection is requested, the connections array
//will be searched to find a connection with the matching source and destination, thus reducing the connection time
//if the connection is already established.
//If NO_STORE is defined, connect() will return a new connection every time
#define NO_STORE

//==================================================
// The structure that holds the nodes in the network
//==================================================
struct network {
	//The file in which the network is stored
	char filename[MAX_FILENAME_LENGTH];

	//The nodes on the network
	node_t *nodes[MAX_NODES];
	//Current number of nodes
	int n_nodes;

	//Path finding algorithm
	connection_t *(*algorithm)(const struct network*, node_t*, node_t*);

	//The established connections
	connection_t *connections[MAX_CONNECTIONS];
	int n_connections;
};



//=========
// Typedefs
//=========
typedef struct network network_t;
//Algorithm that finds the path between the first node and second one, and return a connection.
typedef connection_t *(*findAlgorithm)(const network_t*, node_t*, node_t*);



//=========
// Creation
//=========
/**
 * Loads the nodes from the given file and sets the find algorithm.
 */
network_t *network_create(
	const char *filename,
	findAlgorithm algorithm
);

void network_read(
	network_t *network
);



//==========
//Operations
//==========
/**
 * Returns the index of the network with the given name.
 */
int network_index(
	const network_t *network,
	const char *nodename
);

/**
 * Searches for the node with the name "nodename" in the network.
 */
node_t *network_lookup(
	const network_t *network,
	const char *nodename
);

/**
 * Adds a node to the network
 */
int network_add_node(
	network_t *network,
	node_t *node
);

/**
 * Removes the node with the given name from the network.
 * Note: if the node is contained in a path, that path will be recalculated.
 */
int network_remove_node(
	network_t *network,
	const char *nodename
);

/**
 * Recalculate the given path.
 */
int network_recalculate(
	network_t *network,
	connection_t *connection
);

/**
 * Recalculates all the paths.
 */
void network_refresh(
	network_t *network
);

/**
 * Uses the algorithm to make a connection between 2 nodes and then establishes it.
 */
connection_t *connect(
	network_t *network,
	node_t *source,
	node_t *destination
);

/**
 * Disconnects the given connection.
 */
int disconnect(
	network_t *network,
	connection_t *connection
);



//=============
// Finalization
//=============
int network_write(
	const network_t *network
);

void network_shutdown(
	network_t *network
);

#endif /* NETWORK_H_ */
