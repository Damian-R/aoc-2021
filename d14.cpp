#include "aoc.h"

using namespace std;

int main() {
    string line;
    unordered_map<string, unsigned long long> counts;
    unordered_map<string, char> rules;
    unordered_map<char, unsigned long long> res;
    int steps = 40;
    getline(cin, line);
    for (int i = 1; i < line.size(); ++i) {
        counts[line.substr(i-1, 2)]++;
        res[line[i]]++;
    }
    res[line[0]]++;

    getline(cin, line); // empty

    while (getline(cin, line)) {
        rules[line.substr(0, 2)] = line.substr(6, 1)[0];
    }

    for (int i = 0; i < steps; ++i) {
        unordered_map<string, unsigned long long> starting_counts = counts;
        for (auto x : counts) {
            if (rules.find(x.first) != rules.end()) {
                string s1;
                s1 += x.first[0];
                s1 += rules[x.first];
                string s2;
                s2 += rules[x.first];
                s2 += x.first[1];

                starting_counts[s1] += x.second;
                starting_counts[x.first] -= x.second;
                starting_counts[s2] += x.second;
                res[rules[x.first]] += x.second;
            }
        }
        counts = starting_counts;
    }

    for (auto x : res) {
        cout << x.first << " " << x.second << endl;
    }

    return 0;
}