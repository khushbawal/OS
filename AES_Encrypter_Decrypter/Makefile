all: encrypt decrypt fieldTest aesTest

fieldTest: fieldTest.o field.o
	gcc -Wall -std=c99 -g fieldTest.o field.o -o fieldTest

fieldTest.o: fieldTest.c field.h
	gcc -Wall -std=c99 -g -c fieldTest.c -o fieldTest.o

aesTest: aesTest.o aes.o field.o
	gcc -Wall -std=c99 -g aesTest.o aes.o field.o -o aesTest

aesTest.o: aesTest.c aes.h field.h
	gcc -Wall -std=c99 -g -c aesTest.c -o aesTest.o

encrypt: encrypt.o aes.o field.o io.o
	gcc -Wall -std=c99 -g encrypt.o aes.o field.o io.o -o encrypt

decrypt: decrypt.o aes.o field.o io.o 
	gcc -Wall -std=c99 -g decrypt.o aes.o field.o io.o -o decrypt

encrypt.o: encrypt.c aes.h field.h io.h
	gcc -Wall -std=c99 -g -c encrypt.c -o encrypt.o

decrypt.o: decrypt.c aes.h field.h io.h
	gcc -Wall -std=c99 -g -c decrypt.c -o decrypt.o

aes.o: aes.c aes.h field.h
	gcc -Wall -std=c99 -g -c aes.c -o aes.o

io.o: io.c io.h field.h
	gcc -Wall -std=c99 -g -c io.c -o io.o

field.o: field.c field.h
	gcc -Wall -std=c99 -g -c field.c -o field.o

clean:
	rm -f *.o
	rm -f encrypt
	rm -f decrypt