.PHONY: all

all: 2_1_1 2_1_2 2_2_1 2_2_2 2_3_1 2_3_2


2_1_1: 2_1_1.c
	gcc 2_1_1.c -pthread -o 2_1_1


2_1_2: 2_1_2.c
	gcc 2_1_2.c -pthread -o 2_1_2


2_2_1: 2_2_1.c
	gcc 2_2_1.c -pthread -o 2_2_1


2_2_2: 2_2_2.c
	gcc 2_2_2.c -pthread -o 2_2_2


2_3_1: 2_3_1.c
	gcc 2_3_1.c -pthread -o 2_3_1


2_3_2: 2_3_2.c
	gcc 2_3_2.c -pthread -o 2_3_2


.PHONY: clean

clean:
	rm 2_1_1 2_1_2 2_2_1 2_2_2 2_3_1 2_3_2
