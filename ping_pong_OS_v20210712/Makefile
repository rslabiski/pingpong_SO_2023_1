EXECUTAVEL = ppos-test

all: scheduler run

scheduler:
	gcc -o $(EXECUTAVEL) pingpong-scheduler.c ppos-core-aux.c libppos_static.a

clean:
	rm -rf *.o
	#rm ./bin/* ./obj/*

run:
	./$(EXECUTAVEL)