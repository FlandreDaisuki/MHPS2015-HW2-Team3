## 環境

- Node.js
- Excel

## 步驟
- 將結果輸出到檔案並將檔案移到Result資料夾
```
    $ g++ -O3 -std=c++11 *.cpp -o a.exe && ./a.exe > (日期時間).txt 
```
- 確認在CLI可運行Node
```
    $ node
    > 1+2
    3
    >
``` 
- 確認完Ctrl+c離開
- 用CLI到我們的Result資料夾
```
    $ cd (你的路徑)/MHPS2015-HW2-Team3/Result
```
- 輸入以下指令
```
    $ node result.js (日期時間).txt -stat -csv
```
- 接著就可以開啟csv檔作資料分析了
