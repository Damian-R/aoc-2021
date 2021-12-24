#include "aoc.h"

using namespace std;

int main() {
  string line;
  vector<unsigned long long> scores;
  while (getline(cin, line)) {
    stack<char> s;
    
    int i = 0;
    for (auto c : line) {
      if (c == '(' || c == '{' || c == '<' || c == '[') s.push(c);
      else {
        if (c == ')') {
          if (s.top() == '(') s.pop();
          else break;
        }
        if (c == ']') {
          if (s.top() == '[') s.pop();
          else break;
        }
        if (c == '}') {
          if (s.top() == '{') s.pop();
          else break;
        }
        if (c == '>') {
          if (s.top() == '<') s.pop();
          else {
            break;
          }
        }
      }
      ++i;
    }

    if (i != line.size()) continue; // corrupted line

    unsigned long long score = 0;
    while (!s.empty()) {
      char c = s.top();
      score *= 5;
      if (c == '(') score += 1;
      else if (c == '[') score += 2;
      else if (c == '{') score += 3;
      else if (c == '<') score += 4;
      s.pop();
    }
    scores.push_back(score);
  }

  sort(scores.begin(), scores.end());
  
  cout << scores[scores.size()/2] << endl;
  return 0;
}