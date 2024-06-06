#include <set>
#include <map>
#include <iostream>

using namespace std;

namespace fa
{
    template <typename StateType, typename TransitionType>
    class FA
    {
    protected:
        set<StateType> states;
        map<StateType, set<pair<StateType, TransitionType>>> transitions;
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
                    for (auto transition : itt->second)
                        if (word[depth] == transition.second)
                        {
                            currentPath.emplace_back(transition);
                            DFS(ok, depth + 1, word, transition.first, solutionPath, currentPath);
                            currentPath.pop_back();
                        }
            }
        }

    public:
        FA(StateType initialState, TransitionType lambdaCharacter, vector<TransitionType> lambdaString) : initialState(initialState), lambdaCharacter(lambdaCharacter), lambdaString(lambdaString) {}

        friend istream &operator>>(istream &in, FA &finiteAutomata)
        {
            StateType stateCount, transitionCount, finalStateCount, x, y;
            TransitionType character;
            in >> stateCount;
            for (int index = 1; index <= stateCount; index++)
            {
                in >> x;
                finiteAutomata.states.insert(x);
            }
            in >> transitionCount;
            for (int index = 1; index <= transitionCount; index++)
            {
                in >> x >> y >> character;
                pair<StateType, TransitionType> transition(y, character);
                if (finiteAutomata.transitions.find(x) == finiteAutomata.transitions.end())
                {
                    auto transitionsSet = set<pair<StateType, TransitionType>>{transition};
                    finiteAutomata.transitions.insert(pair<StateType, set<pair<StateType, TransitionType>>>(x, transitionsSet));
                }
                else
                    finiteAutomata.transitions[x].insert(transition);
            }
            in >> finiteAutomata.initialState;
            in >> finalStateCount;
            for (int index = 1; index <= finalStateCount; index++)
            {
                in >> x;
                finiteAutomata.finalStates.insert(x);
            }
            return in;
        }
        friend ostream &operator<<(ostream &out, FA &finiteAutomata)
        {
            out << "States: ";
            for (int state : finiteAutomata.states)
                out << state << ' ';
            out << endl
                << "Transitions: " << endl;
            for (auto &transition : finiteAutomata.transitions)
                for (auto itv = (transition.second).begin(); itv != (transition.second).end(); itv++)
                    out << "From " << transition.first << " to " << itv->first << " with value " << itv->second << endl;
            out << "Initial state: " << finiteAutomata.initialState << endl;
            out << "Final states: ";
            for (int finalState : finiteAutomata.finalStates)
                out << finalState << " ";
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
        void setStates(set<StateType> states) { states = states; }

        map<StateType, set<pair<StateType, TransitionType>>> getTransitiions() const { return transitions; }
        void setTransitions(map<StateType, set<pair<StateType, TransitionType>>> transitions) { transitions = transitions; }

        StateType getInitialState() const { return initialState; }
        void setInitialState(StateType initialState) { initialState = initialState; }

        set<StateType> getFinalStates() const { return finalStates; }
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

    class NFA : public FA<int, char>
    {
    public:
        NFA(int initialState = 0, char lambdaCharacter = '0', string lambdaString = "-")
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
                    for (auto transition : itt->second)
                    {
                        if (lambdaCharacter == transition.second && lambdaStates.find(transition.first) == lambdaStates.end())
                        {
                            lambdaStates.insert(transition.first);
                            currentPath.emplace_back(transition);
                            DFS(ok, depth, word, transition.first, solutionPaths, currentPath, lambdaStates);
                            currentPath.pop_back();
                        }
                        if (word[depth] == transition.second)
                        {
                            currentPath.emplace_back(transition);
                            DFS(ok, depth + 1, word, transition.first, solutionPaths, currentPath, lambdaStates);
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
    };
};
