#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

// REPRISE :
// faire une passe de ménage, notamment vis-à-vis des copies
// notamment, faire une initialisation récursive pour ne pas avoir de copies inutiles
// utiliser des union pour représenter la balance pointée sous forme d'un index ou d'une balance ?
// supposition à mentionner explicitement : un seul arbre pour les balances (il n'y a pas plusieurs composantes
// disjointes) supposition à mentionner explicitement : la balance 0 est forcément la racine de l'arbre supposition à
// mentionner explicitement : fichier d'entrée bien formé edge-case auquel on est robuste = quand il y a plusieurs
// façons d'équilibrer les balances

using namespace std;

struct Balance;

struct Plate {
    int initial_weight = 0;
    int added_weight = 0;

    vector<Balance*> balances;
    vector<int> balances_indexes;

    void add_weight(int weight) { added_weight += weight; }
    int mass() const;

    string stacked_balances_indexes_as_str() const {
        return accumulate(
            balances_indexes.begin(), balances_indexes.end(), string(""), [](string const& s, int const& b) {
                return string(s) + " " + to_string(b);
            });
    }
};

string get_beginning_of_first_non_commented_line(istream& stream) {
    string s;
    do {
        getline(stream, s);
    } while (s.front() == '#');
    return s;
}

istream& operator>>(istream& stream, Plate& p) {
    string s = get_beginning_of_first_non_commented_line(stream);
    istringstream ss(s);
    ss >> p.initial_weight;

    while (!ss.eof()) {
        int index;
        ss >> index;
        p.balances_indexes.push_back(index);
    }
    return stream;
}

struct Visitor {
    virtual ~Visitor() = default;
    virtual void visit(Balance& visited) = 0;
};

struct Balance {
    Balance() = default;
    Balance(Plate&& left_, Plate&& right_) : left{left_}, right{right_} {}
    void accept(Visitor& v) { v.visit(*this); }
    int mass() const { return BALANCE_MASS + left.mass() + right.mass(); }
    Plate left;
    Plate right;
    static int const BALANCE_MASS = 10;
};

int Plate::mass() const {
    int balances_mass = accumulate(balances.begin(), balances.end(), 0, [](int current_mass, Balance const* b) {
        return current_mass + b->mass();
    });
    return initial_weight + added_weight + balances_mass;
}

vector<Balance> parse(string const& fpath) {
    ifstream f(fpath);
    assert(f.good());
    string s = get_beginning_of_first_non_commented_line(f);
    istringstream ss(s);
    int nb_balances = 0;
    ss >> nb_balances;
    vector<Balance> balances;
    for (int balance_index = 0; balance_index < nb_balances; ++balance_index) {
        Plate left, right;
        f >> left;
        f >> right;
        balances.push_back(Balance(move(left), move(right)));
    }

    // at this point, all Balances=Plates are built, but point to other balances as indices
    // we make them point to other balances as references

    // all balances are also built, but their plates point to other balances as indices
    for (Balance& balance : balances) {
        // not very elegant : I need a vector because balance must be at the same time
        // in the global array, AND as a
        for (int balance_index : balance.left.balances_indexes) {
            balance.left.balances.push_back(&balances[balance_index]);
        }
        for (int balance_index : balance.right.balances_indexes) {
            balance.right.balances.push_back(&balances[balance_index]);
        }
    }

    return balances;
}

struct DebuggingVisitor : public Visitor {
    DebuggingVisitor() : m_index{0} {}

    void visit(Balance& visited) override {
        cout << endl;
        cout << "Visiting balance #" << m_index++ << endl;
        cout << "Left  plate (initial=" << visited.left.initial_weight << ") has " << visited.left.added_weight
             << " added" << endl;
        cout << "Right plate (initial=" << visited.right.initial_weight << ") has " << visited.right.added_weight
             << " added" << endl;
        for (Balance* b : visited.left.balances) {
            b->accept(*this);
        }
        for (Balance* b : visited.right.balances) {
            b->accept(*this);
        }
    }
    unsigned int m_index;
};

struct EquilibratingVisitor : public Visitor {
    void visit(Balance& visited) override {
        // equilibrate left children :
        for (auto stacked_balance : visited.left.balances) {
            stacked_balance->accept(*this);
        }

        // equilibrate right children :
        for (auto stacked_balance : visited.right.balances) {
            stacked_balance->accept(*this);
        }

        // equilibrate self :
        int diff = visited.right.mass() - visited.left.mass();
        if (diff > 0) {
            visited.left.add_weight(diff);
        } else if (diff < 0) {
            visited.right.add_weight(-diff);
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw invalid_argument("missing fpath");
    }
    vector<Balance> balances = parse(string(argv[1]));
    Balance& root = balances[0];
    /* DebuggingVisitor visitor; */
    EquilibratingVisitor visitor;
    root.accept(visitor);
    unsigned int counter = 0;
    for (auto const& b : balances) {
        cout << counter++ << ": " << b.left.added_weight << " " << b.right.added_weight << endl;
    }
    return 0;
}
