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

const int POPULATION_SIZE = 50; //MA演算法中親代個數
const int POPULATION_ITERATION = 1500; //GA演算法的代數

const int LOCAL_SEARCH_ITERATION = 10000; // iterations of local search
const int LOCAL_SEARCH_FREQUENCY = 50; //每這麼多次做一次Local Search
const int LOCAL_SEARCH_CHILDREN = 5; // how many children to do Local Search after envSelection()

const double MUTATION_RATE = 0.5;
const double TOURNAMENT_WORSE_ACCEPT_RATE = 0.3;

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
	Matrix &getMatrix() const
	{
		return const_cast<Schedule*>(this)->matrix;
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
	void setFitness(double f)
	{
		fitness = f;
	}
	double getFitness() const
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
		makespan = (this->ii());
	}
	void shuffle()
	{
		int times = 1000, a, b;

		while (times--)
		{
			a = rand() % jobs;
			b = rand() % jobs;
			this->swapJobs(a, b);
		}
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
	double fitness; // this variable is for environment selection(Baldwinian).
	int makespan;
	Matrix matrix;

	int sa() const
	{
		Schedule sa(*this);

		sa.setMakespan(sa.calcMakespan());

		const double INIT_T = 100000.0;
		const double TERM_T = 0.005;
		const double COOLDOWN = 0.9999; //cool down every iterations

		double temperature = INIT_T;

		for (int iter = 0; iter < LOCAL_SEARCH_ITERATION /* && temperature > TERM_T */; ++iter)
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
				double r = ((std::rand() << 2 + std::rand()) % 100000) / 100000.0;

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
		}

		return sa.getMakespan();
	}
	int ii() const
	{
		Schedule ii(*this);
		Schedule best_t(*this);

		ii.setMakespan(ii.calcMakespan());
		best_t.setMakespan(ii.getMakespan());

		const int reset_threshold = 50;
		int reset_th = 0;


		for (int iter = 0; iter < LOCAL_SEARCH_ITERATION; ++iter)
		{
			int xbefore = ii.getMakespan();

			int job1 = rand() % jobs, job2 = rand() % jobs;

			while (job1 == job2)
			{
				job2 = rand() % jobs;
			}

			ii.swapJobs(job1, job2);
			int xafter = ii.calcMakespan();

			if (xafter < xbefore)
			{
				//accept
				ii.setMakespan(xafter);
				reset_th = 0;

				if (xafter < best_t.getMakespan())
				{
					best_t = ii;
					best_t.setMakespan(ii.getMakespan());
				}
			}
			else
			{
				ii.swapJobs(job1, job2);
				reset_th += 1;
			}

			if (reset_th > reset_threshold)
			{
				ii.shuffle();
			}
		}

		return best_t.getMakespan();
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

	//Member Functions
	void readPopulationBase(const char* filename)
	{
		//this function is for reading data
		std::fstream fin;
		fin.open(filename);

		if (!fin.is_open())
		{
			std::cout << "Fail to open file." << std::endl;
			return;
		}

		int jobs_n, machines_n;
		fin >> jobs_n >> machines_n;
		Schedule s(jobs_n, machines_n);

		for (int mn = 0; mn < machines_n; ++mn)
		{
			for (int jn = 0; jn < jobs_n; ++jn)
			{
				fin >> s.getMatrix()[jn][mn];
			}
		}

		schedules.push_back(s);
		job_map = s.getMatrix();
	}
	void InitialPopulation()
	{
		//generate a set of better population
		//3 is magic number
		//select even
		std::vector <Schedule> parent_set;
		Schedule s(schedules[0]);
		schedules[0].setMakespan(schedules[0].calcMakespan());

		for (int i = 0; i < POPULATION_SIZE * 5; ++i)
		{
			s.shuffle();
			s.setMakespan(s.calcMakespan());
			parent_set.push_back(s);
		}

		std::sort(parent_set.begin(), parent_set.end() , [](const Schedule & a, const Schedule & b) -> bool
		{
			return a.getMakespan() < b.getMakespan();
		});

		for (int i = 1; i < POPULATION_SIZE ; i += 1)
		{
			schedules.push_back(parent_set[i * 2]);
		}
	}
	void calculateFitness()
	{
		double worst_case = 0.0;

		for (auto pitr = schedules.begin(); pitr != schedules.end(); ++pitr)
		{
			worst_case = std::max(worst_case, (double)pitr->getMakespan());
		}

		for (auto pitr = schedules.begin(); pitr != schedules.end(); ++pitr)
		{
			double new_fitness = worst_case - (double)pitr->getMakespan();

			if (new_fitness <= 0.0)
			{
				new_fitness = (rand() % 100) / 100;
			}

			pitr->setFitness(new_fitness); //makespan越小 fitness 越大
		}
	}
	void genChildren()
	{
		std::vector <Schedule> parents;
		std::vector <Schedule> children;

		elitism();
		crossover(children);
		mutation_shift(children);

		for (int i = 0; i < POPULATION_SIZE - elitism_num; ++i)
		{
			schedules.push_back(children[i]);
		}
	}
	void elitism()
	{
		this->sortParents();

		this->elitism_num = 0;
		size_t idx_to_push = 0;

		//至少保留一個精英
		schedules.push_back(schedules[idx_to_push]);
		this->elitism_num += 1;
		idx_to_push += 1;

		for (int rtimes = 1; rtimes < 4; ++rtimes)
		{
			if (rand() % 100 <= 100 - rtimes * 20)
			{
				schedules.push_back(schedules[idx_to_push]);
				this->elitism_num += 1;
				idx_to_push += 1;
			}
		}
	}
	const Schedule &twoTournament(const Schedule &P1, const Schedule &P2) const
	{
		if ((rand() % 100) / 100.0 <= TOURNAMENT_WORSE_ACCEPT_RATE)
		{
			return (P1.getFitness() < P2.getFitness()) ? P1 : P2;
		}
		else
		{
			return (P1.getFitness() > P2.getFitness()) ? P1 : P2;
		}
	}
	void crossover(std::vector <Schedule> &children)
	{
		int job;
		job = schedules[0].getJobs();
		std::vector <int> select_parents[2], create_children[2];

		for (int Set = 0; Set < 2; ++Set)
		{
			create_children[Set].resize(job);
		}

		std::vector <int> rand_select[2];

		for (int i = 0; i < POPULATION_SIZE; i += 2) //一次產生2children
		{
			for (int Set = 0; Set < 2; ++Set)  //選父母設定
			{
				select_parents[Set] = scheduleToJob(twoTournament(schedules[rand() % POPULATION_SIZE], schedules[rand() % POPULATION_SIZE]));
				create_children[Set] = select_parents[Set];
				rand_select[Set] = create_children[Set];
			}

			for (int Set = 0; Set < 2; ++Set)
			{
				while (rand_select[Set].size() > job / 4) //建立一個隨機子序列
				{
					rand_select[Set].erase(rand_select[Set].begin() + (rand() % rand_select[Set].size()));
				}
			}

			for (int Set = 0; Set < 2; ++Set)
			{
				int k = 0; //rand_start 加到第幾個

				for (int j = 0; j < job; ++j)
				{
					if (find(rand_select[(Set + 1) % 2].begin(), rand_select[(Set + 1) % 2].end(), create_children[Set][j]) == rand_select[(Set + 1) % 2].end())
					{
						continue;
					}

					create_children[Set][j] = rand_select[(Set + 1) % 2][k];
					++k;
				}
			}

			for (int Set = 0; Set < 2; ++Set)
			{
				children.push_back(jobToSchedule(create_children[Set]));
			}
		}
	}
	void mutation(std::vector <Schedule> &children)
	{
		int job = children[0].getJobs();

		for (auto itr = children.begin() + elitism_num; itr != children.end(); ++itr)
		{
			if ((rand() % 10) / 10.0 <= MUTATION_RATE)
			{
				int a = rand() % job;
				int b = rand() % job;

				while (b == a)
				{
					b = rand() % job;
				}

				int c = rand() % job;

				while (c == a || c == b)
				{
					c = rand() % job;
				}

				itr->swapJobs(a, b);
				itr->swapJobs(b, c);
			}
		}
	}
	void mutation_shift(std::vector <Schedule> &children)
	{
		int job = children[0].getJobs();

		for (auto itr = children.begin() + elitism_num; itr != children.end(); ++itr)
		{
			if ((rand() % 10) / 10.0 <= MUTATION_RATE)
			{
				int a = rand() % job;
				int b = rand() % job;

				while (b < job - 1 && a > 0) {
					itr->swapJobs(b, b + 1);
					b++;
					a--;
				}
			}
		}
	}
	void envSelection()
	{
		// 已知前半是parent 後半是children
		this->sortChildren();
		this->sortParents();
		this->env();
	}
	void env()
	{
		//The method is sort both parent and children
		//Then pick who has best makespan as new population
		std::vector <Schedule> new_generation;
		int children_idx = POPULATION_SIZE;

		for (int i = 0; i < elitism_num; ++i)
		{
			new_generation.push_back(schedules[i]);
		}

		for (int i = 0; i < POPULATION_SIZE - elitism_num; ++i)
		{
			new_generation.push_back(schedules[children_idx + i]);
		}

		schedules.clear();
		schedules = new_generation;
	}
	void localSearch()
	{
		std::vector <int> temp_makespan;

		for (auto itr = schedules.rbegin(); itr != schedules.rbegin() + LOCAL_SEARCH_CHILDREN; ++itr)
		{
			temp_makespan.push_back(itr->getMakespan());
			itr->localSearch();
		}

		this->calculateFitness();

		int i = 0;

		for (auto itr = schedules.rbegin(); itr != schedules.rbegin() + LOCAL_SEARCH_CHILDREN; ++itr)
		{
			itr->setMakespan(temp_makespan[i]);
			++i;
		}
	}
	void sortParents()
	{
		std::sort(schedules.begin(), schedules.begin() + POPULATION_SIZE, [](const Schedule & a, const Schedule & b) -> bool
		{
			return a.getFitness() > b.getFitness();
		});
	}
	void sortChildren()
	{
		std::sort(schedules.begin() + POPULATION_SIZE, schedules.end(), [](const Schedule & a, const Schedule & b) -> bool
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
	std::vector<int> scheduleToJob(const Schedule &s) const
	{
		// Schedule encode to job vector
		std::vector<int> ret;

		for (auto itr = s.getMatrix().begin(); itr != s.getMatrix().end(); ++itr)
		{
			for (size_t i = 0; i < job_map.size(); i++) {
				if (*itr == job_map[i])
				{
					ret.push_back(i);
					break;
				}
			}
		}

		return ret;
	}
	Schedule jobToSchedule(const std::vector<int> &jobv) const
	{
		// job vector decode to Schedule
		Matrix m;

		for (size_t i = 0; i < jobv.size(); ++i)
		{
			m.push_back(job_map[jobv[i]]);
		}

		Schedule s(m);
		return s;
	}

	//I/O function
	void print(std::ostream &out = std::cout) const
	{
		using std::endl;

		for (auto itr = schedules.begin(); itr != schedules.end(); ++itr)
		{
			itr->print(out);
		}
	}
	void printSolutionSimple(std::ostream &out = std::cout) const
	{
		int sum_of_makespan = 0;

		for (auto sitr = schedules.begin(); sitr != schedules.end(); ++sitr)
		{
			std::vector<int> jobseq = scheduleToJob(*sitr);

			for (auto jsitr = jobseq.begin(); jsitr != jobseq.end(); ++jsitr)
			{
				out << *jsitr;

				if (jsitr != jobseq.end() - 1)
				{
					out << ",";
				}
			}

			out << " :" << sitr->getMakespan() << std::endl;
		}
	}
private:
	std::vector<Schedule> schedules;
	Matrix job_map;
	int elitism_num = 0;
};

