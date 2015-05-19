#include <iostream>
#include "datatype.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Population parent;
    parent.readInitialPopulation("testdata.txt");
    parent.print();
    parent.calculateMakespan();
    parent.calculateFitness();
    for (int pitr = 0; pitr < POPULATION_ITERATION; ++pitr)
    {
        cout << "generation " << pitr << endl;
        /*
        When pitr == 0, Makespan and Fitness correct.
        Here Fitness should be EXACTLY correct.
        */
        int num_parent_produce = 10;
        parent.genChildren(num_parent_produce);
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
            /*
            After localSearch, Fitness re-calculated.
            Both makespan and fitness are correct but unordered.
            */
        }
        parent.print();
        system("pause");
        /*
        No matter what we do, Fitness SHOULD BE CORRECT HERE!!!!
        */
    }
    parent.print();
    return 0;
}
