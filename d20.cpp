#include "aoc.h"

using namespace std;

int main() {
    string algorithm;
    getline(cin, algorithm);

    set<pair<int, int>> light_pixels;

    string line;
    getline(cin, line); // skip empty line

    int y = 0;
    int max_x = 0, min_x = 0, max_y = 0, min_y = 0;
    while (getline(cin, line)) {
        for (int x = 0; x < line.size(); ++x)
            if (line[x] == '#') {
                light_pixels.insert(make_pair(y, x));
                max_x = max(x, max_x);
                max_y = max(y, max_y);
                min_x = min(x, min_x);
                min_y = min(y, min_y);
            }
        ++y;
    }

    for (auto x : light_pixels) {
        cout << x.first << " " << x.second << endl;
    }

    cout << min_x << " " << max_x << " " << min_y << " " << max_y << endl;
    max_x = max_x + 2;
    max_y = max_y + 2;
    min_x = min_x - 2;
    min_y = min_y - 2;


}