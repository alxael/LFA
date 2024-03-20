#include <map>
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class NFA {
private:
    set<int> states;
    map<int, set<pair<int, char>>> transitions;
    int initialState;
    set<int> finalStates;
    string lambdaString;

    void DFS(bool &ok, int depth, const string &word, int currentState, vector<pair<int, char>> &solutionPath,
             vector<pair<int, char>> &currentPath) const {
        if (depth == word.length() && finalStates.find(currentState) != finalStates.end()) {
            ok = true;
            solutionPath = currentPath;
        } else {
            auto itt = transitions.find(currentState);
            if (itt != transitions.end())
                for (auto transition: itt->second)
                    if (word[depth] == transition.second) {
                        currentPath.emplace_back(transition);
                        DFS(ok, depth + 1, word, transition.first, solutionPath, currentPath);
                        currentPath.pop_back();
                    }

        }
    }

public:
    explicit NFA(int initialState = 0, string lambdaString = "-") : initialState(initialState),
                                                                    lambdaString(lambdaString) {}

    friend istream &operator>>(istream &in, NFA &a) {
        int n, m, nrf, x, y;
        char l;
        in >> n;
        for (int i = 1; i <= n; i++) {
            in >> x;
            a.states.insert(x);
        }
        in >> m;
        for (int i = 1; i <= m; i++) {
            in >> x >> y >> l;
            pair<int, char> transition(y, l);
            if (a.transitions.find(x) == a.transitions.end())
                a.transitions.insert(pair<int, set<pair<int, char>>>(x, set<pair<int, char>>{transition}));
            else
                a.transitions[x].insert(transition);
        }
        in >> a.initialState;
        in >> nrf;
        for (int i = 1; i <= nrf; i++) {
            in >> x;
            a.finalStates.insert(x);
        }
        return in;
    }

    friend ostream &operator<<(ostream &out, NFA &a) {
        out << "States: ";
        for (int state: a.states)
            out << state << ' ';
        out << endl << "Transitions: " << endl;
        for (auto &transition: a.transitions)
            for (auto itv = (transition.second).begin(); itv != (transition.second).end(); itv++)
                out << "From " << transition.first << " to " << itv->first << " with value " << itv->second << endl;
        out << "Initial state: " << a.initialState << endl;
        out << "Final states: ";
        for (int finalState: a.finalStates)
            out << finalState << " ";
        out << endl;
        return out;
    }

    bool evaluateString(const string &s) const {
        if (!s.compare(lambdaString))
            return (finalStates.find(initialState) != finalStates.end());

        bool ok = false;
        vector<pair<int, char>> current, solution;
        DFS(ok, 0, s, initialState, solution, current);
        return ok;
    }

    vector<int> evaluateStringWithPath(const string &s) const {
        vector<int> answer;
        if (!s.compare(lambdaString))
            return (finalStates.find(initialState) != finalStates.end()) ? vector<int>{initialState} : vector<int>{};

        bool ok = false;
        vector<pair<int, char>> current, solution;
        DFS(ok, 0, s, initialState, solution, current);
        if (!ok)
            return answer;
        answer.emplace_back(initialState);
        for (auto transition: solution)
            answer.emplace_back(transition.first);
        return answer;
    }
};

int main() {
    // does not accept lambda character
    // alternatively, convert NFA to DFA then evaluate
    NFA nfa;

    ifstream in("../src/teme/tema1/tests/1.txt");
    in >> nfa;

    int nrWords;
    string word;

    in >> nrWords;
    for (int index = 0; index < nrWords; index++) {
        in >> word;
        vector<int> solution = nfa.evaluateStringWithPath(word);
        if (!solution.empty()) {
            cout << "VALID" << endl;
            for (auto value: solution)
                cout << value << " ";
            cout << endl;
        } else
            cout << "INVALID" << endl;
    }
    return 0;
}