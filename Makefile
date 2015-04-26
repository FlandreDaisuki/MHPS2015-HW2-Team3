all: main
	./main.out
	make clean
test:
	g++ -std=c++11 test.cpp -o t.out
	./t.out
	rm ./t.out
main:
	g++ -std=c++11 -O3 main.cpp -o main.out
clean:
	rm ./*.out