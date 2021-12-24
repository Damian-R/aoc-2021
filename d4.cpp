#include <iostream>
#include <vector>
#include <string>
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

typedef vector<vector<int>> board;
bool board_wins(board& current_board) {
  // check rows
  for (auto row : current_board) {
    bool win = true;
    for (auto col : row) {
      if (col != -1) win = false;
    }
    if (win) return true;
  }

  // check cols
  for (int i = 0; i < 5; ++i) {
    bool win = false;
    for (int j = 0; j < 5; ++j) {
      if (current_board[j][i] != -1) win = false;
    }
    if (win) return true;
  }

  return false;
}

int score(board& current_board, int chosen) {
  int sum = 0;
  for (auto row : current_board) {
    for (auto col : row) {
      if (col != -1) sum += col;
    }
  }

  return sum*chosen;
}


int main() {
  string line;
  getline(cin, line);
  vector<int> sequence = split(line, ',');

  // read in boards
  vector<board> boards;
  while (getline(cin, line)) {
    board current_board(5, vector<int>(5));
    for (int row = 0; row < 5; ++row) {
      for (int col = 0; col < 5; ++col) {
        cin >> current_board[row][col];
      }
    }
    boards.push_back(current_board);
  }

  int remaining = boards.size();
  vector<bool> status(boards.size(), true);
  // naive solution because its past midnight
  for (auto chosen : sequence) {
    for (int i = 0; i < boards.size(); ++i) {
      if (!status[i]) continue; // slight optimization
      board& current_board = boards[i];
      for (auto& row : current_board) {
        for (auto& col : row) {
          if (col == chosen) col = -1;
        }
      }

      if (board_wins(current_board)) {

        for (int i = 0; i < 5; ++i) {
          for (int j = 0; j < 5; ++j) {
            cout << current_board[i][j] << " ";
          }
          cout << endl;
        }
        cout << endl;
        cout << chosen << endl;

        --remaining;
        status[i] = false;
        if (remaining == 0) {
          cout << score(current_board, chosen) << endl;
          return 0;
        }
      }
    }
  }

  return 0;
}