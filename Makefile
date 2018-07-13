Curve.pdf: main
	./main 3

main: main.c
	gcc main.c -o main `pkg-config --cflags --libs cairo`

