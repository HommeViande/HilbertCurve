main: main.c
	gcc main.c -o main `pkg-config --cflags --libs cairo`

Curve.pdf: main
	./main 3

