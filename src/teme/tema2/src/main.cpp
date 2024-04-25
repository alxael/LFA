#include <fstream>
#include <iostream>
#include "fa.h"

using namespace std;
using namespace fa;

int main()
{
    DFA dfa;
    NFA nfa;

    ifstream din("tests/4.txt");
    din >> dfa;

    ifstream nin("tests/1.txt");
    nin >> nfa;
    DFA converted = nfa.turnDeterministic();

    cout << "Task 1: " << endl;
    cout << "NFA: " << endl
         << nfa << endl;
    dfa.minimize();
    cout << "Converted DFA: " << endl
         << converted << endl;

    int inputCount;
    string input;
    nin >> inputCount;
    for (int index = 0; index < inputCount; index++)
    {
        nin >> input;

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

        vector<int> solution = converted.evaluateStringWithPath(input);
        if (!solution.empty())
        {
            cout << "DFA - VALID" << endl;
            for (auto state : solution)
                cout << state << " ";
            cout << endl;
        }
        else
            cout << "DFA - INVALID" << endl;
        cout << endl;
    }

    cout << "Task 2: " << endl;
    cout << "DFA: " << endl
         << dfa << endl;
    dfa.minimize();
    cout << "Minimized DFA: " << endl
         << dfa << endl;
    return 0;
}