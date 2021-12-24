#include <iostream>
#include <vector>
#include <string>

using namespace std;

int get_rating(vector<string> lines, bool co2) {
  int remaining = lines.size(), idx = 0;
  vector<bool> status(lines.size(), 1);

  while (remaining > 1) {
    int count = 0;
    for (int i = 0; i < lines.size(); ++i) {
      if (status[i]) count += lines[i][idx] == '1' ? 1 : -1;
    }

    bool discard = !(count >= 0) ^ co2;
    for (int i = 0; i < lines.size(); ++i) {
      if (status[i] && (lines[i][idx] - '0') == discard) {
        status[i] = false;
        --remaining;
      }
    }
    ++idx;
  }
  
  int i = 0;
  while (!status[i]) ++i;
  return stoi(lines[i], nullptr, 2);
}

int main() {
  vector<string> lines;
  string line;

  while (getline(cin, line)) lines.push_back(line);
  cout << get_rating(lines, false)*get_rating(lines, true) << endl;

  return 0;
}
