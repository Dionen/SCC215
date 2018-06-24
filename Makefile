all:
	gcc -o programaTrab2 main.c src/b_tree.c src/functions.c src/utils.c src/fila.c src/buffer.c -I./include -std=c99 -g
