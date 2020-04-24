// maze.cpp 
#include "maze.h"
#include <iterator> 

maze::maze(int xDim, int yDim, ifstream &input) {
	this->xDim = xDim;
	this->yDim = yDim;
	dims = xDim * yDim;
	graph.resize(2 * dims);
	cout << "maze: " << endl;  
	for (int y = 0; y < yDim; y++) {
		for (int x = 0; x < xDim; x++) {
			input >> *this;
			if (tile[y*yDim + x] < 0) cout << " " << tile[y*xDim + x];
		else cout << "  " << tile[y*xDim + x];
		}
		cout << endl;
	}

	for (int i = dims; i < 2 * dims; i++) tile.push_back(tile[i - dims]); 
	cout << endl << endl; 
	calcAdj();
} 

istream &operator>>(istream &input, maze &maze) {
	int temp;
	input >> temp;
	maze.tile.push_back(temp);
	return input;
}

void maze::calcAdj() {  // adjacency list for horizontal/vertical tiles
	for (int i = 0; i < dims; i++) {
		int jMag = abs(tile[i]);   int xPos = i % xDim;
		if (tile[i] > 0) {
			if (xPos + jMag <= xDim - 1)			  // right boundary check 
				graph[i].push_back(jMag + i);
			if (jMag * xDim + i <= dims - 1)		  // bottom boundary check
				graph[i].push_back(jMag * xDim + i);
			if (xPos - jMag >= 0)					  // left boundary check
				graph[i].push_back(i - jMag);
			if (i - (jMag * xDim) >= 0) {			  // top boundary check
				graph[i].push_back(i - (jMag * xDim));
			}
			// switching to diagonal tiles (second half of graph)
			if (tile[i] < 0) {
				if ((xPos + jMag <= xDim - 1) && (jMag * xDim + i <= dims - 1))  // bottom right boundary check
					graph[i].push_back(i + (jMag* xDim) + jMag + dims);
				if ((xPos - jMag >= 0) && (jMag* xDim + i <= dims - 1))		     // bottom left boundary check
					graph[i].push_back(i + (jMag* xDim) - jMag + dims);
				if (((xPos + jMag <= xDim - 1)) && (i - (jMag* xDim) >= 0))      // top right boundary check
					graph[i].push_back(i - (jMag* xDim) + jMag + dims);
				if (((xPos - jMag >= 0)) && (i - (jMag* xDim) >= 0))		     // bottom left boundary check
					graph[i].push_back(i - (jMag* xDim) - jMag + dims);
			}
		}
		// adjacency list for diagonal jumps  
		for (int i = dims; i < 2 * dims; i++) {
			int jMag = abs(tile[i]);
			int xPos = i % xDim;
			if (tile[i] > 0) {
				if ((xPos + jMag <= xDim - 1) && (jMag * xDim + i <= 2 * dims - 1))
					graph[i].push_back(i + (jMag * xDim) + jMag);
				if ((xPos - jMag >= 0) && (jMag * xDim + i <= 2 * dims - 1))
					graph[i].push_back(i + (jMag * xDim) - jMag);
				if (((xPos + jMag <= xDim - 1)) && (i - (jMag * xDim) >= dims))
					graph[i].push_back(i - (jMag * xDim) + jMag);
				if (((xPos - jMag >= 0)) && (i - (jMag * xDim) >= dims))
					graph[i].push_back(i - (jMag * xDim) - jMag);
			}
			// switch to horizontal/vertical jumps (first half of graph) 
			if (tile[i] < 0) {
				if (xPos + jMag <= xDim - 1)
					graph[i].push_back(jMag + i - dims);
				if (jMag* xDim + i <= 2 * dims - 1)
					graph[i].push_back(jMag* xDim + i - dims);
				if (xPos - jMag >= 0)
					graph[i].push_back(i - jMag - dims);
				if (i - (jMag* xDim) >= dims)
					graph[i].push_back(i - (jMag * xDim) - dims);
			}
		}
	}
}
void maze::bfs(ofstream &out) {
	state.resize(2 * dims);  parent.resize(2 * dims);  position.resize(2 * dims);
	// set to default values
	for (int i = 0; i < 2 * dims; i++) {
		state[i] = maze::UNDISCOVERED;
		parent[i] = -1;
		position[i] = -1;
	} 
	// set to initial conditions 
	state[0] = maze::DISCOVERED;
	parent[0] = NULL;
	position[0] = 0;
	bfsQueue.push(0); 
	// run BFS algorithm
	while (bfsQueue.size() != 0) {  
		int u = bfsQueue.front();   
		bfsQueue.pop(); 
		for (list<int>::iterator i = graph[u].begin(); i != graph[u].end(); i++) {
			if (state[*i] == maze::UNDISCOVERED) {
				state[*i] = maze::DISCOVERED;
				position[*i] = position[u] + 1;
				parent[*i] = u;
				bfsQueue.push(*i);
			}
		}
		state[u] = maze::EXPLORED;
	} 
	calcPath(out);
}

void maze::calcPath(ofstream &out) {
	// holds the path, but reversed
	vector <int> path; 
	// checks position of bottom right tile in vector to determine whether it is closer in the diagonal half or horizontal half of the vector
	if ((position[dims - 1] <= position[2 * dims - 1] && position[dims - 1] != -1) || position[2 * dims - 1] == -1) {
		path.push_back(dims - 1);
	}
	else path.push_back(2 * dims - 1); 
	while (path[path.size() - 1] != 0) {
		path.push_back(parent[path[path.size() - 1]]);
	}
	for (int i = 0; i < path.size(); i++) {
		path[i] = path[i] % (dims);
	}
	reverse(path.begin(), path.end());
	cout << "(x, y)" << endl;
	for (int i = 0; i < path.size(); i++) {
		if (path[i] / xDim + 1 <= 8) {
			cout << "(" << path[i] % xDim + 1 << ", " << path[i] / xDim + 1 << ") ";
			cout << path[i] << endl;    
			out << "(" << path[i] % xDim + 1 << ", " << path[i] / xDim + 1 << ") ";
		}
		else {
			cout << "(" << path[i] % xDim + 1 << ", " << path[i] / (2 * xDim) + 1 << ") ";
			cout << path[i] << endl;    out << "(" << path[i] % xDim + 1 << ", " << path[i] / (2 * xDim) + 1 << ") ";
		}
	}
	out << endl << endl;
}