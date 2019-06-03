/*
 * Main.cpp
 *
 *  Created on: 3 de mai de 2018
 *      Author: cesar
 */

#include "Main.hpp"

// required MPI include file
#include "mpi/mpi.h"
#include <stdio.h>
#include <unistd.h>
// mpirun -n n_processes executable args
// mpirun -n 5 INE5418 caseArg

char buffer[bufsize];
int numtasks, rank;

enum { caseAnel, caseServidor, caseLamport };
const char* caseName[] = {
		[caseAnel] = "anel",
		[caseServidor] = "servidor",
		[caseLamport] = "lamport",
};

int main(int argc, char *argv[])  {
	// initialize MPI
	MPI_Init(&argc,&argv);
	// get number of tasks
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	// get my rank
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	unsigned char caseArg = atoi(argv[1%argc]);
	if(rank == --numtasks) {
		if(caseArg < 0 || 3 <= caseArg) {
			fputs("Valor de simulação inválido\n", stdout);
			for(caseArg = 0; caseArg < 3; ++caseArg) {
				printf("%d - %s\n", caseArg, caseName[caseArg]);
			}
		} else {
			printf("Simulando %s\n", caseName[caseArg]);
			printProcess();
		}
	} else {
		srand(rank);
		switch (caseArg) {
		case caseAnel:
			anel();
			break;
		case caseServidor:
			servidor();
			break;
		case caseLamport:
			lamport();
			break;
		default: break;
		}
	}

	// done with MPI
	MPI_Finalize();

}
