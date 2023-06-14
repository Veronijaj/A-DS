main.out: main.o libfunc.a
				gcc -o main.out main.o -L. -lfunc
main.o: main.c
				gcc -c main.c
libfunc.a: func.o
				ar r libfunc.a func.o
func.o: func.c
				gcc -c func.c
clean:
				rm -rf  *.a
				rm -rf *.o
