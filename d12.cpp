#include "aoc.h"

using namespace std;

void dfs(unordered_map<string, vector<string>>& m, unordered_map<string, int> c, string node, int& res) {
  if (node == "start" && c[node] > 1) return;
  if (islower(node[0]) && c[node] > 1) {
    if (c[node] > 2) return;
    for (auto& p : c) {
      if (islower(p.first[0]) && p.second > 1 && node.compare(p.first) != 0) {
        return;
      }
    }
  }

  if (node == "end") {
    ++res;
    return;
  }
  
  for (auto x : m[node]) {
    c[x]++;
    dfs(m, c, x, res);
    c[x]--;
  }
}

int main() {
  string line;

  unordered_map<string, vector<string>> m;

  // build graph
  while (getline(cin, line)) {
    string start = line.substr(0, line.find('-'));
    string end = line.substr(line.find('-')+1, string::npos);

    m[start].push_back(end);
    m[end].push_back(start);
  }

  int res = 0;
  unordered_map<string, int> c;
  c["start"] = 1;
  dfs(m, c, "start", res);

  cout << res << endl;

  return 0;
}