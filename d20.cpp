#include "aoc.h"

using namespace std;

int kernel_to_int(vector<vector<char>>& pixels, int row, int col) {
    vector<int> vert = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    vector<int> horiz = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    vector<int> bin;

    for (int i = 0; i < 9; ++i) {
        if (pixels[row+vert[i]][col+horiz[i]] == '#') bin.push_back(1);
        else bin.push_back(0);
    }

    int result = 0;
    for (int i = bin.size()-1; i >= 0; --i) {
        result += bin[i]*pow(2, bin.size()-i-1);
    }

    return result;
}

vector<vector<char>> simulate_turn(string& algorithm, vector<vector<char>>& pixels) {
    vector<vector<char>> new_board(pixels.size()-2, vector<char>(pixels.size()-2));

    for (int i = 1; i < pixels.size()-1; ++i) {
        for (int j = 1; j < pixels.size()-1; ++j) {
            int val = kernel_to_int(pixels, i, j);
            new_board[i-1][j-1] = algorithm[val];
        }
    }

    return new_board;
}

int main() {
    int MAX_TURNS = 50, turn = 0;
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
        // Insert new top, bottom row
        pixels.insert(pixels.begin(), row);
        pixels.insert(pixels.begin(), row);
        pixels.push_back(row);
        pixels.push_back(row);

        // Insert new left, right column
        for (auto& r : pixels) {
            r.insert(r.begin(), outer);
            r.insert(r.begin(), outer);
            r.push_back(outer);
            r.push_back(outer);
        }

        auto new_pixels = simulate_turn(algorithm, pixels);
        pixels = new_pixels;

        ++turn;
    }

    int light_pixels = 0;
    for (auto row : pixels) {
        for (auto col : row) {
            if (col == '#') ++light_pixels;
        }
    }

    cout << "Light pixels: " << light_pixels << endl;

}