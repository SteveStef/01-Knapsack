
 /*
	Given weights and values of n items, put these items in a knapsack of capacity W to get the 
	maximum total value in the knapsack. In other words, given two integer arrays val[0..n-1] 
	and wt[0..n-1] which represent values and weights associated with n items respectively. 
	Also given an integer W which represents knapsack capacity, find out the maximum value 
	subset of val[] such that sum of the weights of this subset is smaller than or equal to W. 
	You cannot break an item, either pick the complete item or don’t pick it (0-1 property).
*/

#include <vector>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <map>
using namespace std;

class Solution {
	public:
	map<string, int> dp;
	int knapsack(int values[], int weights[], int capacity, int n) {
		return f(values, weights, capacity, n, 0, 0, 0);
	} 
	
	int f(int values[], int weights[], int capacity, int n, 
	int currWeight, int currVal, int idx) {

		string key = to_string(idx) + "," + to_string(currVal) + "," + to_string(currWeight);
		if(dp.find(key) != dp.end()) return dp[key];
		
		if(currWeight > capacity) return -1;
		if(idx >= n) return currVal;
		
		return dp[key] = max (
			f(values, weights, capacity, n, currWeight+weights[idx],currVal+values[idx], idx+1),
			f(values, weights, capacity, n, currWeight, currVal, idx+1)
		);
	}
};
 
int main()
{
	int values[] = { 11, 8, 6, 12, 5, 8, 4, 13 };
	int weights[] = { 10, 4, 5, 2, 8, 2, 6, 7};
	
	int capacity = 15, n = sizeof(values) / sizeof(values[0]);

	int start = clock();
	Solution solution;
	cout << solution.knapsack(values, weights, capacity, n);
	
	int end = clock();
	cout << "\nexecution: " << ((float) end - start)/CLOCKS_PER_SEC << "s";
	return 0;
}