all: 
	gcc -Wall -Werror -std=c99 -Os -o align8 align8.c
	gcc -Wall -Werror -std=c99 -Os -o align16 align16.c
	gcc -Wall -Werror -std=c99 -Os -o align32 align32.c
	gcc -Wall -Werror -std=c99 -Os -o align16 align64.c
run:
	make -C . 
	./align8
	./align16
	./align32
	./align64
