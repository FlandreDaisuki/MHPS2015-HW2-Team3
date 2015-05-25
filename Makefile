BF = Runall/Benchmark/
RES = Runall/
ANA = Runall/Analytics/

all: main mvtorunall runall ana clean

main:
	g++ -std=c++11 -O3 main.cpp -o main.out

mvtorunall: main
	cp main.out Runall

runall: j20 j50 j100

j20: mvtorunall
	$(RES)main.out $(BF)1.txt > $(RES)1.res.txt
	$(RES)main.out $(BF)2.txt > $(RES)2.res.txt
	$(RES)main.out $(BF)3.txt > $(RES)3.res.txt

j50: mvtorunall
	$(RES)main.out $(BF)4.txt > $(RES)4.res.txt
	$(RES)main.out $(BF)5.txt > $(RES)5.res.txt
	$(RES)main.out $(BF)6.txt > $(RES)6.res.txt

j100: mvtorunall
	$(RES)main.out $(BF)7.txt > $(RES)7.res.txt
	$(RES)main.out $(BF)8.txt > $(RES)8.res.txt
	$(RES)main.out $(BF)9.txt > $(RES)9.res.txt

mvRestoAnalytics:
	mv $(RES)*.res.txt $(ANA)

ana: mvRestoAnalytics
	node $(ANA)analyze.js $(ANA)1.res.txt -stat -csv
	node $(ANA)analyze.js $(ANA)2.res.txt -stat -csv
	node $(ANA)analyze.js $(ANA)3.res.txt -stat -csv
	node $(ANA)analyze.js $(ANA)4.res.txt -stat -csv
	node $(ANA)analyze.js $(ANA)5.res.txt -stat -csv
	node $(ANA)analyze.js $(ANA)6.res.txt -stat -csv
	node $(ANA)analyze.js $(ANA)7.res.txt -stat -csv
	node $(ANA)analyze.js $(ANA)8.res.txt -stat -csv
	node $(ANA)analyze.js $(ANA)9.res.txt -stat -csv

clean:
	rm ./*.out
	rm $(RES)*.out