all: control.c write.c
	gcc -x c -o control control.c
	gcc -x c -o write write.c

clean:
	rm control
	rm write
