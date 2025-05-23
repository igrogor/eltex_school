all: liblist.so
	gcc main.c -o main -L../6.2 -llist -Wl,-rpath,/home/egor/eltex_school/module_C/6.2/

# createbild: func.o
# 	ar q list.a func.o

func: func.c header.h
	gcc -c -fPIC func.c
	gcc -shared -o liblist.so func.o

clean:
	rm -rf *.o main
	rm liblist.so
