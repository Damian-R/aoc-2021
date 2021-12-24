#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <stack>
#include <regex>
#include <tuple>
#include <climits>
#include <bitset>
#include <cassert>
#include <cmath>
#include <map>

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

bool compare_len(const std::string& a, const std::string& b)
{
    return (a.size() < b.size()); 
}