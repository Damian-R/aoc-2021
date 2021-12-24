#include "aoc.h"

using namespace std;

// https://stackoverflow.com/questions/18310952/convert-strings-between-hex-format-and-binary-format
const char* hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
}

std::string hex_str_to_bin_str(const std::string& hex)
{
    std::string bin;
    for (unsigned i = 0; i != hex.length(); ++i)
       bin += hex_char_to_bin(hex[i]);
    return bin;
}

int get_version(int i, string& bitstring) {
    string v = bitstring.substr(i, 3);
    bitset<3> bits;
    istringstream is(v);
    is >> bits;

    return bits.to_ulong();
}

int get_subpacket_bitlength(int i, string& bitstring) {
    string L = bitstring.substr(i, 15);
    bitset<15> bits;
    istringstream is(L);
    is >> bits;

    return bits.to_ulong();
}

int get_num_subpackets(int i, string& bitstring) {
    string L = bitstring.substr(i, 11);
    bitset<11> bits;
    istringstream is(L);
    is >> bits;

    return bits.to_ulong();
}

int get_type(int i, string& bitstring) {
    return get_version(i, bitstring);
}

class TreeNode {
    public:
    int version;
    int type;
    long long int value = -1;
    vector<TreeNode*> subpackets;

    TreeNode(int version, int type) : version(version), type(type) {}

    void add_child(TreeNode* node) {
        if (node) subpackets.push_back(node);
    }
};

TreeNode* build_tree(int& i, string& bitstring) {
    cout << "building tree " << i << endl;
    if (i >= bitstring.size()) return nullptr;
    int version = get_version(i, bitstring);
    i += 3; if (i >= bitstring.size()) return nullptr;
    int type = get_type(i, bitstring);
    i += 3; if (i >= bitstring.size()) return nullptr;
    TreeNode* node = new TreeNode(version, type);

    if (type == 4) {
        // literal
        bool more = false;
        vector<int> v;
        do {
            more = bitstring[i] == '1';
            if (i+4 >= bitstring.size()) return nullptr;
            for (int j = 1; j <= 4; ++j) v.push_back(bitstring[i+j] - '0');
            i += 5;
        } while (more);

        long long int n = 0;
        for (int j = v.size()-1; j >= 0; --j) n += v[j]*pow(2, v.size()-j-1);
        node->value = n;
    } else {
        // operator
        char type_id = bitstring[i];
        i += 1; if (i >= bitstring.size()) return nullptr;
        if (type_id == '0') {
            int bitlength = get_subpacket_bitlength(i, bitstring);
            i += 15;
            int m = bitlength+i;
            while (i < m) {
                node->add_child(build_tree(i, bitstring));
            }
        } else if (type_id == '1') {
            int num_subpackets = get_num_subpackets(i, bitstring);
            i += 11;
            for (int j = 0; j < num_subpackets; ++j) {
                node->add_child(build_tree(i, bitstring));
            }
        }
    }

    return node;
}

void postorder(vector<TreeNode*>& v, TreeNode* node) {
    if (!node) return;
    for (auto child : node->subpackets) postorder(v, child);
    v.push_back(node);
}

vector<TreeNode*> postorder(TreeNode* root) {
    vector<TreeNode*> v;
    postorder(v, root);
    return v;
}

void evaluate(TreeNode* node) {
    if (!node) return;
    long long int res = 0;
    switch(node->type) {
        case 0: { // sum
            for (auto c : node->subpackets) {
                if (c->type != 4) evaluate(c);
                res += c->value;
            }
            break;
        }
        case 1: {
            // product
            res = 1;
            for (auto c : node->subpackets) {
                if (c->type != 4) evaluate(c);
                res *= c->value;
            }
            break;
        }
        case 2: { // minimum
            res = LLONG_MAX;
            for (auto c : node->subpackets) {
                if (c->type != 4) evaluate(c);
                res = min(res, c->value);
            }
            break;
        }
        case 3: { // maximum
            res = LLONG_MIN;
            for (auto c : node->subpackets) {
                if (c->type != 4) evaluate(c);
                res = max(res, c->value);
            }
            break;
        }
        case 5: { // greater than
            for (auto c : node->subpackets) {
                if (c->type != 4) evaluate(c);
            }

            res = node->subpackets[0]->value > node->subpackets[1]->value ? 1 : 0;
            break;
        }
        case 6: { // less than
            for (auto c : node->subpackets) {
                if (c->type != 4) evaluate(c);
            }

            res = node->subpackets[0]->value < node->subpackets[1]->value ? 1 : 0;
            break;
        }
        case 7: { // equal to
            for (auto c : node->subpackets) {
                if (c->type != 4) evaluate(c);
            }

            res = node->subpackets[0]->value == node->subpackets[1]->value ? 1 : 0;
            break;
        }
    }

    node->value = res;
    node->type = 4; // node is now a literal
}

int main() {
    string line;
    getline(cin, line);

    string bitstring = hex_str_to_bin_str(line);
    cout << bitstring << endl;

    int i = 0;
    TreeNode* root = build_tree(i, bitstring);
    vector<TreeNode*> po = postorder(root);

    int versions = 0;
    for (auto n : po) {
        versions += n->version;
    }

    evaluate(root);
    cout << "part 1: " << versions << endl;
    cout << "part 2: " << root->value << endl;
}