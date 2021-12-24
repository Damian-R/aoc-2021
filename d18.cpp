#include "aoc.h"

using namespace std;

class TreeNode {
    public:
        int value, depth = 0;
        TreeNode *left = nullptr, *right = nullptr, *parent = nullptr;

        TreeNode() : value(-1) {}
        TreeNode(TreeNode* parent) : parent(parent), value(-1) {}

        ~TreeNode() {
            if (left) delete left;
            if (right) delete right;
        }
};

TreeNode* build_tree(string& line, TreeNode* parent, int& i) {
    TreeNode* node = new TreeNode(parent);

    if (line[i] == '[') {
        ++i; // [
        node->left = build_tree(line, node, i);
        ++i; // ,
        node->right = build_tree(line, node, i);
        ++i; // ]
    } else if (isdigit(line[i])) {
        node->value = line[i] - '0';
        ++i;
    }

    return node;
}

void update_depths(TreeNode* node) {
    if (!node) return;
    if (node->parent) node->depth = node->parent->depth + 1;
    if (node->left) update_depths(node->left);
    if (node->right) update_depths(node->right);
}

void print(TreeNode* node) {
    if (node == nullptr) return;
    if (node->value == -1) cout << "[";
    print(node->left);
    if (node->value != -1) cout << node->value;
    else cout << ",";
    print(node->right);
    if (node->value == -1) cout << "]";
}

void inorder(vector<TreeNode*>& v, TreeNode* node, bool all) {
    if (!node) return;
    inorder(v, node->left, all);
    if (all || node->value >= 0) v.push_back(node);
    inorder(v, node->right, all);
}

vector<TreeNode*> inorder(TreeNode* node, bool all) {
    vector<TreeNode*> v;
    inorder(v, node, all);
    return v;
}

void postorder(vector<TreeNode*>& v, TreeNode* node, bool all) {
    if (!node) return;
    inorder(v, node->left, all);
    inorder(v, node->right, all);
    if (all || node->value >= 0) v.push_back(node);
}

vector<TreeNode*> postorder(TreeNode* node, bool all) {
    vector<TreeNode*> v;
    inorder(v, node, all);
    return v;
}

TreeNode* find_split(TreeNode* node) {
    vector<TreeNode*> nodes = inorder(node, false);

    for (auto node : nodes) {
        if (node->value > 9) return node;
    }
    return nullptr; // not found
}

bool split(TreeNode* root) {
    TreeNode* to_split = find_split(root);
    if (!to_split) return false;

    // cout << "splitting " << to_split->value << endl;

    to_split->left = new TreeNode(to_split);
    to_split->right = new TreeNode(to_split);

    to_split->left->value = to_split->value/2;
    to_split->right->value = (to_split->value+1)/2;
    to_split->value = -1;
    return true;
}

TreeNode* find_explode(TreeNode* node) {
    vector<TreeNode*> nodes = inorder(node, true);

    for (auto node : nodes) {
        if (node->left && node->right && node->left->value >= 0 && node->right->value >= 0 && node->depth >= 4) return node;
    }
    return nullptr; // not found
}

bool explode(TreeNode* root) {
    TreeNode* to_explode = find_explode(root);
    if (!to_explode) return false;

    vector<TreeNode*> order = inorder(root, false);
    TreeNode* left = to_explode->left;
    TreeNode* right = to_explode->right;

    // cout << "exploding [" << left->value << "," << right->value << "]" << endl;

    int i = 0, j = 0;
    while (order[i] != left) ++i;
    while (order[j] != right) ++j;
    TreeNode *predecessor = i > 0 ? order[i-1] : nullptr;
    TreeNode *successor = j < order.size()-1 ? order[j+1] : nullptr;
    if (predecessor) predecessor->value += left->value;
    if (successor) successor->value += right->value;

    delete left;
    delete right;
    to_explode->left = nullptr;
    to_explode->right = nullptr;
    to_explode->value = 0;
    
    return true;
}

void reduce(TreeNode* root) {
    if (!root) return;

    bool done = false;
    while (!done) {
        done = true;
        update_depths(root);
        // print(root); cout << endl;
        if (explode(root)) {
            done = false;
        } else if (split(root)) {
            done = false;
        }
    }
}

int magnitude(TreeNode* node) {
    if (!node) return 0;
    if (node->value >= 0) return node->value;
    return 3*magnitude(node->left) + 2*magnitude(node->right);
}

TreeNode* add_numbers(TreeNode* one, TreeNode* two) {
    TreeNode* root = new TreeNode(nullptr);
    root->left = one;
    one->parent = root;
    root->right = two;
    two->parent = root;
    reduce(root);
    return root;
}

int main() {
    string line;
    vector<string> numbers;
    while (getline(cin, line)) numbers.push_back(line);

    int i = 0;
    TreeNode* last_result = build_tree(numbers[0], nullptr, i);

    for (int j = 1; j < numbers.size(); ++j) {
        i = 0;
        TreeNode* next = build_tree(numbers[j], nullptr, i);
        TreeNode* result = add_numbers(last_result, next);
        last_result = result;
    }

    cout << "part 1: " << magnitude(last_result) << endl;


    int res = 0;
    for (int first = 0; first < numbers.size(); ++first) {
        for (int second = 0; second < numbers.size(); ++second) {
            if (first == second) continue;
            i = 0;
            TreeNode* a = build_tree(numbers[first], nullptr, i);
            i = 0;
            TreeNode* b = build_tree(numbers[second], nullptr, i);

            TreeNode* added_numbers = add_numbers(a, b);
            res = max(res, magnitude(added_numbers));
        }
    }

    cout << "part 2: " << res << endl;
}