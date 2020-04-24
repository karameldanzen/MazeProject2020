// maze.h 

#pragma once

#include <iostream> 
#include <list> 
#include <queue> 
#include <stack> 
#include <fstream> 
#include <vector>
#include <cmath> 

using namespace std;

class maze {
public:
	maze(int xDim, int yDim, ifstream &input);
	~maze();
	friend istream &operator>>(istream &input, maze &maze);
	void calcAdj();
	void dfs(ofstream&);
	void bfs(ofstream&);
	void calcPath(ofstream&);

private:
	enum status { UNDISCOVERED, DISCOVERED, EXPLORED };
	int xDim;
	int yDim;  
	int dims;  
	stack<int> dfsStack;  
	vector<int> tile;  
	vector<int> parent;  
	vector<int> position;  
	vector<int> state;  
	vector<list<int>> graph;  
	queue<int> bfsQueue;

}; 
