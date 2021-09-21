#include <bits/stdc++.h>
#include <fstream>
using namespace std;

ifstream fin("poduri.in");
ofstream fout("poduri.out");

int allRows[] = {-1, 0, 1, 0};  // check directions for Double bridge
int allCols[] = {0, 1, 0, -1};

int vertRows[] = {-1, 1};  // check directions for Vertical bridge
int vertCols[] = {0, 0};

int horiRows[] = {0, 0};  // check directions for Horizontal bridge
int horiCols[] = {1, -1};

queue<int> X, Y;

struct Point {
    int x;
    int y;
};

struct node_queue {
    Point pt;
    int dist;
};
// check if the position is valid within the matrix
bool isValid(int row, int col, int N, int M) {
    return (row >= 1) && (row <= N) && (col >= 1) && (col <= M);
}
// check if we are about to get out of the maze on a Vertical bridge
bool isVertical(int x, int y, char **vect, int N) {
	return ((x == 1 || x == N) && (vect[x][y] == 'V' || vect[x][y] == 'D'));
}
// check if we are about to get out of the maze on Horizonal bridge
bool isHorizontal(int x, int y, char **vect, int M) {
	return ((y == 1 || y == M) && (vect[x][y] == 'O' || vect[x][y] == 'D'));
}
// check if we are on surface from either the Hori or Vertical bridge
bool isSurface(int x, int y, char **vect, int N, int M) {
	return (isHorizontal(x, y, vect, M) || isVertical(x, y, vect, N));
}
// algorithm of lee
int lee(int N, int M, int start_x, int start_y, char **vect) {
	Point start = {start_x, start_y};
	bool visited[N][M];
    memset(visited, false, sizeof visited);
    visited[start_x][start_y] = true;
	queue<node_queue> q;
    node_queue src = {start, 0};
    q.push(src);

	// bfs from the start point
	while (!q.empty()) {
        node_queue curr = q.front();
        Point curr_pt = curr.pt;
		// we reached the surface from the bridge maze
        if (isSurface(curr_pt.x, curr_pt.y, vect, N, M))
        	return curr.dist;

        q.pop();
		// we are on a vertical bridge
        if (vect[curr_pt.x][curr_pt.y] == 'V') {
        	for (int i = 0; i < 2; i++) {
				// check up and down
        		int next_x = curr_pt.x + vertRows[i];
            	int next_y = curr_pt.y + vertCols[i];

				// enqueue the valid cell
            	if (isValid(next_x, next_y, N, M) &&
            			vect[next_x][next_y] != '.' &&
            			!visited[next_x][next_y]) {
		            visited[next_x][next_y] = true;
		            node_queue neighbour = {{next_x, next_y}, curr.dist + 1};
		            q.push(neighbour);
            	}
        	}
		// we are on a horizontal bridge
        } else if (vect[curr_pt.x][curr_pt.y] == 'O') {
        	for (int i = 0; i < 2; i++) {
        		int next_x = curr_pt.x + horiRows[i];
            	int next_y = curr_pt.y + horiCols[i];
            	if (isValid(next_x, next_y, N, M) &&
	            		vect[next_x][next_y] != '.' &&
	            		!visited[next_x][next_y]) {
		            visited[next_x][next_y] = true;
		            node_queue neighbour = {{next_x, next_y}, curr.dist + 1};
		            q.push(neighbour);
            	}
        	}
		// we are on a double bridge
        } else if (vect[curr_pt.x][curr_pt.y] == 'D') {
        	for (int i = 0; i < 4; i++) {
        		int next_x = curr_pt.x + allRows[i];
            	int next_y = curr_pt.y + allCols[i];
            	if (isValid(next_x, next_y, N, M) &&
            			vect[next_x][next_y] != '.' &&
            			!visited[next_x][next_y]) {
		            visited[next_x][next_y] = true;
		            node_queue neighbour = {{next_x, next_y}, curr.dist + 1};
		            q.push(neighbour);
            	}
        	}
        }
	}
    return -1;  // destination cannot be reached
}
int main(int argc, char const *argv[]) {
	int N, M;
	int start_x, start_y;

	fin >> N >> M;
	fin >> start_x >> start_y;

	char **vect = (char **)malloc(N * sizeof(char*));
	for(int i = 1; i <= N; i++)
		vect[i] = (char *)malloc(M * sizeof(char));

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			fin >> vect[i][j];
		}
	}

	int dist = lee(N, M, start_x, start_y, vect) + 1;
	fout << dist << endl;

	for (int i = 1; i <= N; i++)
		free(vect[i]);
	free(vect);
	return 0;
}
