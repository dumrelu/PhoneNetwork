/*
 * main.c
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */
#include <stdio.h>
#include "connection/connection.h"

int main(int argc, char *argv[])
{
	node_t *node1 = node_new("Node1");
	node_t *node2 = node_new("Node2");
	node_t *node3 = node_new("Node3");
	node_t *node4 = node_new("Node4");

	connection_t *connection = connection_new(node1, node4);

	connection_push(connection, node2);
	connection_pop(connection);
	connection_push(connection, node3);

	int i;
	for(i = 0; i < connection->path_length; i++)
		node_print(connection->path[i]);

	node_free(node1);
	node_free(node2);
	node_free(node3);
	node_free(node4);

	return 0;
}
