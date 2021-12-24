#include <iostream>
#include <algorithm>
#include <string>
#include <regex>

using namespace std;
typedef pair<int, int> point;

bool is_horiz(pair<point, point> line) {
  return line.first.second == line.second.second;
}

bool is_vert(pair<point, point> line) {
  return line.first.first == line.second.first;
}

void add_horiz(int** grid, pair<point, point> line) {
  int start = min(line.first.first, line.second.first);
  int stop = max(line.first.first, line.second.first);
  for (int i = start; i <= stop; ++i) {
    grid[line.first.second][i] += 1;
  }
}

void add_vert(int** grid, pair<point, point> line) {
  int start = min(line.first.second, line.second.second);
  int stop = max(line.first.second, line.second.second);
  for (int i = start; i <= stop; ++i) {
    grid[i][line.first.first] += 1;
  }
}

void add_diag(int** grid, pair<point, point> line) {
  point start = line.first.first < line.second.first ? line.first : line.second;
  point end = start == line.first ? line.second : line.first;
  
  int step_y = start.second < end.second ? 1 : -1;
  int y = start.second;
  for (int x = start.first; x <= end.first; ++x) {
    grid[y][x] += 1;
    y += step_y;
  }
}

int main() {
  int result = 0;
  int** grid = new int*[1000];
  for (int i = 0; i < 1000; ++i) {
    grid[i] = new int[1000];
    fill(grid[i], grid[i]+1000, 0);
  }

  regex reg("(\\d+),(\\d+) -> (\\d+),(\\d+)");
  smatch matches;
  string line;
  vector<pair<point, point>> lines; 

  while (getline(cin, line)) {
    regex_search(line, matches, reg);
    int x1 = stoi(matches[1]), y1 = stoi(matches[2]), x2 = stoi(matches[3]), y2 = stoi(matches[4]);

    lines.push_back(make_pair(make_pair(x1, y1), make_pair(x2, y2)));
  }
  
  for (auto line : lines) {
    if (is_horiz(line)) add_horiz(grid, line);
    else if (is_vert(line)) add_vert(grid, line);
    else add_diag(grid, line);
  }

  for (int i = 0; i < 1000; ++i) {
    for (int j = 0; j < 1000; ++j) {
      if (grid[i][j] >= 2) ++result;
    }
  }

  cout << result << endl;

  return 0;
}