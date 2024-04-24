#include <fstream>
#include <iostream>
#include "fa.h"

using namespace std;
using namespace fa;

int main()
{
    LambdaNFA nfa;
    ifstream in("tests/1.txt");

    in >> nfa;
    DFA dfa = nfa.turnDeterministic();

    cout << "NFA: " << nfa << endl;
    cout << "DFA: " << dfa << endl;

    int inputCount;
    string input;
    in >> inputCount;
    for (int index = 0; index < inputCount; index++)
    {
        in >> input;

        vector<vector<int>> solutions = nfa.evaluateStringWithPath(input);
        if (!solutions.empty())
        {
            cout << "NFA - VALID" << endl;
            for (const auto &solution : solutions)
            {
                for (auto state : solution)
                    cout << state << " ";
                cout << endl;
            }
        }
        else
            cout << "NFA - INVALID" << endl;

        vector<int> solution = dfa.evaluateStringWithPath(input);
        if (!solution.empty())
        {
            cout << "DFA - VALID" << endl;
            for (auto state : solution)
                cout << state << " ";
            cout << endl;
        }
        else
            cout << "DFA - INVALID" << endl;
    }
    return 0;
}