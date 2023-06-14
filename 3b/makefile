lab.out: laba9.o libfunc.a
		gcc -o lab.out laba9.o -L. -lfunc
laba9.o: laba9.c
        gcc -c laba9.c
libfunc.a: func.o
		ar r libfunc.a func.o
func.o: func.c
		gcc -c func.c
clean:
		rm -rf  *.a
		rm -rf *.o
