#include <map>
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class LambdaNFA {
private:
    set<int> states;
    map<int, set<pair<int, char>>> transitions;
    int initialState;
    set<int> finalStates;
    char lambdaChar;
    string lambdaString;

    void DFS(bool &ok, int depth, const string &word, int currentState, vector<vector<pair<int, char>>> &solutionPaths,
             vector<pair<int, char>> &currentPath, set<int> &lambdaStates) const {
        if (depth == word.length() && finalStates.find(currentState) != finalStates.end()) {
            ok = true;
            solutionPaths.emplace_back(currentPath);
        } else {
            auto itt = transitions.find(currentState);
            if (itt != transitions.end())
                for (auto transition: itt->second) {
                    if (lambdaChar == transition.second && lambdaStates.find(transition.first) == lambdaStates.end()) {
                        lambdaStates.insert(transition.first);
                        currentPath.emplace_back(transition);
                        DFS(ok, depth, word, transition.first, solutionPaths, currentPath, lambdaStates);
                        currentPath.pop_back();
                    }
                    if (word[depth] == transition.second) {
                        currentPath.emplace_back(transition);
                        DFS(ok, depth + 1, word, transition.first, solutionPaths, currentPath, lambdaStates);
                        currentPath.pop_back();
                    }
                }


        }
    }

public:
    explicit LambdaNFA(int initialState = 0, char lambdaChar = '0', string lambdaString = "-") : initialState(
            initialState), lambdaChar(lambdaChar), lambdaString(lambdaString) {}

    friend istream &operator>>(istream &in, LambdaNFA &a) {
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

    friend ostream &operator<<(ostream &out, LambdaNFA &a) {
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
        vector<pair<int, char>> current;
        vector<vector<pair<int, char>>> solutions;
        set<int> lambda;
        DFS(ok, 0, s, initialState, solutions, current, lambda);
        return ok;
    }

    vector<vector<int>> evaluateStringWithPath(const string &s) const {
        vector<vector<int>> answer;
        if (!s.compare(lambdaString))
            return (finalStates.find(initialState) != finalStates.end()) ? vector<vector<int>>{
                    vector<int>{initialState}} : vector<vector<int>>{};

        bool ok = false;
        vector<pair<int, char>> current;
        vector<vector<pair<int, char>>> solutions;
        set<int> lambda;
        DFS(ok, 0, s, initialState, solutions, current, lambda);
        if (!ok)
            return answer;

        for (const auto &solution: solutions) {
            vector<int> path{initialState};
            for (auto transition: solution)
                path.emplace_back(transition.first);
            answer.emplace_back(path);
        }

        return answer;
    }
};

int main() {
    // does not accept lambda character
    // alternatively, convert LambdaNFA to DFA then evaluate
    LambdaNFA lnfa;

    ifstream in("../src/teme/tema1/tests/5.txt");
    in >> lnfa;

    int nrWords;
    string word;

    in >> nrWords;
    for (int index = 0; index < nrWords; index++) {
        in >> word;
        vector<vector<int>> solutions = lnfa.evaluateStringWithPath(word);
        if (!solutions.empty()) {
            cout << "VALID" << endl;
            for (const auto &solution: solutions) {
                for (auto state: solution)
                    cout << state << " ";
                cout << endl;
            }
        } else
            cout << "INVALID" << endl;
    }
    return 0;
}