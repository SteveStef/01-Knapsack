#include <vector>
#include <iostream>
#include <time.h>
#include <map>
#include <algorithm>
#include <cstdlib>
using namespace std;

/*
	to optimize the genetic algorithm:
		- Have better starting population
		- remake selection funcion, 4 children per couple
		- keep track of best mutation in population
		- if weight goes over set it = to 0
		- make struct
*/

class GeneticAlgorithm {
	private:
	int vals[8] = {0};
	int weights[8] = {0};
	int capacity, n, populationSize = 100, bestChromosome = 0;
	vector<string> population;

	string createChromosome() {
		string chromosome = "";
		int weight = 0;
		for(int i = 0; i < n; i++) chromosome += '0';
		while(true) {
			int pos = rand() % this->n;
			if(weight+this->weights[pos] > this->capacity) break;
			chromosome[pos] = '1';
			weight += this->weights[pos];
		}
		return chromosome;
	}

	void initPopulation() {
		for(int i=0;i<this->populationSize;i++) {
			string chromosome = this->createChromosome();
			this->population.push_back(chromosome);
			//cout << chromosome << "\n";
		}
	}

	int calcFitness(string chromosome) {
		int v = 0;
		for(int i = 0; i < chromosome.length(); i++) {
			if(chromosome[i] == '1') v += this->vals[i];
		}
		return v;
	}

	vector<string> selectFromPopulation() {
		vector<string> parents;
		while(parents.size() < this->populationSize / 2) {
			int pos1 = rand() % this->populationSize;
			int pos2 = rand() % this->populationSize;
			string p1 = population[pos1], p2 = population[pos2];
			int fit1 = calcFitness(p1), fit2 = calcFitness(p2);
			if(fit1 > fit2) parents.push_back(p1);
			else parents.push_back(p2);
			this->bestChromosome = max(fit1,fit2);
		}
		return parents;
	}

	// weed out weights that are over 15
	vector<string> crossover(string p1, string p2) {
		string p1Start = p1.substr(0, p1.length()/2);
		string p1End = p1.substr(p1.length()/2, p1.length());
		string p2Start = p2.substr(0, p2.length()/2);
		string p2End = p2.substr(p2.length()/2, p2.length());
		return { p1Start + p2End,  p2Start + p1End };
	}

	bool validChromosome(string chromosome) {
		int weight = 0;
		for(int i=0;i<chromosome.length();i++) {
			if(chromosome[i] == '1') weight += this->weights[i];
		}
		if(weight > this->capacity) return false;
		else return true;
	}

	void nextGeneration(vector<string> parents) {
		// before the population is full, put the best from gen 1 into gen 2,later
		this->population.clear();
		for(int i = 0; i < parents.size()-1; i+=2) {
			string p1 = parents[i], p2 = parents[i+1];
			vector<string> children = crossover(p1,p2);
			if(validChromosome(children[0])) this->population.push_back(children[0]);
			if(validChromosome(children[1])) this->population.push_back(children[1]);
			this->population.push_back(parents[i]);
			this->population.push_back(parents[i+1]);
		}
		// int generationScore = 0;
		// for(auto i : this->population) {
		// 	cout << i << " fitness: " << calcFitness(i) << "\n";
		// }
		// cout << "Generation average: " << generationScore / this->populationSize << "\n";
	}

	void mutationRate() {
		for(int i=0;i<this->population.size();i++) {
			if(((double) rand() / (RAND_MAX)) < 0.1) {
				string curr = this->population[i];
				int pos = rand() % this->n;
				if(curr[pos] == '1') curr[pos] = '0';
				else curr[pos] = '1';
				if(validChromosome(curr)) this->population[i] = curr;
			}
		}
	}

	public:
	GeneticAlgorithm(int vals[], int weights[], int cap, int n) {
			this->capacity = cap;
			this->n = n;
			for(int i=0;i<n;i++) {
			this->vals[i] = vals[i];
			this->weights[i] = weights[i];
		}
	}

	double percentOptimal(int solution) {
		double sum = 0;
		for(int i = 0; i < this->population.size(); i++) {
			if(calcFitness(this->population[i]) == solution)
				sum += 1;
		}

		return (int)((sum / population.size()) * 100);
	}

	int knapsack() {
		initPopulation();
		int generations = 1;
		while(generations != 101) {
			vector<string> parents = selectFromPopulation(); // population / 2 each call aka n
			nextGeneration(parents); // make new generation // n
			mutationRate(); // n
			//cout << "Generation" << generations << ": " << (double) percentOptimal(41) << "%\n";
			generations++;
		}
		return this->bestChromosome ;
	}
};

int main()
{
	srand(time(NULL));
	int vals[] = { 11, 8, 6, 12, 5, 8, 4, 13 };
	int weights[] = { 10, 4, 5, 2, 8, 2, 6, 7 };
	int capacity = 15, n = 8;
	
	int start = clock();
	GeneticAlgorithm solution(vals,weights,capacity,n);
	cout << solution.knapsack() << " is the optimal value for the input";
	int end = clock();

	cout << "\nexecuation: " << ((float) end - start)/CLOCKS_PER_SEC << "s";

	return 0;
}
