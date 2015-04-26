#pragma once

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
    Schedule(int j, int m): jobs(j), machines(m)
    {
        this->matrix.resize(machines, std::vector<int> (jobs, 0));
    }
    Schedule(const Schedule &s) : jobs(s.jobs), machines(s.machines)
    {
        this->matrix.resize(machines, std::vector<int> (jobs, 0));

        for (int i = 0; i < s.machines; ++i)
        {
            for (int j = 0; j < s.jobs; ++j)
            {
                this->matrix[i][j] = s.matrix[i][j];
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
    void print(std::ostream &out = std::cout) const
    {
        using std::endl;

        out << "Jobs:" << jobs << " Machines:" << machines << endl;
        for (int i = 0; i < machines; ++i)
        {
            for (int j = 0; j < jobs; ++j)
            {
                out.width(3);
                out << matrix[i][j];
            }
            out << endl;
        }
        out << "Makespan:" << makespan() << endl << endl;
    }

    //Member Functions
    int makespan() const //
    {
        std::vector<int> timespan(jobs + 1, 0);

        for (int i = 0; i < machines; ++i)
        {
            for (int j = 1; j <= jobs; ++j)
            {
                timespan[j] = std::max(timespan[j], timespan[j - 1]) + matrix[i][j - 1];
            }
        }
        return timespan[jobs];
    }
    void swapJobs(int job1, int job2) {
        for (int i = 0; i < machines; ++i)
        {
            std::swap(matrix[i][job1], matrix[i][job2]);
        }
    }
private:
    const int jobs, machines;
    Matrix matrix;
};

class Population
{
public:

    Population() {};
    ~Population() {};

    //get/set Functions
    std::vector<Schedule>& getSchedules()
    {
        return schedules;
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
            Schedule s(jobs_n,machines_n);
            for (int mn = 0; mn < machines_n; ++mn)
            {
                for (int jn = 0; jn < jobs_n; ++jn)
                {
                    fin >> s.getMatrix()[mn][jn];
                }
            }
            schedules.push_back(s);
        }
    }
    void InitialPopulation(int population_size)
    {
        //todo
    }
private:
    std::vector<Schedule> schedules;
};