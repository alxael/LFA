#include <set>
#include <map>
#include <queue>
#include <iostream>

using namespace std;

namespace fa
{
    template <typename StateType, typename TransitionType>
    class FA
    {
    protected:
        set<StateType> states;
        map<StateType, map<StateType, set<TransitionType>>> transitions;
        StateType initialState;
        set<StateType> finalStates;
        TransitionType lambdaCharacter;
        vector<TransitionType> lambdaString;

        void DFS(bool &ok, int depth, const vector<TransitionType> &word, StateType currentState,
                 vector<pair<StateType, TransitionType>> &solutionPath, vector<pair<StateType, TransitionType>> &currentPath) const
        {
            if (depth == word.size() && finalStates.find(currentState) != finalStates.end())
            {
                ok = true;
                solutionPath = currentPath;
            }
            else
            {
                auto itt = transitions.find(currentState);
                if (itt != transitions.end())
                    for (auto it = (itt->second).begin(); it != (itt->second).end(); it++)
                        if ((it->second).find(word[depth]) != (it->second).end())
                        {
                            currentPath.emplace_back(make_pair(it->first, word[depth]));
                            DFS(ok, depth + 1, word, it->first, solutionPath, currentPath);
                            currentPath.pop_back();
                        }
            }
        }

    public:
        FA(StateType initialState, TransitionType lambdaCharacter, vector<TransitionType> lambdaString) : initialState(initialState), lambdaCharacter(lambdaCharacter), lambdaString(lambdaString) {}

        friend istream &operator>>(istream &in, FA &fa)
        {
            StateType stateCount, transitionCount, finalStateCount, x, y;
            TransitionType character;
            in >> stateCount;
            for (int index = 1; index <= stateCount; index++)
            {
                in >> x;
                fa.addState(x);
            }
            in >> transitionCount;
            for (int index = 1; index <= transitionCount; index++)
            {
                in >> x >> y >> character;
                fa.addTransition(x, y, character);
            }
            in >> fa.initialState;
            in >> finalStateCount;
            for (int index = 1; index <= finalStateCount; index++)
            {
                in >> x;
                fa.finalStates.insert(x);
            }
            return in;
        }
        friend ostream &operator<<(ostream &out, FA &fa)
        {
            out << "States: ";
            for (auto its = fa.states.begin(); its != fa.states.end(); its++)
                out << *its << ' ';
            out << endl
                << "Transitions: " << endl;
            for (auto itt = fa.transitions.begin(); itt != fa.transitions.end(); itt++)
                for (auto itv = (itt->second).begin(); itv != (itt->second).end(); itv++)
                {
                    out << "From " << itt->first << " to " << itv->first << " with values:  ";
                    for (auto itc = (itv->second).begin(); itc != (itv->second).end(); itc++)
                        cout << *itc << " ";
                    cout << endl;
                }

            out << "Initial state: " << fa.initialState << endl;
            out << "Final states: ";
            for (auto itf = fa.finalStates.begin(); itf != fa.finalStates.end(); itf++)
                out << *itf << " ";
            out << endl;
            return out;
        }
        bool evaluateString(const vector<TransitionType> &str) const
        {
            if (str == lambdaString)
                return (finalStates.find(initialState) != finalStates.end());

            bool ok = false;
            vector<pair<StateType, TransitionType>> current, solution;
            DFS(ok, 0, str, initialState, solution, current);
            return ok;
        }
        vector<StateType> evaluateStringWithPath(const vector<TransitionType> &str) const
        {
            vector<StateType> answer;
            if (str == lambdaString)
                return (finalStates.find(initialState) != finalStates.end()) ? vector<StateType>{initialState} : vector<StateType>{};

            bool ok = false;
            vector<pair<StateType, TransitionType>> current, solution;
            DFS(ok, 0, str, initialState, solution, current);
            if (!ok)
                return answer;
            answer.emplace_back(initialState);
            for (auto transition : solution)
                answer.emplace_back(transition.first);
            return answer;
        }

        set<StateType> getStates() const { return states; }
        void addState(StateType newState) { states.insert(newState); }
        void setStates(set<StateType> states) { states = states; }

        map<StateType, set<pair<StateType, TransitionType>>> getTransitiions() const { return transitions; }
        void addTransition(StateType startState, StateType endState, TransitionType character)
        {
            if (states.find(startState) == states.end())
                throw(runtime_error("Could not find start state."));
            if (states.find(endState) == states.end())
                throw(runtime_error("Could not find end state."));
            if (transitions.find(startState) == transitions.end())
            {
                map<StateType, set<TransitionType>> transitionsMap;
                transitionsMap.insert(pair<StateType, set<TransitionType>>(endState, set<TransitionType>{character}));
                transitions.insert(pair<StateType, map<StateType, set<TransitionType>>>(startState, transitionsMap));
            }
            else
            {
                auto transitionsMap = transitions[startState];
                if (transitionsMap.find(endState) == transitionsMap.end())
                    transitions[startState].insert(pair<StateType, set<TransitionType>>(endState, set<TransitionType>{character}));
                else
                    transitions[startState][endState].insert(character);
            }
        }
        void setTransitions(map<StateType, set<pair<StateType, TransitionType>>> transitions) { transitions = transitions; }

        StateType getInitialState() const { return initialState; }
        void setInitialState(StateType initialState) { initialState = initialState; }

        set<StateType> getFinalStates() const { return finalStates; }
        void addFinalState(StateType newState) { finalStates.insert(newState); }
        void setFinalStates(set<StateType> finalStates) { finalStates = finalStates; }

        TransitionType getLambdaCharacter() const { return lambdaCharacter; }
        void setLambdaCharacter(TransitionType lambdaCharacter) { lambdaCharacter = lambdaCharacter; }

        vector<TransitionType> getLambdaString() const { return lambdaString; }
        void setLambdaString(vector<TransitionType> lambdaString) { lambdaString = lambdaString; }
    };

    class DFA : public FA<int, char>
    {
    public:
        DFA(int initialState = 0, char lambdaCharacter = '0', string lambdaString = "-")
            : FA<int, char>(initialState, lambdaCharacter, vector<char>(lambdaString.begin(), lambdaString.end())) {}

        bool evaluateString(const string &str) const
        {
            vector<char> strVector(str.begin(), str.end());
            return FA<int, char>::evaluateString(strVector);
        }

        vector<int> evaluateStringWithPath(const string &str) const
        {
            vector<char> strVector(str.begin(), str.end());
            return FA<int, char>::evaluateStringWithPath(strVector);
        }
    };

    class LambdaNFA : public FA<int, char>
    {
    protected:
        void DFS(bool &ok, int depth, const vector<char> &word, int currentState, vector<vector<pair<int, char>>> &solutionPaths,
                 vector<pair<int, char>> &currentPath, set<int> &lambdaStates) const
        {
            if (depth == word.size() && finalStates.find(currentState) != finalStates.end())
            {
                ok = true;
                solutionPaths.emplace_back(currentPath);
            }
            else
            {
                auto itt = transitions.find(currentState);
                if (itt != transitions.end())
                    for (auto it = (itt->second).begin(); it != (itt->second).end(); it++)
                    {
                        if ((it->second).find(lambdaCharacter) != (it->second).end() && lambdaStates.find(it->first) == lambdaStates.end())
                        {
                            lambdaStates.insert(it->first);
                            currentPath.emplace_back(pair<int, char>(it->first, lambdaCharacter));
                            DFS(ok, depth, word, it->first, solutionPaths, currentPath, lambdaStates);
                            currentPath.pop_back();
                        }
                        if ((it->second).find(word[depth]) != (it->second).end())
                        {
                            currentPath.emplace_back(pair<int, char>(it->first, word[depth]));
                            DFS(ok, depth + 1, word, it->first, solutionPaths, currentPath, lambdaStates);
                            currentPath.pop_back();
                        }
                    }
            }
        }

    public:
        LambdaNFA(int initialState = 0, char lambdaCharacter = '0', string lambdaString = "-")
            : FA<int, char>(initialState, lambdaCharacter, vector<char>(lambdaString.begin(), lambdaString.end())) {}

        bool evaluateString(const string &str) const
        {
            vector<char> strVector(str.begin(), str.end());
            if (strVector == lambdaString)
                return (finalStates.find(initialState) != finalStates.end());

            bool ok = false;
            vector<pair<int, char>> current;
            vector<vector<pair<int, char>>> solutions;
            set<int> lambda;
            DFS(ok, 0, vector<char>(str.begin(), str.end()), initialState, solutions, current, lambda);
            return ok;
        }

        vector<vector<int>> evaluateStringWithPath(const string &str) const
        {
            vector<char> strVector(str.begin(), str.end());
            vector<vector<int>> answer;
            if (strVector == lambdaString)
                return (finalStates.find(initialState) != finalStates.end()) ? vector<vector<int>>{
                                                                                   vector<int>{initialState}}
                                                                             : vector<vector<int>>{};

            bool ok = false;
            vector<pair<int, char>> current;
            vector<vector<pair<int, char>>> solutions;
            set<int> lambda;
            DFS(ok, 0, strVector, initialState, solutions, current, lambda);
            if (!ok)
                return answer;

            for (const auto &solution : solutions)
            {
                vector<int> path{initialState};
                for (auto transition : solution)
                    path.emplace_back(transition.first);
                answer.emplace_back(path);
            }

            return answer;
        }

        DFA turnDeterministic() const
        {
            DFA automata;

            map<int, map<char, set<int>>> nondeterministicTransitionTable;
            for (auto itt = transitions.begin(); itt != transitions.end(); itt++)
            {
                nondeterministicTransitionTable.insert(pair<int, map<char, set<int>>>(itt->first, map<char, set<int>>()));
                for (auto it = (itt->second).begin(); it != (itt->second).end(); it++)
                    for (auto itc = (it->second).begin(); itc != (it->second).end(); itc++)
                        if (nondeterministicTransitionTable[itt->first].find(*itc) == nondeterministicTransitionTable[itt->first].end())
                            nondeterministicTransitionTable[itt->first].insert(pair<char, set<int>>(*itc, set<int>{it->first}));
                        else
                            nondeterministicTransitionTable[itt->first][*itc].insert(it->first);
            }

            map<set<int>, map<char, set<int>>> deterministicTransitionTable;

            queue<set<int>> stateQueue;
            stateQueue.push(set<int>{initialState});

            map<set<int>, int> generatedStateEncoding;
            int encodingIndex = 0;

            while (!stateQueue.empty())
            {
                set<int> currentState = stateQueue.front();
                generatedStateEncoding.insert(pair<set<int>, int>(currentState, encodingIndex));
                automata.addState(encodingIndex);
                encodingIndex++;
                stateQueue.pop();

                map<char, set<int>> currentStateTransitions;
                for (auto its = currentState.begin(); its != currentState.end(); its++)
                    for (auto it = nondeterministicTransitionTable[*its].begin(); it != nondeterministicTransitionTable[*its].end(); it++)
                        if (currentStateTransitions.find(it->first) == currentStateTransitions.end())
                            currentStateTransitions.insert(pair<char, set<int>>(it->first, it->second));
                        else
                            currentStateTransitions[it->first].insert((it->second).begin(), (it->second).end());

                for (auto it = currentStateTransitions.begin(); it != currentStateTransitions.end(); it++)
                    if (generatedStateEncoding.find(it->second) == generatedStateEncoding.end())
                        stateQueue.push(it->second);

                deterministicTransitionTable.insert(pair<set<int>, map<char, set<int>>>(currentState, currentStateTransitions));
            }

            automata.setInitialState(generatedStateEncoding[set<int>{initialState}]);
            for (auto itt = deterministicTransitionTable.begin(); itt != deterministicTransitionTable.end(); itt++)
                for (auto it = (itt->second).begin(); it != (itt->second).end(); it++)
                    automata.addTransition(generatedStateEncoding[itt->first], generatedStateEncoding[it->second], it->first);
            for (auto ite = generatedStateEncoding.begin(); ite != generatedStateEncoding.end(); ite++)
                for (auto itf = finalStates.begin(); itf != finalStates.end(); itf++)
                    if ((ite->first).find(*itf) != (ite->first).end())
                        automata.addFinalState(ite->second);

            return automata;
        }
    };
};
