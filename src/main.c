/*
 * main.c
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */
#include <stdio.h>
#include "network/network.h"

int main(int argc, char *argv[])
{
	network_t *network = network_create("test_network.txt", NULL);

	/*node_t *node1 = node_new("Node1");
	node_t *node2 = node_new("Node2");
	node_t *node3 = node_new("Node3");

	node_add_neighbor(node1, node2, 1);
	node_add_neighbor(node2, node3, 1);

	connection_t *connection = connection_new(node1, node3);
	connection->state = VALID;

	connection_push(connection, node2);

	//Reverse
	connection_t *buffer = connection;
	connection = connection_reverse(connection);
	free(buffer);

	int i;
	for(i = 0; i < connection->path_length; i++)
		node_print(connection->path[i]);

	printf("Establish connection: %d\n", connection_establish(connection));

	for(i = 0; i < connection->path_length; i++)
			node_print(connection->path[i]);

	//Send message
	connection_send(connection, "Hello World!");

	printf("Stop connection: %d\n", connection_stop(connection));

	for(i = 0; i < connection->path_length; i++)
				node_print(connection->path[i]);

	node_free(node1);
	node_free(node2);
	node_free(node3);

	free(connection);*/

	return 0;
}
