#include <set>
#include <map>
#include <stack>
#include <queue>
#include <iostream>
#include <algorithm>

using namespace std;

namespace fa {
    template<typename SymbolType>
    class CNFAutomata {
    private:
        SymbolType emptySymbol, startSymbol;
        set<SymbolType> symbols;
        map<SymbolType, set<SymbolType>> parentOfSymbol;
        map<pair<SymbolType, SymbolType>, set<SymbolType>> parentOfSymbols;

    public:
        CNFAutomata(SymbolType emptySymbol, SymbolType startSymbol) : emptySymbol(emptySymbol),
                                                                      startSymbol(startSymbol) {
            symbols.insert(emptySymbol);
            symbols.insert(startSymbol);
        };

        ~CNFAutomata() = default;

        bool evaluate(const vector<SymbolType> &str) {
            vector<vector<set<char>>> partitions;

            partitions.resize(str.size() + 2);
            for (int lineIndex = 0; lineIndex <= str.size(); lineIndex++)
                partitions[lineIndex].resize(str.size() + 2);

            for (int strIndex = 0; strIndex < str.size(); strIndex++)
                partitions[1][strIndex] = parentOfSymbol[str[strIndex]];

            for (int partitionSize = 2; partitionSize <= str.size(); partitionSize++)
                for (int partitionStart = 0; partitionStart + partitionSize - 1 < str.size(); partitionStart++) {
                    int partitionEnd = partitionStart + partitionSize - 1;
                    for (int delimiterPosition = partitionStart;
                         delimiterPosition <= partitionEnd - 1; delimiterPosition++) {
                        int leftSegmentSize =
                                delimiterPosition - partitionStart + 1; // (partitionStart, delimiterPosition)
                        int rightSegmentSize =
                                partitionEnd - delimiterPosition; // (delimiterPosition + 1, partitionEnd)

                        for (const auto &leftSegmentSymbol: partitions[leftSegmentSize][partitionStart])
                            for (const auto &rightSegmentSymbol: partitions[rightSegmentSize][delimiterPosition + 1])
                                if (parentOfSymbols.find(
                                        pair<SymbolType, SymbolType>(leftSegmentSymbol, rightSegmentSymbol)) !=
                                    parentOfSymbols.end())
                                    for (const auto &symbol: parentOfSymbols[pair<SymbolType, SymbolType>(
                                            leftSegmentSymbol,
                                            rightSegmentSymbol)])
                                        partitions[partitionSize][partitionStart].insert(symbol);
                    }
                }

//            for (int lineIndex = 1; lineIndex <= str.size(); lineIndex++) {
//                for (int columnIndex = 0; columnIndex < str.size(); columnIndex++) {
//                    if (partitions[lineIndex][columnIndex].empty())
//                        cout << "NULL";
//                    else {
//                        for (const auto &partition: partitions[lineIndex][columnIndex])
//                            cout << partition << ",";
//                    }
//                    cout << " ";
//                }
//                cout << endl;
//            }

            return (partitions[str.size()][0].find(startSymbol) != partitions[str.size()][0].end());
        }

        friend istream &operator>>(istream &in, CNFAutomata &automata) {
            int symbolCount;
            SymbolType symbol;

            in >> symbolCount;
            for (int symbolIndex = 1; symbolIndex <= symbolCount; symbolIndex++) {
                in >> symbol;
                automata.symbols.insert(symbol);
            }

            int ruleCount, ruleType;
            SymbolType source, destinationOne, destinationTwo;

            in >> ruleCount;
            for (int ruleIndex = 1; ruleIndex <= ruleCount; ruleIndex++) {
                in >> ruleType >> source;
                if (ruleType != 0 && ruleType != 1)
                    throw (logic_error("Invalid rule type!"));
                if (ruleType == 0) {
                    in >> destinationOne >> destinationTwo;
                    automata.parentOfSymbols[pair<SymbolType, SymbolType>(destinationOne, destinationTwo)].insert(
                            source);
                }
                if (ruleType == 1) {
                    in >> destinationOne;
                    automata.parentOfSymbol[destinationOne].insert(source);
                }
            }

            return in;
        }

        friend ostream &operator<<(ostream &out, const CNFAutomata &automata) {
            out << "Symbols: ";
            for (const auto &symbol: automata.symbols)
                out << symbol << " ";
            out << endl;
            out << "Rules: " << endl;
            for (const auto &entry: automata.parentOfSymbol) {
                out << entry.first << " <- ";
                for (const auto &symbol: entry.second)
                    out << symbol << " ";
                out << endl;
            }
            for (const auto &entry: automata.parentOfSymbols) {
                out << entry.first.first << entry.first.second << " <- ";
                for (const auto &symbol: entry.second)
                    out << symbol << " ";
                out << endl;
            }
            return out;
        }
    };

    template<typename StateType, typename SymbolType>
    class PushdownAutomata {
    private:
        set<StateType> states;
        set<SymbolType> symbols;
        // match, pop, push
        map<StateType, map<StateType, set<tuple<SymbolType, SymbolType, vector<SymbolType>>>>> transitions;
        StateType initialState;
        set<StateType> finalStates;
        SymbolType lambdaSymbol, startSymbol;

        void DFS(int currentState, const vector<SymbolType> &str, int strIndex, stack<SymbolType> &stack, bool &answer,
                 bool previousIsLambda, int firstLambda) {
            if (strIndex == str.size() && finalStates.find(currentState) != finalStates.end()) {
                answer = true;
                return;
            }

            for (const auto &nextState: transitions[currentState])
                for (const auto &transition: nextState.second) {
                    if (answer)
                        return;
                    if (previousIsLambda && nextState.first == firstLambda)
                        continue;
                    if ((get<0>(transition) == lambdaSymbol || get<0>(transition) == str[strIndex])
                        && (get<1>(transition) == lambdaSymbol || (!stack.empty() && stack.top() == str[strIndex]))) {
                        if(get<1>(transition) == lambdaSymbol)
                            stack.pop();
                        for(const auto& pushState: get<2>(transition))
                            stack.push(pushState);
                        if(get<0>(transition) != lambdaSymbol)
                            DFS(nextState.first, str, strIndex + 1, stack, answer, false, -1);
                        else {
                            if(previousIsLambda)
                                DFS(nextState.first, str, strIndex, stack, answer, true, currentState);
                            else
                                DFS(nextState.first, str, strIndex, stack, answer, true, firstLambda);
                        }
                    }
                }
        }

    public:
        PushdownAutomata(SymbolType lambdaSymbol, SymbolType startSymbol) : lambdaSymbol(lambdaSymbol),
                                                                            startSymbol(startSymbol) {
            symbols.insert(lambdaSymbol);
            symbols.insert(startSymbol);
        }

        ~PushdownAutomata() = default;

        bool evaluate(const vector<SymbolType> &str) {
            stack<SymbolType> stack;
            bool answer = false;
            DFS(initialState, str, 0, stack, answer, false, -1);
            return answer;
        }

        friend istream &operator>>(istream &in, PushdownAutomata &automata) {
            int stateCount, startState, endState, finalStateCount;
            StateType state;

            in >> stateCount;
            for (int stateIndex = 1; stateIndex <= stateCount; stateIndex++) {
                in >> state;
                automata.states.insert(state);
            }
            in >> startState;
            automata.initialState = startState;
            in >> finalStateCount;
            for (int stateIndex = 1; stateIndex <= finalStateCount; stateIndex++) {
                in >> state;
                automata.finalStates.insert(state);
            }

            int symbolCount;
            SymbolType symbol;

            in >> symbolCount;
            for (int symbolIndex = 1; symbolIndex <= symbolCount; symbolIndex++) {
                in >> symbol;
                automata.symbols.insert(symbol);
            }

            int transitionCount;
            SymbolType matchSymbol, popSymbol;

            in >> transitionCount;
            for (int transitionIndex = 1; transitionIndex <= transitionCount; transitionIndex++) {
                in >> startState >> endState >> matchSymbol >> popSymbol >> symbolCount;
                vector<SymbolType> pushSymbols;
                for (int symbolIndex = 1; symbolIndex <= symbolCount; symbolIndex++) {
                    in >> symbol;
                    pushSymbols.push_back(symbol);
                }
                if (automata.transitions.find(startState) == automata.transitions.end())
                    automata.transitions.insert(make_pair(startState, map < StateType, set <
                                                                                       tuple<SymbolType, SymbolType, vector<SymbolType>>
                                                                                               >> ()));
                if (automata.transitions[startState].find(endState) == automata.transitions[endState].end())
                    automata.transitions[startState].insert(make_pair(endState, set <
                                                                                tuple<SymbolType, SymbolType, vector<SymbolType>> >
                                                                                {make_tuple(matchSymbol, popSymbol,
                                                                                            pushSymbols)}));
                else
                    automata.transitions[startState][endState].insert(make_tuple(matchSymbol, popSymbol, pushSymbols));
            }
            return in;
        }

        friend ostream &operator<<(ostream &out, const PushdownAutomata &automata) {
            out << "States: ";
            for (const auto &state: automata.states)
                out << state << " ";
            out << endl;

            out << "Initial state: " << automata.initialState << endl;

            out << "Final states: ";
            for (const auto &finalState: automata.finalStates)
                out << finalState << " ";
            out << endl;

            out << "Symbols: ";
            for (const auto &symbol: automata.symbols)
                out << symbol << " ";
            out << endl;

            out << "Transitions: " << endl;
            for (const auto &line: automata.transitions)
                for (const auto &column: line.second)
                    for (const auto &value: column.second) {
                        out << line.first << " - " << column.first << " with " << get<0>(value) << " -> "
                            << get<1>(value) << " ";
                        for (const auto &symbol: get<2>(value))
                            out << symbol;
                        out << endl;
                    }
            return out;
        }
    };
};
