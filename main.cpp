#include <iostream>
#include "datatype.h"

using namespace std;

int main(int argc, char const *argv[])
{
    const int POPULATION_SIZE = 20; //MA演算法中親代個數
    const int POPULATION_ITERATION = 1000; //GA演算法的代數
    const int LOCAL_SEARCH_FREQUENCY = 100; //每這麼多次做一次Local Search
    const int LOCAL_SEARCH_CHILDREN = 5; // how many children to do Local Search after envSelection()
    const int LOCAL_SEARCH_ITERATION = 1000; // iterations of local search


    Population parent;
    parent.readInitialPopulation("testdata.txt");
    parent.print();

    parent.calculateFitness();
    for (int pitr = 0; pitr < POPULATION_ITERATION; ++pitr)
    {
        parent.genChildren();
        parent.envSelection();
        if (pitr % LOCAL_SEARCH_FREQUENCY == 0)
        {
            parent.localSearch(LOCAL_SEARCH_CHILDREN);
            //local search also calculate fitness
        }
        else
        {
            parent.calculateFitness();
        }

    }

    return 0;
}
