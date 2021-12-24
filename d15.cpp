#include "aoc.h"

using namespace std;

void add(vector<vector<int>>& fullgrid, priority_queue<tuple<int, int, int>>& q, int x, int y, int dist) {
    if (x < 0 || y < 0 || x >= fullgrid[0].size() || y >= fullgrid.size()) return;
    q.push({ dist+fullgrid[y][x], x, y });
}

void add_neighbours(vector<vector<int>>& fullgrid, priority_queue<tuple<int, int, int>>& q, int x, int y, int dist) {
    add(fullgrid, q, x-1, y, dist);
    add(fullgrid, q, x+1, y, dist);
    add(fullgrid, q, x, y-1, dist);
    add(fullgrid, q, x, y+1, dist);
}

int main() {
    string line;
    vector<vector<int>> grid;

    while (getline(cin, line)) {
        vector<int> row;
        for (auto c : line) row.push_back(c-'0');
        grid.push_back(row);
    }

    int n = grid.size();
    vector<vector<int>> fullgrid(n*5, vector<int>(n*5));
    
    // construct full grid
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    int val = grid[i][j]+r+c;
                    if (val > 9) {
                        val = (val+1) % 10;
                    }

                    fullgrid[r*n + i][c*n + j] = val;
                }
            }
        }
    }

    grid = fullgrid;
    n = grid.size();

    auto compare = [](tuple<int, int, int>& a, tuple<int, int, int>& b) {
        return get<2>(a) > get<2>(b);
    };

    typedef tuple<int, int, int> t;

    vector<vector<int>> dists(fullgrid.size(), vector<int>(fullgrid.size(), INT_MAX));
    priority_queue<t, vector<t>, greater<t>> q; // distance, x, y
    set<tuple<int, int>> visited;
    q.push({0, 0, 0});
    dists[0][0] = 0;

    vector<int> dirsX = {-1, 0, 1,  0};
    vector<int> dirsY = { 0, 1, 0, -1};

    while (!q.empty()) {
        auto [ dist, x, y ] = q.top(); q.pop();

        for (int i = 0; i < 4; ++i) {
            int nX = x + dirsX[i], nY = y + dirsY[i];
            if (nX < 0 || nY < 0 || nX >= fullgrid[0].size() || nY >= fullgrid.size()) continue;

            int alt = dists[y][x] + fullgrid[nY][nX];
            if (alt < dists[nY][nX]) {
                dists[nY][nX] = alt;
                q.push({ alt, nX, nY });
            }
        }
    }
    
    cout << dists[n-1][n-1] << endl;
}