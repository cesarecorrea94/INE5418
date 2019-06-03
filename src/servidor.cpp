/*
 * servidor.cpp
 *
 *  Created on: 4 de mai de 2018
 *      Author: cesar
 */

#include "mpi/mpi.h"
#include <unistd.h>
#include <queue>
#include "Main.hpp"

void servidor(){
	enum { Request, Concess, Free };
	if(rank == 0) {
		std::queue<int> fila;
		bool locked = false;
		while(true) {
			MPI_Status Stat;
			MPI_Recv(nullptr, 0, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Stat);
			switch (Stat.MPI_TAG) {
			case Request:
				if(locked){
					_printf("Põe %i na fila", Stat.MPI_SOURCE);
					fila.push(Stat.MPI_SOURCE);
				} else {
					_printf("Concede permissão a %i", Stat.MPI_SOURCE);
					MPI_Send(nullptr, 0, MPI_CHAR, Stat.MPI_SOURCE, Concess, MPI_COMM_WORLD);
					locked = true;
				}
				break;
			case Free:
				if(fila.empty()) {
					_printf("Espera por requisições");
					locked = false;
				} else {
					_printf("Concede permissão a %i", fila.front());
					MPI_Send(nullptr, 0, MPI_CHAR, fila.front(), Concess, MPI_COMM_WORLD);
					fila.pop();
				}
				break;
			default: throw false;
			}
		}
	}
	else while(true) {
		sleep(1);//porção de código
		if(quer_RC()) {
			MPI_Status Stat;
			_printf("Requisita RC");
			MPI_Send(nullptr, 0, MPI_CHAR, 0, Request, MPI_COMM_WORLD);
			MPI_Recv(nullptr, 0, MPI_CHAR, 0, Concess, MPI_COMM_WORLD, &Stat);
			acessar_RC();
			_printf("Libera RC");
			MPI_Send(nullptr, 0, MPI_CHAR, 0, Free, MPI_COMM_WORLD);
		}
	}
}

