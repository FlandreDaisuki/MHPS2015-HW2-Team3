var fs = require('fs');

var inputfile = process.argv[2];

fs.readFile(inputfile, {encoding: 'utf-8'}, function(err, fd){
    if(err){
        throw err;
    }
    var all = fd.split('\n');
    all.length = all.length - 1;
    
    var json = [];
    all.forEach(function(val, idx){
        var d = val.split(',');
        if(json[d[0]] === undefined) {
            json[d[0]] = [];
        }
        json[d[0]].push(d[1] | 0);
    });
    
    var max = [], min = [], avg = [], dev = [];
    json.forEach(function(arr){
        max.push(calcMax(arr));
        min.push(calcMin(arr));
        avg.push(calcAvg(arr));
        dev.push(calcDev(arr));
    });

    var filestr='';
    for(var i = 0; i < max.length; i++) {
        filestr += [max[i], min[i], avg[i], dev[i]].join(',') + '\n';
    }
    fs.writeFileSync(inputfile.replace('.txt','.stat'), filestr);
});

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
