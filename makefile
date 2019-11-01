all: clean

run: compile
	./simula-vm 64 < teste3.txt

compile: 
	gcc -Wall -o "simula-vm" "simula-vm.c" 

clean: run
	rm simula-vm
