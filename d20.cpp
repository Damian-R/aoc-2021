#include "aoc.h"

using namespace std;

int kernel_to_int(vector<vector<char>>& pixels, int row, int col) {
    vector<int> vert = { 0, 0, 0, 1, 1, 1, -1, -1, -1 };
    vector<int> horiz = { 0, 1, -1, 0, 1, -1, 0, 1, -1 };

    vector<int> bin;

    for (int i = 0; i < 9; ++i) {
        if (pixels[row+vert[i]][col+horiz[i]] == '#') bin.push_back(1);
        else bin.push_back(0);
    }

    int result = 0;
    for (int i = bin.size()-1; i >= 0; --i) {
        result += pow(2, bin.size()-i-1);
    }

    return result;
}

vector<vector<char>> simulate_turn(string& algorithm, vector<vector<char>>& pixels) {
    vector<vector<char>> new_board(pixels.size(), vector<char>(pixels.size()));

    for (int i = 1; i < pixels.size()-1; ++i) {
        for (int j = 1; j < pixels.size()-1; ++j) {
            int val = kernel_to_int(pixels, i, j);
            new_board
        }
    }

    return new_board;
}

int main() {
    int MAX_TURNS = 2, turn = 0;
    string algorithm;
    getline(cin, algorithm);

    vector<vector<char>> pixels;

    string line;
    getline(cin, line); // skip empty line

    while (getline(cin, line)) {
        vector<char> row;
        for (auto c : line) row.push_back(c);
        pixels.push_back(row);
    }

    

    while (turn < MAX_TURNS) {
        char outer = turn % 2 == 0 ? '.' : '#';
        vector<char> row(pixels.size(), outer);
        pixels.insert(pixels.begin(), row);
        pixels.push_back(row);

        for (auto& r : pixels) {
            r.insert(r.begin(), outer);
            r.push_back(outer);
        }

        cout << "simulating on: " << endl;

        for (auto row : pixels) {
            for (auto col : row) cout << col;
            cout << endl;
        }

        auto new_pixels = simulate_turn(algorithm, pixels);
        ++turn;
    }

}