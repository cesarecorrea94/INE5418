/*
 * lamport.cpp
 *
 *  Created on: 4 de jun de 2018
 *      Author: cesar
 */

#include "mpi/mpi.h"
#include <unistd.h>
#include <queue>
#include <cassert>
#include <algorithm>
#include "Main.hpp"

enum { Request, Concess };
const char* msgtag[] = {
		[Request] = "Request",
		[Concess] = "Concess",
};

void Bcast(int &my_LC) {
	_printf("(LC=%u) Broadcast request",
			++my_LC);
	for(int to = 0; to < numtasks; to++) {
		if(to == rank) continue;
		MPI_Send(&my_LC, 1, MPI_INT, to, Request, MPI_COMM_WORLD);
	}
}

void update_LC(int &my_LC, const int its_LC, const MPI_Status stat) {
	my_LC = std::max(my_LC, its_LC);
	_printf("(LC=%u) Receive '%s' from %u",
			++my_LC, msgtag[stat.MPI_TAG], stat.MPI_SOURCE);
}

void send_concess(int &my_LC, const int to) {
	_printf("(LC=%u) Send '%s' to %u",
			++my_LC, msgtag[Concess], to);
	MPI_Send(&my_LC, 1, MPI_INT, to, Concess, MPI_COMM_WORLD);
}

void atender_pendentes(int &my_LC, std::queue<int> &fila) {
	while(!fila.empty()) {
		send_concess(my_LC,fila.front());
		fila.pop();
	}
}

void atender_requisicoes(int &my_LC, int &its_LC, MPI_Request &req){
	MPI_Status stat;
	int flag;
	for(MPI_Test(&req, &flag, &stat); flag == 1;
			MPI_Test(&req, &flag, &stat) ) {
		assert(stat.MPI_TAG == Request);
		update_LC(my_LC, its_LC, stat);
		send_concess(my_LC, stat.MPI_SOURCE);
		MPI_Irecv(&its_LC, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
	}
}

void lamport(){
	MPI_Request req;
	int my_LC = 0, its_LC;
	MPI_Irecv(&its_LC, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
	while(true) {
		sleep(1);//porção de código
		atender_requisicoes(my_LC, its_LC, req);
		if(quer_RC()) {
			Bcast(my_LC);
			int msg_LC = my_LC;
			std::queue<int> fila;
			for(int n = 1; n < numtasks;) {
				MPI_Status stat;
				MPI_Wait(&req, &stat);
				update_LC(my_LC, its_LC, stat);
				switch(stat.MPI_TAG) {
				case Concess:
					n++; break;
				case Request:
					if(its_LC < msg_LC || (its_LC == msg_LC && stat.MPI_SOURCE < rank) ) {
						send_concess(my_LC, stat.MPI_SOURCE);
					} else {
						fila.push(stat.MPI_SOURCE);
					}
					break;
				default: throw false;
				}
				MPI_Irecv(&its_LC, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
			}
			acessar_RC();
			atender_pendentes(my_LC, fila);
			atender_requisicoes(my_LC, its_LC, req);
		}
	}
}

