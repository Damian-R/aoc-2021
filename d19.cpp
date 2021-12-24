#include "aoc.h"

using namespace std;
typedef tuple<int, int, int> t3;

double distance(t3&, t3&);

vector<vector<int>> rotations = {
    { 1, 1, 1 },
    { 1, 1,-1 },
    { 1,-1, 1 },
    { 1,-1,-1 },
    {-1, 1, 1 },
    {-1, 1,-1 },
    {-1,-1, 1 },
    {-1,-1,-1 },
};

t3 vec3_subtract(const t3& a, const t3& b) {
    return make_tuple(
        get<0>(a) - get<0>(b),
        get<1>(a) - get<1>(b),
        get<2>(a) - get<2>(b)
    );
}

t3 vec3_add(const t3& a, const t3& b) {
    return make_tuple(
        get<0>(a) + get<0>(b),
        get<1>(a) + get<1>(b),
        get<2>(a) + get<2>(b)
    );
}

class Beacon {
    public:
    int id; // debug help
    t3 location;
    set<double> dists; // distances to other beacons
    Beacon(int x, int y, int z) : location(make_tuple(x, y, z)) {}

    int operator[](int idx) {
        switch (idx) {
            case 0: return get<0>(location);
            case 1: return get<1>(location);
            case 2: return get<2>(location);
        }

        return -1; // error state
    }

    t3 permute_location(vector<int>& permutation) {
        return make_tuple(operator[](permutation[0]), operator[](permutation[1]), operator[](permutation[2]));
    }

    friend bool operator==(Beacon& b1, Beacon& b2) {
        return b1[0] == b2[0] && b1[1] == b2[1] && b1[2] == b2[2];
    }
};

class Scanner {
    public:
    t3 location;
    vector<Beacon*> beacons;

    void add_beacon(Beacon* beacon) {
        beacons.push_back(beacon);
    }

    void update_beacon_dists() {
        for (auto beacon1 : beacons) {
            for (auto beacon2 : beacons) {
                double d = distance(beacon1->location, beacon2->location);
                if (d > 0) beacon1->dists.insert(d);
            }
        }
    }
};

double distance(t3& a, t3& b) {
    auto [ aX, aY, aZ ] = a;
    auto [ bX, bY, bZ ] = b;
    return sqrt(pow(aX-bX, 2) + pow(aY-bY, 2) + pow(aZ-bZ, 2));
}

int beacon_distance_intersection_size(Beacon* b, Beacon* o) {
    set<double> s;
    
    set_intersection(
        b->dists.begin(),
        b->dists.end(),
        o->dists.begin(),
        o->dists.end(),
        inserter(s, s.begin())
    );

    return s.size();
}

vector<set<Beacon*>> beacon_set_intersection(Scanner* s, Scanner* o) {
    set<Beacon*> s_beacons;
    set<Beacon*> o_beacons;
    for (auto b1 : s->beacons) {
        for (auto b2 : o->beacons) {
            // if two beacons in different coordinate systems share at least 11 
            // inter-beacon (from self to other beacons) vectors of
            // equal magnitude, we consider them the same beacon.
            // since we consider regions overlapping if there are at least 12
            // shared beacons, at least 12 beacons within the overlap will satisfy
            // this criteria
            if (beacon_distance_intersection_size(b1, b2) >= 11) {
                s_beacons.insert(b1);
                o_beacons.insert(b2);
            }
        }
    }

    return { s_beacons, o_beacons };
}

long long int manhattan_distance(t3& a, t3& b) {
    return abs(get<0>(a)-get<0>(b)) + abs(get<1>(a)-get<1>(b)) + abs(get<2>(a)-get<2>(b));
}

int main() {
    string line;
    set<Scanner*> unknown_scanners;
    Scanner* origin_scanner = nullptr;

    vector<t3> scanner_locations;
    scanner_locations.push_back(make_tuple(0, 0, 0));

    // process input
    while (getline(cin, line)) {
        string coords;
        Scanner* s = new Scanner();

        if (origin_scanner) unknown_scanners.insert(s);
        else origin_scanner = s;

        while (getline(cin, coords)) {
            if (coords.size() == 0) break;

            vector<int> pos = split(coords, ',');
            Beacon* b = new Beacon(pos[0], pos[1], pos[2]);
            s->add_beacon(b);
        }

        s->update_beacon_dists();
    }

    // merge beacon locations in the first scanner's beacon list
    // and only compare other scanners to the first scanner
    Scanner* s1 = origin_scanner;
    while (!unknown_scanners.empty()) {
        s1->update_beacon_dists();
        for (auto s2 : unknown_scanners) {
            s2->update_beacon_dists();
            auto beacon_intersection = beacon_set_intersection(s1, s2);
            auto s1_beacons = beacon_intersection[0];
            auto s2_beacons = beacon_intersection[1];

            if (s2_beacons.size() >= 11 || unknown_scanners.size() == 1) {
                unknown_scanners.erase(s2);

                set<Beacon*> to_be_translated; // beacons to be added to s1

                for (auto b : s2->beacons) {
                    if (s2_beacons.find(b) == s2_beacons.end()) to_be_translated.insert(b);
                }

                // translate beacon coordinates to s1's coordinate system:
                //      iterate through all possible rotations and axis permutations
                //      if chosen permutation + rotation results in all beacons in
                //      intersection matching with beacon positions already in s1 (plus a constant offset, s2's relative location to s1),
                //      then the permutation + rotation is valid and can be used to translate

                vector<int> permutation = {0, 1, 2};
                vector<int> rotation;
                int rotation_idx = 0;
                bool done = false;
                t3 s2; // origin of unknown scanner

                do {
                    rotation_idx = 0;
                    for (; rotation_idx < rotations.size(); ++rotation_idx) {
                        rotation = rotations[rotation_idx];

                        map<t3, int> candidate_s2_locations;
                        for (auto b2 : s2_beacons) {
                            for (auto b1 : s1_beacons) {
                                auto [ x, y, z ] = b2->permute_location(permutation);
                                Beacon transformed(x*rotation[0], y*rotation[1], z*rotation[2]);

                                candidate_s2_locations[vec3_subtract(b1->location, transformed.location)]++;
                            }
                        }

                        for (auto& p : candidate_s2_locations) {                            
                            if (p.second == s1_beacons.size()) {
                                s2 = p.first;
                                done = true;
                            }
                        }

                        if (done) break;
                    }
                } while (
                    !done
                    && next_permutation(permutation.begin(), permutation.end())
                );
                // permutation & rotation valid, s2 = origin of unknown scanner relative to (0,0,0)

                scanner_locations.push_back(s2);
                
                for (auto b : to_be_translated) {
                    auto [ x, y, z ] = b->permute_location(permutation);
                    t3 translated_location = make_tuple(x*rotation[0], y*rotation[1], z*rotation[2]);
                    b->location = vec3_add(translated_location, s2); // translate to s1's coordinate system
                    s1->add_beacon(b); // add to s1's beacon list
                }
            }
        }
    }

    // calculate max manhattan distance
    long long int res = 0;
    for (auto s1 : scanner_locations) {
        for (auto s2 : scanner_locations) {
            if (s1 == s2) continue;
            res = max(res, manhattan_distance(s1, s2));
        }
    }

    cout << "PART 1: " << origin_scanner->beacons.size() << endl; // TODO: sometimes overcounts by <10 ?
    cout << "PART 2: " << res << endl; // always correct
}