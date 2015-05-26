## 環境

- Node.js
- Excel

## 步驟
- 編譯並執行並將執行檔移到Runall資料夾
```
    $ g++ -O3 -std=c++11 *.cpp -o a.exe
```
- 確認在CLI可運行Node
```
    $ node
    > 1+2
    3
    >
``` 
- 確認完Ctrl+c離開
- 用CLI到我們的Runall資料夾
```
    $ cd (你的路徑)/MHPS2015-HW2-Team3/Runall
```
- 假設想跑第一筆測資就這樣下，標記的地方可改1~9
```
	                  *                 *
	$ a.exe Benchmark/1.txt > Analytics/1.res.txt
```
- 輸入以下指令
```
	                                      *
    $ node Analytics/analyze.js Analytics/1.res.txt -stat -csv
```
- 接著就可以開啟csv檔作資料分析了
