var fs = require('fs');

var toJson = false;
var toFlattedJson = false;
var toScatterCsv = false;
var toStat = false;

process.argv.forEach(function(val){
    if(val === '-json')
        toJson = true;
    if(val === '-fjson')
        toFlattedJson = true;
    if(val === '-csv')
        toScatterCsv = true;
    if(val === '-stat')
        toStat = true;
    if(val === '-all')
        toJson = toFlattedJson = toScatterCsv = toStat = true;
});

var filename = process.argv[2].split('.')[0];

var jsonArr, fjsonArr, statArr;

fs.readFile(process.argv[2], { encoding: 'utf-8' }, function(err, data) {
    if (err) {
        console.log(err);
        return;
    }
    var gArray = data.trim().split('\n\n');

    jsonArr = gArray.map(function(str, itri) {
        var solarr = str.trim().split('\n');
        solarr = solarr.map(function(sol) {
            return [itri, parseInt(sol.slice(sol.indexOf(':')+1), 10)];
        });
        return solarr;
    });

    statArr = jsonArr.map(function(genarr){
        return genarr.map(function(arr){
            return arr[1];
        });
    });


    fjsonArr = jsonArr.reduce(function(a, b){ return a.concat(b)});
    if(toFlattedJson) {
        fs.writeFileSync(filename+'.flatted.json', JSON.stringify(fjsonArr));
    }
    if(toJson) {
        fs.writeFileSync(filename+'.json', JSON.stringify(jsonArr));
    }
    if(toScatterCsv) {
        var csvstr = '';
        fjsonArr.forEach(function(elem){
            csvstr = csvstr + elem[0] + ',' + elem[1] + '\n';
        });
        fs.writeFileSync(filename+'.csv', csvstr);
    }
    if(toStat) {
        var max = [], min = [], avg = [], dev = [];
        statArr.forEach(function(arr){
            max.push(calcMax(arr));
            min.push(calcMin(arr));
            avg.push(calcAvg(arr));
            dev.push(calcDev(arr));
        });

        var statstr='';
        for(var i = 0; i < max.length; i++) {
            statstr += [max[i], min[i], avg[i], dev[i]].join(',') + '\n';
        }
        fs.writeFileSync(filename+'.stat.csv', statstr);
    }
})

function calcMax(arr) {
    return Math.max.apply(null, arr);
}

function calcMin(arr) {
    return Math.min.apply(null, arr);
}

function calcAvg(arr) {
   var s = arr.reduce(function(a, b){return a + b;});
   var realf = s / arr.length;

   return Math.round(realf*100)/100;
}

function calcDev(arr) {
   var av = calcAvg(arr);
   var a = arr.map(function(x){return Math.pow(av-x, 2);});
   var ss = a.reduce(function(a, b){return a + b;});
   var vari = ss / arr.length;
   var realdev = Math.sqrt(vari);

   return Math.round(realdev*100)/100;
}