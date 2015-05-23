var fs = require('fs');
var populationSize = 20;
var generationTimes = 1000;
var toJson = false;
var toCsv = false;
process.argv.forEach(function(val){
    if(val === '-json')
       toJson = true;
    if(val === '-csv')
       toCsv = true;
});

fs.readFile(process.argv[2], { encoding: 'utf-8' }, function(err, data) {
    if (err) {
        throw err;
    }
    var parse_ge_regex = new RegExp('((?:[\\d, :]+\\s){'+populationSize+'})','g');
    iterarr = data.match(parse_ge_regex);
    var objarr = iterarr.map(function(str,itri) {

        var solarr = str.split('\n');
        solarr.length = populationSize;
        solarr = solarr.map(function(sol) {
            return [itri, parseInt(sol.slice(sol.indexOf(':')+1), 10)];
        });
        //console.log(solarr);
        return solarr;
    });
    objarr = objarr.reduce(function(a, b){ return a.concat(b)});
    if(toJson) {
        fs.writeFileSync(process.argv[2]+'.json', JSON.stringify(objarr));
    }
    if(toCsv) {
        var csvstr = '';
        objarr.forEach(function(elem){
            csvstr = csvstr + elem[0] + ',' + elem[1] + '\n';
        });
        fs.writeFileSync(process.argv[2]+'.csv', csvstr);
    }
})
