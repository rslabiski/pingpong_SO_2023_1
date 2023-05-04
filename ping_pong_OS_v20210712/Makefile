EXECUTAVEL = ppos-main

all: metrica run

scheduler:
	gcc -o $(EXECUTAVEL) pingpong-scheduler.c ppos-core-aux.c libppos_static.a

preempcao:
	gcc -o $(EXECUTAVEL) pingpong-preempcao.c ppos-core-aux.c libppos_static.a

preempcao_stress:
	gcc -o $(EXECUTAVEL) pingpong-preempcao-stress.c ppos-core-aux.c libppos_static.a

contab-prio:
	gcc -o $(EXECUTAVEL) pingpong-contab-prio.c ppos-core-aux.c libppos_static.a

clean:
	rm -rf *.o
	rm -rf *_exec
	#rm ./bin/* ./obj/*

run:
	./$(EXECUTAVEL)