all:
	g++ -std=c++11 -O3 *.cpp -o a.out
clean:
	rm ./a.out