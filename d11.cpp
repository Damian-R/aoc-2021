#include "aoc.h"

using namespace std;

void flash(vector<vector<int>>& map, int y, int x, int& count, vector<vector<bool>>& flashed) {
  if (y < 0 || y >= map.size()) return;
  if (x < 0 || x >= map[0].size()) return;
  ++count;
  flashed[y][x] = true;

  vector<int> dirs = {-1, 0, 1};

  for (int v : dirs) {
    for (int h : dirs) {
      if (v == 0 && h == 0) continue;

      if (y+v >= 0 && y+v < map.size() && x+h >= 0 && x+h < map[0].size()) {
        map[y+v][x+h]++;
        if (map[y+v][x+h] > 9 && !flashed[y+v][x+h]) flash(map, y+v, x+h, count, flashed);
      }
    }
  }
}

int main() {
  string line;
  vector<vector<int>> oct;
  while (getline(cin, line)) {
    vector<int> v;
    for (auto c : line) {
      v.push_back(c - '0');
    }

    oct.push_back(v);
  }

  int count = 0;
  vector<vector<bool>> flashed(oct.size(), vector<bool>(oct[0].size(), false));
  for (int i = 0; i < 223; ++i) {
    for (int y = 0; y < oct.size(); ++y) {
      for (int x = 0; x < oct[0].size(); ++x) {
        ++oct[y][x];
      }
    }

    int before = count;
    for (int y = 0; y < oct.size(); ++y) {
      for (int x = 0; x < oct[0].size(); ++x) {
        if (oct[y][x] > 9 && !flashed[y][x]) flash(oct, y, x, count, flashed);
      }
    }

    if (count - before == 10*10) {
      cout << i << endl;
      break;
    }

    for (int y = 0; y < oct.size(); ++y) {
      for (int x = 0; x < oct[0].size(); ++x) {
        flashed[y][x] = 0;
        if (oct[y][x] > 9) oct[y][x] = 0;
      }
    }

    // for (int y = 0; y < oct.size(); ++y) {
    //   for (int x = 0; x < oct[0].size(); ++x) {
    //     cout << oct[y][x];
    //   }
    //   cout << endl;
    // }
  }

  

  // cout << count << endl;
  return 0;
}