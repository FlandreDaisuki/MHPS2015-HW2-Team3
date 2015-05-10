#include <iostream>
#include "datatype.h"

using namespace std;

int main(int argc, char const *argv[])
{
    const int POPULATION_SIZE = 20; //MA演算法中親代個數
    const int POPULATION_ITERATION = 1000; //GA演算法的代數
    const int LOCAL_SEARCH_FREQUENCY = 100; //每這麼多次做一次Local Search
    const int LOCAL_SEARCH_CHILDREN = 5; // how many children to do Local Search after envSelection()


    Population parent;
    parent.readInitialPopulation("testdata.txt");
    parent.print();
    parent.calculateMakespan();
    parent.calculateFitness();
    for (int pitr = 0; pitr < POPULATION_ITERATION; ++pitr)
    {
        /*
        When pitr == 0, Makespan and Fitness correct.
        Here Fitness should be EXACTLY correct.
        */
        parent.genChildren();
        /*
        After genChildren, Children Makespan is not correct and Fitness not.
        */
        parent.calculateMakespan();
        /*
        After calculateMakespan, all population Makespan is corrent but Fitness NOT.
        Here Makespan should be EXACTLY correct.
        */
        parent.envSelection();
        /*
        After envSelection, Makespan keep correct but Fitness NOT.
        */
        parent.calculateFitness();
        /*
        After calculateFitness, both correct.
        Here Fitness should be EXACTLY correct.
        */
        parent.sortPopulation();
        /*
        After sortPopulation, we can get sorted-by-fitness makespan-correct population, fitness correct as well. 
        */
        if (pitr % LOCAL_SEARCH_FREQUENCY == 0)
        {
            parent.localSearch(LOCAL_SEARCH_CHILDREN);
            //local search also calculate fitness
        }
        else
        {
            parent.calculateFitness();
        }
        /*
        No matter what we do, Fitness SHOULD BE CORRECT HERE!!!!
        */
    }
    return 0;
}
