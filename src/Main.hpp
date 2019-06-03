/*
 * Main.hpp
 *
 *  Created on: 4 de mai de 2018
 *      Author: cesar
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include "mpi/mpi.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <cstdlib>

void anel();
void servidor();
void lamport();

extern int numtasks, rank;//, len, rc;
const unsigned bufsize = 64;
extern char buffer[bufsize];

template<typename ...params>
inline void _printf(const char* format, params ...args) {
	snprintf(buffer, bufsize, format, args...);
	MPI_Ssend(buffer, bufsize, MPI_CHAR, numtasks, 0, MPI_COMM_WORLD);
}

template<>
inline void _printf(const char* format) {
	strncpy(buffer, format, bufsize);
	MPI_Ssend(buffer, bufsize, MPI_CHAR, numtasks, 0, MPI_COMM_WORLD);
}

inline bool quer_RC() {
	return rand()%(numtasks+1) == 0;
}

inline void acessar_RC() {
	_printf("Entra RC");
	sleep(1);
	_printf("Sai RC");
}

inline void printProcess() {
	MPI_Status stat;
	while(true) {
		MPI_Recv(buffer, bufsize, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
		printf("%*c %u: %s\n", stat.MPI_SOURCE*8,' ', stat.MPI_SOURCE, buffer);
	}
}



#endif /* MAIN_HPP_ */
