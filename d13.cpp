#include "aoc.h"

using namespace std;

void fold(vector<vector<bool>>& paper, char dir, int loc) {
    if (dir == 'y') {
        for (int i = 1; loc-i >= 0 && loc+i < paper.size(); ++i) {
            int l1 = loc-i;
            int l2 = loc+i;
            for (int j = 0; j < paper[0].size(); ++j) {
                if (paper[l2][j])
                    paper[l1][j] = true;
            }
        }
    } else if (dir == 'x') {
        for (int i = 1; loc-i >= 0 && loc+i < paper[0].size(); ++i) {
            int l1 = loc-i;
            int l2 = loc+i;
            for (int j = 0; j < paper[0].size(); ++j) {
                if (paper[j][l2])
                    paper[j][l1] = true;
            }
        }
    }
}

int main() {
    string line;
    vector<vector<int>> coords;
    vector<pair<char, int>> folds;
    regex reg("fold along (x|y)=(\\d+)");

    bool f = false;
    while (getline(cin, line)) {
        if (line.size() == 0) {
            f = true;
            continue;
        }
        
        if (!f) {
            vector<int> c = split(line, ',');
            coords.push_back(c);
        } else {
            smatch match;
            regex_match(line, match, reg);
            int loc = stoi(match[2]);
            char c = match[1].str()[0];
            folds.push_back(make_pair(c, loc));
        }
    }

    vector<vector<bool>> paper(1500, vector<bool>(1500, false));
    for (auto v : coords) {
       paper[v[1]][v[0]] = true; 
    }

    int x = 1500, y = 1500;
    for (auto f : folds) {
        fold(paper, f.first, f.second);
        if (f.first == 'x') x = f.second;
        else y = f.second;
    }

    for (int i = 0; i < y; ++i) {
        for (int j = 0; j < x; ++j) {
            cout << (paper[i][j] ? '#' : '.');
        }
        cout << endl;
    }

    return 0;
}