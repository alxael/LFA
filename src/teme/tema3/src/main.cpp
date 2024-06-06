#include <fstream>
#include <iostream>
#include "fa.hpp"

using namespace std;
using namespace fa;

int main() {
    ifstream cnfaIn("../tests/cnfa.txt");
    ifstream pdaIn("../tests/pda.txt");

    CNFAutomata<char> cnfAutomata('0', 'S');
    cnfaIn >> cnfAutomata;
    cout << "CNF Automata for CYK:" << endl << cnfAutomata << endl;

    PushdownAutomata<int, char> pushdownAutomata('0', '$');
    pdaIn >> pushdownAutomata;
    cout << "Pushdown Automata: " << pushdownAutomata << endl;

    string input;
    int operationType;

    cout << "Evaluate string with CYK using CNF automata - 0" << endl;
    cout << "Evaluate string with pushdown Automata - 1" << endl;
    cin >> operationType;

    cout << "String: " << endl;
    cin >> input;

    if (operationType != 1 && operationType != 0)
        return 0;

    if (operationType == 0)
        cout << cnfAutomata.evaluate(vector<char>(input.begin(), input.end()));
    else
        cout << pushdownAutomata.evaluate(vector<char>(input.begin(), input.end()));
    return 0;
}