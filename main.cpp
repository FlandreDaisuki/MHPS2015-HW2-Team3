#include <iostream>
#include "datatype.h"
#include <time.h>
using namespace std;

int main(int argc, char const *argv[])
{
	srand(time(0));
	Population parent;
	//parent.readInitialPopulation("testdata.txt");
	parent.readPopulationBase("one_testdata.txt");
	parent.InitialPopulation();
	parent.calculateFitness();
    parent.printSolutionSimple(cout);
    cout<<endl;
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
		//std::cout<<"in";
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

		parent.printSolutionSimple(cout);
        cout << endl;
		/*
		No matter what we do, Fitness SHOULD BE CORRECT HERE!!!!
		*/
	}
	return 0;
}
