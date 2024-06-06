#include <fstream>
#include <iostream>
#include "fa.hpp"

using namespace std;
using namespace fa;

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

int main()
{
    int testNumber, finiteAutomataImplementation;

    cout << "For this program we consider the following: " << endl;
    cout << "1 - States are integers, transitions are characters." << endl;
    cout << "2 - Lambda character is '0' and lambda string is '-'" << endl;
    cout << endl;

    while (true)
    {
        cout << "Please select the type of finite automata implementation:" << endl;
        cout << "1 - DFA" << endl;
        cout << "2 - NFA" << endl;
        cout << "3 - Lambda NFA" << endl;
        cin >> finiteAutomataImplementation;
        if (finiteAutomataImplementation >= 1 && finiteAutomataImplementation <= 3)
            break;
        else
        {
            system(CLEAR_COMMAND);
            cout << "Please enter a valid finite automata implementation" << endl;
        }
    }
    cout << endl;

    while (true)
    {
        cout << "Please select the test number (1-5): " << endl;
        cin >> testNumber;
        if (testNumber >= 1 && testNumber <= 5)
            break;
        else
        {
            system(CLEAR_COMMAND);
            cout << "Please enter a valid test number" << endl;
        }
    }
    cout << endl;

    ifstream in("./tests/" + to_string(testNumber) + ".txt");

    switch (finiteAutomataImplementation)
    {
    case 1:
    {
        DFA automata;
        in >> automata;

        int nrWords;
        string word;
        in >> nrWords;
        for (int index = 0; index < nrWords; index++)
        {
            in >> word;
            vector<int> solution = automata.evaluateStringWithPath(word);
            if (!solution.empty())
            {
                cout << "VALID" << endl;
                for (auto value : solution)
                    cout << value << " ";
                cout << endl;
            }
            else
                cout << "INVALID" << endl;
        }
        break;
    }

    case 2:
    {
        NFA automata;
        in >> automata;

        int nrWords;
        string word;
        in >> nrWords;
        for (int index = 0; index < nrWords; index++)
        {
            in >> word;
            vector<int> solution = automata.evaluateStringWithPath(word);
            if (!solution.empty())
            {
                cout << "VALID" << endl;
                for (auto value : solution)
                    cout << value << " ";
                cout << endl;
            }
            else
                cout << "INVALID" << endl;
        }
        break;
    }

    case 3:
    {
        LambdaNFA lnfa;
        in >> lnfa;

        int nrWords;
        string word;

        in >> nrWords;
        for (int index = 0; index < nrWords; index++)
        {
            in >> word;
            vector<vector<int>> solutions = lnfa.evaluateStringWithPath(word);
            if (!solutions.empty())
            {
                cout << "VALID" << endl;
                for (const auto &solution : solutions)
                {
                    for (auto state : solution)
                        cout << state << " ";
                    cout << endl;
                }
            }
            else
                cout << "INVALID" << endl;
        }
        break;
    }

    default:
        break;
    }

    return 0;
}