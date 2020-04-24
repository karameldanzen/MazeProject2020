// main.cpp 
#include "maze.h" 
int main() {
	ofstream bfsOut;  bfsOut.open("bfs_solution.txt");
	ifstream inputFile("input.txt");  int xDim, yDim;
	inputFile >> xDim >> yDim;  maze maze(xDim, yDim, inputFile);
	maze.bfs(bfsOut);
	system("pause");  return 0;
}