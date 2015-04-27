#pragma once

#include <map>
#include <cassert>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <cstdio>
#include <algorithm>

/* Nameing Guide *************/
/*                           */
/* Class name: 字首大寫      */
/* Member function: 駱駝式   */
/* Member variable: 小寫底線 */
/*                           */
/*****************************/

typedef std::vector< std::vector<int> > Matrix;

///一個無編碼的排程，一個解序列
class Schedule
{
public:
    Schedule(int j, int m): jobs(j), machines(m), fitness(0)
    {
        matrix.resize(jobs, std::vector<int> (machines, 0));
    }
    Schedule(const Schedule &s) : jobs(s.jobs), machines(s.machines), fitness(0)
    {
        matrix.resize(jobs, std::vector<int> (machines, 0));

        for (int i = 0; i < s.machines; ++i)
        {
            for (int j = 0; j < s.jobs; ++j)
            {
                matrix[j][i] = s.matrix[j][i];
            }
        }
    }
    Schedule(const Matrix &m) : jobs(m.size()), machines(m[0].size()), fitness(0)
    {
        matrix.resize(jobs, std::vector<int> (machines, 0));

        for (int i = 0; i < machines; ++i)
        {
            for (int j = 0; j < jobs; ++j)
            {
                matrix[j][i] = m[j][i];
            }
        }
    }
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
        out << "Makespan:" << makespan() << endl << endl;
    }

    //Member Functions
    int makespan() const //計算makespan 因為是計算 所以會花費時間 或許可以存到private裏面
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
private:
    const int jobs, machines;
    int fitness; // this variable is for environment selection(Baldwinian).
    Matrix matrix;
};

class JobMap
{
public:
    JobMap(const Matrix &m)
    {
        for (size_t msz = 0; msz < m.size(); ++msz)
        {
            job_to_jn[m[msz]] = msz;
            jn_to_job[msz] = m[msz];
        }
    };
    ~JobMap() {};
    const std::vector<int> &getJobs(size_t i)
    {
        return jn_to_job[i];
    }
    size_t getJobNumber(const std::vector<int> &j)
    {
        return job_to_jn[j];
    }
private:
    std::map< std::vector<int>, int  > job_to_jn;
    std::map< int, std::vector<int> > jn_to_job;
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

        jmp = new JobMap(schedules[0].getMatrix());
    };
    ~Population() {};

    //get/set Functions
    const JobMap& getJobMap() {
        return *jmp;
    }
    std::vector<Schedule>& getSchedules()
    {
        return schedules;
    }
    Schedule& getSchedule(size_t i)
    {
        assert(i < schedules.size() && i >= 0);
        return schedules[i];
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

        jmp = new JobMap(schedules[0].getMatrix());
    }
    void InitialPopulation(int population_size)
    {
        //read initial base schedule
        //generate a set of better population
    }
    void calculateFitness()
    {
        //calculate each schedule fitness
    }
    void genChildren()
    {
        //select betters {輪盤法,window,...} depend on fitness
        //elitism
        //crossover {1 point, 2 point,....}
        //mutation
        //add to this population
    }
    void envSelection()
    {
        // 已知前半是parent 後半是children
        // 2/4
        // (1,1)
        // else
    }
    void localSearch(int num_to_search)
    {
        // SA search for each
        // calculate fitness
    }
private:
    JobMap *jmp;
    std::vector<Schedule> schedules;
};

