EXECUTAVEL = ppos-main

all: preempcao run

scheduler:
	gcc -o $(EXECUTAVEL) pingpong-scheduler.c ppos-core-aux.c libppos_static.a

preempcao:
	gcc -o $(EXECUTAVEL) pingpong-preempcao.c ppos-core-aux.c libppos_static.a

preempcao_stress:
	gcc -o $(EXECUTAVEL) pingpong-preempcao-stress.c ppos-core-aux.c libppos_static.a

timer:
	gcc -o $(EXECUTAVEL) timer.c

clean:
	rm -rf *.o
	rm -rf *_exec
	#rm ./bin/* ./obj/*

run:
	./$(EXECUTAVEL)