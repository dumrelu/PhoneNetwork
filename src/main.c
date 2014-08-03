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

	//Print the nodes in the network
	int i;
	for(i = 0; i < network->n_nodes; i++)
		node_print(network->nodes[i]);

	return 0;
}
