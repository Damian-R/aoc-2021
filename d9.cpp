#include "aoc.h"

using namespace std;

bool is_low_point(vector<vector<int>>& map, int row, int col) {
  bool low = true;
  if (row != 0 && map[row-1][col] <= map[row][col]) low = false;
  if (row != map.size()-1 && map[row+1][col] <= map[row][col]) low = false;
  if (col != 0 && map[row][col-1] <= map[row][col]) low = false;
  if (col != map[0].size()-1 && map[row][col+1] <= map[row][col]) low = false;

  return low;
}

int get_basin_size(vector<vector<int>>& map, vector<vector<int>>& state, int i, int j) {
  int res = 0;
  queue<pair<int, int>> q;
  
  q.push(make_pair(i, j));

  while (!q.empty()) {
    int row = q.front().first;
    int col = q.front().second;
    q.pop();

    if (row < 0 || row >= map.size() || col < 0 || col >= map[0].size() || map[row][col] == 9 || state[row][col] == 1) continue;
    res++;
    state[row][col] = 1;
    q.push(make_pair(row+1, col));
    q.push(make_pair(row-1, col));
    q.push(make_pair(row, col+1));
    q.push(make_pair(row, col-1));
  }
  return res;
}

int main() {
  string line;
  vector<vector<int>> map;
  while (getline(cin, line)) {
    vector<int> l;
    for (auto c : line) {
      l.push_back(c - '0');
    }
    map.push_back(l);
  }

  vector<vector<int>> state(map.size(), vector<int>(map[0].size(), 0));

  priority_queue<int> pq;

  int res = 0;
  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[0].size(); ++j) {
      if (is_low_point(map, i, j)) {
        pq.push(get_basin_size(map, state, i, j));
      }
    }
  }
  cout << pq.top() << " ";
  pq.pop();
  cout << pq.top() << " ";
  pq.pop();
  cout << pq.top() << " ";
  pq.pop();
  return 0;
}