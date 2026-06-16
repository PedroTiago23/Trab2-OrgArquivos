all:
	gcc -o project_main *.c Dados/*.c Index/*.c

run:
	./project_main

clean:
	rm project_main