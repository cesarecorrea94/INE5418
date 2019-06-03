NP=4

INE5418:
	mpic++ src/*.cpp -o INE5418

run: INE5418
	mpirun -n $$(($(NP)+1)) INE5418 $(caseArg)

clean:
	rm INE5418 || true

anel:
	make run caseArg=caseAnel
	
servidor:
	make run caseArg=caseServidor
	
lamport:
	make run caseArg=caseLamport	
