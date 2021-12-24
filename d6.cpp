#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

template <typename Out>
void split(const string &s, char delim, Out result) {
    istringstream iss(s);
    string item;
    while (getline(iss, item, delim)) {
        *result++ = stoi(item);
    }
}

vector<int> split(const string &s, char delim) {
    vector<int> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

int contrib(int days, int fish, unsigned long long* memo) {
  if (days < 0) return 1;
  int n = 0;

  if (memo[days] != 0) {
    return memo[days];
  } else {
    while (days > 0) {
      days -= fish;
      n += contrib(days-2, 7, memo);
      fish = 7;
    }
  }

  memo[days] = n;

  return n;
}

int main() {
  int days = 256;
  string line;
  getline(cin, line);
  vector<int> fish = split(line, ',');
  unsigned long long* dp = new unsigned long long[days];
  // dp[i] = how many fish a fish that hits 0 on day i is responsible for creating
  fill(dp, dp+days, 0);

  for (int i = 0; i < days; ++i) {
    if (i < 7) dp[i] = 1;
    else {
      if (i >= 9) dp[i] = dp[i-9];
      dp[i] += dp[i-7]+1;
    }
  }

  for (int i = 0; i < days; ++i) {
    cout << dp[i] << " ";
  }
  cout << endl;

  int result = 0;
  for (auto f : fish)
    result += dp[days-f-1] + 1;

  cout << result << endl;
  return 0;
}