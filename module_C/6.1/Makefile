all: createbild
	gcc main.c -o main -L . list.a

createbild: func.o
	ar q list.a func.o

func.o: func.c
	gcc -c func.c -o func.o

clean:
	rm -rf *.o main
