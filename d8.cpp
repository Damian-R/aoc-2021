#include "aoc.h"

int get_value(unordered_map<char, char> mapping, string s) {
  if (s.size() == 2) return 1;
  if (s.size() == 4) return 4;
  if (s.size() == 3) return 7;
  if (s.size() == 7) return 8;

  if (s.size() == 5) {
    if (s.find(mapping['f']) == string::npos) return 2;
    if (s.find(mapping['c']) == string::npos) return 5;
    return 3;
  }

  if (s.size() == 6) {
    if (s.find(mapping['d']) == string::npos) return 0;
    if (s.find(mapping['e']) == string::npos) return 9;
    return 6;
  }

  return -1; // error
}

int main() {
  string line;
  unordered_map<char, char> mapping;
  int res = 0;
  string str = "abcdefg";
  while (getline(cin, line)) {
    string signal = line.substr(0, line.find('|'));
    cout << signal << endl;
    stringstream ss1(signal);
    vector<string> v;
    string s;
    for (int i = 0; i < 10; ++i) {
      ss1 >> s;
      v.push_back(s);
    }
    
    sort(v.begin(), v.end(), compare_len);
    
    for (char c : str) {
      int num = 0;
      for (auto x : v) {
        if (x.find(c) != string::npos) ++num;
      }

      if (num == 9) mapping['f'] = c;
      else if (num == 4) mapping['e'] = c;
      else if (num == 6) mapping['b'] = c;
      else if (num == 7) { // d or g
        if (v[2].find(c) == string::npos) mapping['g'] = c;
        else mapping['d'] = c;
      } else if (num == 8) { // a or c
        if (v[2].find(c) == string::npos) mapping['a'] = c;
        else mapping['c'] = c;
      }
    }

    int result = 0;
    string output = line.substr(line.find('|')+1, string::npos);
    stringstream ss(output);
    for (int i = 0; i < 4; ++i) {
      result *= 10;
      string s;
      ss >> s;
      result += get_value(mapping, s);
    }
    res += result;
  }

  cout << res << endl;

  return 0;
}