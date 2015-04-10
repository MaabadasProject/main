assembler: mainProcess.o my_file.o Symbols.o filesBuilder.o
	gcc -g -pedantic -Wall mainProcess.o my_file.o Symbols.o filesBuilder.o -o assembler

mainProcess.o: mainProcess.c
	gcc -c -ansi -pedantic -Wall -std=c99 mainProcess.c -o mainProcess.o

my_file.o: my_file.c
	gcc -c -ansi -pedantic -Wall -std=c99 my_file.c -o my_file.o

Symbols.o: Symbols.c
	gcc -c -ansi -pedantic -Wall -std=c99 Symbols.c -o Symbols.o

filesBuilder.o: filesBuilder.c
	gcc -c -ansi -pedantic -Wall -std=c99 filesBuilder.c -o filesBuilder.o