#include <iostream>
#include "datatype.h"

using namespace std;

int main(int argc, char const *argv[])
{
    const int POPULATION_SIZE = 20; //MA演算法中親代個數
    const int POPULATION_ITERATION = 1000; //GA演算法的代數
    const int LOCAL_SEARCH_FREQUENCY = 100; //每這麼多次做一次Local Search

    
    Population parent;
    parent.readInitialPopulation("testdata.txt");
    parent.print();

    for (int pitr = 0; pitr < POPULATION_ITERATION; ++pitr)
    {
        parent.genChild();
    }

    return 0;
}
