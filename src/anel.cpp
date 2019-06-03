/*
 * anel.cpp
 *
 *  Created on: 4 de mai de 2018
 *      Author: cesar
 */

#include "mpi/mpi.h"
#include <unistd.h>
#include <cstdlib>
#include "Main.hpp"

void anel(){
	enum { token };
	const int dest = (rank+1)%numtasks,
			source = (rank-1+numtasks)%numtasks;
	MPI_Request req;
	if(rank == 0) {
		MPI_Send(nullptr, 0, MPI_CHAR, dest,   token, MPI_COMM_WORLD);
	}
	MPI_Irecv	(nullptr, 0, MPI_CHAR, source, token, MPI_COMM_WORLD, &req);
	while(true) {
		sleep(1); // porção de código
		int flag;
		MPI_Status stat;
		if(quer_RC()) {
			MPI_Wait(&req, &stat);
			flag = 1;
			acessar_RC();
		} else {
			MPI_Test(&req, &flag, &stat);
		}
		if(flag == 1) {
			_printf("Repassa token");
			MPI_Send (nullptr, 0, MPI_CHAR, dest,   token, MPI_COMM_WORLD);
			MPI_Irecv(nullptr, 0, MPI_CHAR, source, token, MPI_COMM_WORLD, &req);
		}
	}
}

