/*
 * main.c
 *
 *  Created on: Aug 2, 2014
 *      Author: relu
 */
#include <stdio.h>
#include "node/node.h"

int main(int argc, char *argv[])
{
	node_t *node1 = node_new("Node1");
	node_t *node2 = node_new("Node2");
	node_t *node3 = node_new("Node3");

	node_add_neighbor(node1, node2, 1);
	node_add_neighbor(node2, node3, 1);

	node_use_connection(node1, node2->name);
	node_free_connection(node1, node2->name);

	node_print(node1);
	node_print(node2);
	node_print(node3);

	node_free(node2);

	node_print(node1);
	node_print(node3);

	node_free(node1);
	node_free(node3);

	return 0;
}
