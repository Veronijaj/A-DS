3b.out: 3b.o libfunc.a
		gcc -o 3b.out 3b.o -L. -lfunc
3b.o: 3b.c
		gcc -c 3b.c
libfunc.a: func.o
		ar r libfunc.a func.o
func.o: func.c
		gcc -c func.c
clean:
		rm -rf  *.a
		rm -rf *.o
		rm -rf *.bin
