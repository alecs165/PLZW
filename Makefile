HEADERS = decode.h encode.h
OBJECTS = program.o decode.o encode.o hashtable.o linked_l.o stack.o

default: program

program.o: main.c $(HEADERS)
	gcc -c main.c -o program.o

decode.o: decode.c decode.h
	gcc -g -c decode.c -o decode.o

encode.o: encode.c encode.h
	gcc -g -c encode.c -o encode.o

hashtable.o: hashtable.c hashtable.h
	gcc -c hashtable.c -o hashtable.o

linked_l.o: linked_l.c linked_l.h
	gcc -c linked_l.c -o linked_l.o
	
stack.o: stack.c stack.h
	gcc -c stack.c -o stack.o

program: $(OBJECTS)
	touch encode
	touch decode
	rm -f decode
	rm -f encode
	
	gcc $(OBJECTS) -o program

	ln -s program decode
	ln -s program encode

clean:
	-rm -f $(OBJECTS)
	-rm -f program
	-rm -f decode
	-rm -f encode