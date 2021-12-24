#include "aoc.h"

using namespace std;

bool missed(vector<int>& targetX, vector<int>& targetY, int vX, int vY, int posX, int posY) {
    return posX > targetX[1] || posY < targetY[0];
}

bool hit(vector<int>& targetX, vector<int>& targetY, int posX, int posY) {
    return posX >= targetX[0] && posX <= targetX[1] && posY >= targetY[0] && posY <= targetY[1];
}

int main() {
    vector<int> targetX = {277, 318};
    vector<int> targetY = {-92, -53};

    int res = 0, count = 0;
    for (int _vY = -92; _vY < 1000; ++_vY) {
        for (int _vX = 0; _vX <= 318; ++_vX) {
            int vX = _vX, vY = _vY, posX = 0, posY = 0;
            int step = 0;
            int h = 0;
            while (!missed(targetX, targetY, vX, vY, posX, posY)) {
                posX += vX;
                posY += vY;
                if (vX > 0) --vX;
                --vY;
                ++step;
                h = max(h, posY);
                if (hit(targetX, targetY, posX, posY)) {
                    ++count;
                    res = max(res, h);
                    break;
                }
            }
        }
    }

    cout << res << endl;
    cout << count << endl;
}