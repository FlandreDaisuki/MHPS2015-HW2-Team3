#pragma once

#include <cmath>
#include <ctime>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <map>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <limits>

/* Nameing Guide *************/
/*                           */
/* Class name: 字首大寫      */
/* Member function: 駱駝式   */
/* Member variable: 小寫底線 */
/*                           */
/*****************************/

typedef std::vector< std::vector<int> > Matrix;

const int LOCAL_SEARCH_ITERATION = 1000; // iterations of local search
const int POPULATION_SIZE = 20; //MA演算法中親代個數
const int POPULATION_CHILDREN_SIZE = 20; //MA演算法中子代個數
const int POPULATION_ITERATION = 1000; //GA演算法的代數
const int LOCAL_SEARCH_FREQUENCY = 100; //每這麼多次做一次Local Search
const int LOCAL_SEARCH_CHILDREN = 5; // how many children to do Local Search after envSelection()

///一個無編碼的排程，一個解序列
class Schedule
{
public:
    Schedule(int j, int m): jobs(j), machines(m), fitness(0), makespan(0)
    {
        matrix.resize(jobs, std::vector<int> (machines, 0));
    }
    Schedule(const Schedule &s) : jobs(s.jobs), machines(s.machines), fitness(s.fitness), makespan(s.makespan), matrix(s.matrix)
    {}
    Schedule(const Matrix &m) : jobs(m.size()), machines(m[0].size()), fitness(0), makespan(0), matrix(m)
    {}
    ~Schedule() {}

    //get/set Functions
    Matrix &getMatrix()
    {
        return matrix;
    }
    int getJobs() const
    {
        return jobs;
    }
    int getMachines() const
    {
        return machines;
    }
    void setMakespan(int s)
    {
        makespan = s;
    }
    int getMakespan() const
    {
        assert(makespan >= 0);
        return makespan;
    }
    void setFitness(int f)
    {
        fitness = f;
    }
    int getFitness() const
    {
        return fitness;
    }
    void print(std::ostream &out = std::cout) const
    {
        using std::endl;

        out << "Jobs:" << jobs << " Machines:" << machines << endl;
        for (int i = 0; i < machines; ++i)
        {
            for (int j = 0; j < jobs; ++j)
            {
                out.width(3);
                out << matrix[j][i];
            }
            out << endl;
        }
        out << "Makespan:" << makespan << " Fitness:" << fitness << endl << endl;
    }

    //Member Functions
    int calcMakespan() const
    {
        std::vector<int> timespan(jobs + 1, 0);

        for (int i = 0; i < machines; ++i)
        {
            for (int j = 1; j <= jobs; ++j)
            {
                timespan[j] = std::max(timespan[j], timespan[j - 1]) + matrix[j - 1][i];
            }
        }
        return timespan[jobs];
    }
    void swapJobs(int job1, int job2)
    {
        std::swap(matrix[job1], matrix[job2]);
    }
    void localSearch()
    {
        makespan = (this->simulated_annealing());
    }
    Schedule& operator=(const Schedule& rhs)
    {
        const_cast<int&>(jobs) = rhs.jobs;
        const_cast<int&>(machines) = rhs.machines;
        fitness = rhs.fitness;
        makespan = rhs.makespan;
        matrix = rhs.matrix;

        return *this;
    }
private:
    const int jobs, machines;
    int fitness; // this variable is for environment selection(Baldwinian).
    int makespan;
    Matrix matrix;

    int simulated_annealing()
    {
        Schedule sa(*this);

        sa.setMakespan(sa.calcMakespan());

        const double INIT_T = 10000.0;
        const double TERM_T = 0.005;
        const double COOLDOWN = 0.99; //cool down every iterations

        double temperature = INIT_T;
        for (int iter = 0; iter < POPULATION_ITERATION  && temperature > TERM_T ; ++iter)
        {
            int xbefore = sa.getMakespan();

            int job1 = rand() % jobs, job2 = rand() % jobs;
            while (job1 == job2)
            {
                job2 = rand() % jobs;
            }

            sa.swapJobs(job1, job2);
            int xafter = sa.calcMakespan();

            if (xafter < xbefore)
            {
                //accept
                sa.setMakespan(xafter);
            }
            else
            {
                double r = (std::rand() % 10000) / 10000.0;
                //printf("E:%f r:%f\n", std::exp((xafter - xbefore) / temperature) , r);

                if (std::exp((xbefore - xafter) / temperature) > r)
                {
                    //accept
                    sa.setMakespan(xafter);
                }
                else
                {
                    //not accept
                    sa.swapJobs(job1, job2);
                }
            }

            temperature *= COOLDOWN;

            //sa.print();
            //printf("temperature:%f\n", temperature);
            //fgetc(stdin);
        }
        return sa.getMakespan();
    }
};

class Population
{
public:
    Population() {};
    Population(const Population &p)
    {
        for (auto pitr = p.schedules.begin(); pitr != p.schedules.end(); ++pitr)
        {
            schedules.push_back(*pitr);
        }
        job_map = schedules[0].getMatrix();
    };
    ~Population() {};

    //get/set Functions
    std::vector<Schedule>& getSchedules()
    {
        return schedules;
    }
    Schedule& getSchedule(size_t i)
    {
        assert(i < schedules.size() && i >= 0);
        return schedules[i];
    }
    void calculateMakespan()
    {
        for (auto itr = schedules.begin(); itr != schedules.end(); ++itr)
        {
            itr->setMakespan(itr->calcMakespan());
        }

    }
    void print(std::ostream &out = std::cout) const
    {
        using std::endl;

        for (auto itr = schedules.begin(); itr != schedules.end(); ++itr)
        {
            itr->print(out);
        }
    }

    //Member Functions
    void readInitialPopulation(const char* filename)
    {
        //this function is for testing data
        std::fstream fin;
        fin.open(filename);

        if (!fin.is_open())
        {
            std::cout << "Fail to open file." << std::endl;
            return;
        }

        int jobs_n, machines_n, schedules_n;
        fin >> jobs_n >> machines_n >> schedules_n;

        for (int sn = 0; sn < schedules_n; ++sn)
        {
            Schedule s(jobs_n, machines_n);
            for (int mn = 0; mn < machines_n; ++mn)
            {
                for (int jn = 0; jn < jobs_n; ++jn)
                {
                    fin >> s.getMatrix()[jn][mn];
                }
            }
            schedules.push_back(s);
        }

        job_map = schedules[0].getMatrix();
    }
    void InitialPopulation(int population_size)
    {
        //read initial base schedule
        //generate a set of better population

        job_map = schedules[0].getMatrix();
    }

    void calculateFitness()
    {
        int worst_case = 0;
        for (auto pitr = schedules.begin(); pitr != schedules.end(); ++pitr)
        {
            worst_case = std::max(worst_case, pitr->getMakespan());
        }
        for (auto pitr = schedules.begin(); pitr != schedules.end(); ++pitr)
        {
            pitr->setFitness(worst_case - pitr->getMakespan());       //makespan越小 fitness 越大
        }

    }
    void genChildren(int parent_produce_num)
    {
        std::vector <Schedule> parents;
        std::vector <Schedule> children;
        int elitism_num;
        //select betters {輪盤法,window,...} depend on fitness
        select_parent(parents, parent_produce_num);
        elitism_num = elitism(parents, children);
        crossover(parents, children);
        mutation(children);
        int i = 0;
        for (auto itr = children.begin(); i < POPULATION_SIZE - elitism_num; ++itr)
        {
            schedules.push_back(*itr);
            ++i;
        }
    }
    int elitism(std::vector <Schedule> parents, std::vector <Schedule> children)
    {
        std::sort(parents.begin(), parents.begin() + POPULATION_SIZE , [](const Schedule & a, const Schedule & b) -> bool
        {
            return a.getFitness() > b.getFitness();
        });
        int elitism_num = rand() % 3;
        for (int i = 0; i < elitism_num; ++i)
        {
            schedules.push_back(parents[i]);
        }
        return elitism_num;
    }
    void crossover(std::vector <Schedule> parents, std::vector <Schedule> children)
    {

    }
    void mutation( std::vector <Schedule> &children)
    {

    }
    void select_parent(std::vector <Schedule> &parent, int parent_produce_num)
    {
        int group_start_index[POPULATION_SIZE], total_fitness, index;
        double rand_num;
        group_start_index[0] = 0;
        total_fitness = schedules[0].getFitness();
        for (int i = 1; i < POPULATION_SIZE; ++i)
        {
            group_start_index[i] = group_start_index[i - 1] + schedules[i - 1].getFitness();
            total_fitness += schedules[i].getFitness();
        }
        rand_num = (double)total_fitness * 0.5 / parent_produce_num;
        index = 0;
        for (int i = 0; i < parent_produce_num; ++i)
        {
            for (; index < POPULATION_SIZE && (double)group_start_index[index] <= rand_num; ++index);
            parent.push_back(schedules[index - 1]);
            rand_num += (double)total_fitness / parent_produce_num;
        }
    }
    void envSelection()
    {
        // 已知前半是parent 後半是children
        this->env2_4();
    }
    void env2_4()
    {
        //The method is sort both parent and children
        //Then pick who has best makespan as new population
        std::sort(schedules.begin(), schedules.end(), [](const Schedule & a, const Schedule & b) -> bool
        {
            return a.getMakespan() < b.getMakespan();
        });

        schedules.erase(schedules.begin() + POPULATION_SIZE, schedules.end());
    }
    void localSearch(int num_to_search)
    {
        std::vector <int> temp_makespan(num_to_search);
        int i = 0;
        for (auto itr = schedules.begin(); itr != schedules.begin() + num_to_search; ++itr)
        {
            temp_makespan[i] = itr->getMakespan();
            ++i;
            itr->localSearch();
        }
        this->calculateFitness();
        i = 0;
        for (auto itr = schedules.begin(); itr != schedules.begin() + num_to_search; ++itr)
        {
            itr->setMakespan(temp_makespan[i]);
            ++i;
        }
    }
    void sortParents(int num_of_parent)
    {
        std::sort(schedules.begin(), schedules.begin() + num_of_parent, [](const Schedule & a, const Schedule & b) -> bool
        {
            return a.getFitness() > b.getFitness();
        });
    }
    void sortChildren(int num_of_parent)
    {
        std::sort(schedules.begin() + num_of_parent, schedules.end(), [](const Schedule & a, const Schedule & b) -> bool
        {
            return a.getFitness() > b.getFitness();
        });
    }
    void sortPopulation()
    {
        std::sort(schedules.begin(), schedules.end(), [](const Schedule & a, const Schedule & b) -> bool
        {
            return a.getFitness() > b.getFitness();
        });
    }
    std::vector<int> scheduleToJob(const Schedule &s)
    {
        // Schedule encode to job vector
    }
    Schedule jobToSchedule(const std::vector<int> &jobv)
    {
        // job vector decode to Schedule
    }
private:
    std::vector<Schedule> schedules;
    Matrix job_map;
};

