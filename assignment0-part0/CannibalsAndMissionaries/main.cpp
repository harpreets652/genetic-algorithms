#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <sstream>

//Configure this parameters
static int boatSize = 2;
//Configuration end

const static char EMPTY_SEAT_ON_BOAT = 'E';

class State {
public:
    int cannibalsOnLeftBank = 0;
    int missionariesOnLeftBank = 0;
    int cannibalsOnRightBank = 0;
    int missionariesOnRightBank = 0;
    char boatPosition; //'L' or 'R'
    State *parent = nullptr;

    State() {}

    State(int cannibalsOnLeftBank, int missionariesOnLeftBank, int cannibalsOnRightBank, int missionariesOnRightBank,
          char boatPosition, State *pParent) : cannibalsOnLeftBank(cannibalsOnLeftBank),
                                               missionariesOnLeftBank(missionariesOnLeftBank),
                                               cannibalsOnRightBank(cannibalsOnRightBank),
                                               missionariesOnRightBank(missionariesOnRightBank),
                                               boatPosition(boatPosition), parent(pParent) {}

    //used to define unique states
    std::string key() {
        std::ostringstream oss;
        oss << cannibalsOnLeftBank << "_" << cannibalsOnRightBank << "_"
            << missionariesOnLeftBank << "_" << missionariesOnRightBank
            << "_" << boatPosition;

        return oss.str();
    }

    State *getParent() {
        return parent;
    }

    void setParent(State *pParent) {
        parent = pParent;
    }

    //whether a state is valid
    bool isValid() {
        return (missionariesOnLeftBank == 0 || missionariesOnLeftBank >= cannibalsOnLeftBank)
               && (missionariesOnRightBank == 0 || missionariesOnRightBank >= cannibalsOnRightBank);
    }

    //is this the goal state
    bool isGoal() {
        return cannibalsOnLeftBank == 0 && missionariesOnLeftBank == 0;
    }

    //find all successor from this state based on boat size
    std::vector<State> getSuccessors() {
        std::vector<State> successors;

        std::vector<char> n;
        std::vector<std::vector<char>> combinations;
        if (boatPosition == 'L') {
            //create vector of chars depending on count of cannibals and missionaries
            generateChoicePool(n, cannibalsOnLeftBank, missionariesOnLeftBank);

            //generate combination of the return crew from the left bank
            nChooseK(combinations, &n[0], static_cast<int>(n.size()), boatSize);

            //create new states based on combination of return crew and validity of new state
            for (const auto &returnCrew : combinations) {
                int leftCannibalCount = cannibalsOnLeftBank;
                int rightCannibalCount = cannibalsOnRightBank;
                int leftMissionaryCount = missionariesOnLeftBank;
                int rightMissionaryCount = missionariesOnRightBank;

                for (auto moving : returnCrew) {
                    switch (moving) {
                        case 'C':
                            leftCannibalCount--;
                            rightCannibalCount++;
                            break;
                        case 'M':
                            leftMissionaryCount--;
                            rightMissionaryCount++;
                            break;
                        case EMPTY_SEAT_ON_BOAT:
                            break;
                        default:
                            std::cerr << "INVALID RETURN CREW COMBINATION" << std::endl;
                            exit(-1);
                    }
                }

                State successor(leftCannibalCount, leftMissionaryCount, rightCannibalCount, rightMissionaryCount, 'R',
                                this);
                if (successor.isValid()) {
                    successor.setParent(this);
                    successors.push_back(successor);
                }
            }
        } else {
            //create vector of chars depending on count of cannibals and missionaries
            generateChoicePool(n, cannibalsOnRightBank, missionariesOnRightBank);

            //generate combination of the return crew from the right bank
            nChooseK(combinations, &n[0], static_cast<int>(n.size()), boatSize);

            //create new states based on combination of return crew and validity of new state
            for (const auto &returnCrew : combinations) {
                int leftCannibalCount = cannibalsOnLeftBank;
                int rightCannibalCount = cannibalsOnRightBank;
                int leftMissionaryCount = missionariesOnLeftBank;
                int rightMissionaryCount = missionariesOnRightBank;

                for (auto moving : returnCrew) {
                    switch (moving) {
                        case 'C':
                            leftCannibalCount++;
                            rightCannibalCount--;
                            break;
                        case 'M':
                            leftMissionaryCount++;
                            rightMissionaryCount--;
                            break;
                        case EMPTY_SEAT_ON_BOAT:
                            break;
                        default:
                            std::cerr << "INVALID RETURN CREW COMBINATION" << std::endl;
                            exit(-1);
                    }
                }

                State successor(leftCannibalCount, leftMissionaryCount, rightCannibalCount, rightMissionaryCount, 'L',
                                this);
                if (successor.isValid()) {
                    successor.setParent(this);
                    successors.push_back(successor);
                }
            }
        }

        return successors;
    }

private:
    void generateChoicePool(std::vector<char> &choices, int numOfCannibals, int numOfMissionaries) {
        for (int i = 0; i < numOfCannibals; ++i) {
            choices.push_back('C');
        }

        for (int i = 0; i < numOfMissionaries; ++i) {
            choices.push_back('M');
        }

        //adding this character to account for less than capacity on boat
        choices.push_back(EMPTY_SEAT_ON_BOAT);

        //just to make sure that when calculating "n choose k" that n is > k
        auto numOfChoices = choices.size();
        for (auto t = numOfChoices; t < boatSize; t++) {
            choices.push_back(EMPTY_SEAT_ON_BOAT);
        }
    }

    //size has to be greater than k by one so pad
    void nChooseK(std::vector<std::vector<char>> &combinations, char *pool, int n, int k) {
        char temp[k];
        combinatorial(combinations, pool, temp, 0, n, 0, k);
    }

    //found this online but changed it to integrate into my code
    void combinatorial(std::vector<std::vector<char>> &combinations, char *pool,
                       char *temp, int start, int end, int pos, int k) {
        if (pos == k) {
            std::vector<char> tempVec;
            for (int j = 0; j < k; j++) {
                tempVec.push_back(temp[j]);
            }

            combinations.push_back(tempVec);
            return;
        }

        for (int i = start; i < end && end - i >= k - pos; i++) {
            temp[pos] = pool[i];
            combinatorial(combinations, pool, temp, i + 1, end, pos + 1, k);

            while (pool[i] == pool[i + 1])
                i++;
        }
    }
};

//method declaration
void bredthFirstSearch();

State breadthFirstSearchImpl(State pState);

void print(State pState);


int main() {
    bredthFirstSearch();

    return 0;
}

void bredthFirstSearch() {
    State initialState(3, 3, 0, 0, 'L', nullptr);

    State pathGoal = breadthFirstSearchImpl(initialState);

    if (pathGoal.getParent() == nullptr) {
        std::cerr << "COULD NOT FIND PATH" << std::endl;
        exit(-1);
    }
}

State breadthFirstSearchImpl(State pState) {
    std::queue<State> fringe;
    std::map<std::string, State> visited;
    fringe.push(pState);

    State evaluate;
    while (!fringe.empty()) {
        evaluate = fringe.front();
        fringe.pop();

        if (visited.find(evaluate.key()) != visited.end()) {
            continue;
        }
        visited[evaluate.key()] = evaluate;

        if (evaluate.isGoal()) {
            std::cout << "FOUND GOAL" << std::endl;
            print(evaluate);
            return evaluate;
        } else {
            print(evaluate);
            for (auto successor : evaluate.getSuccessors()) {
                fringe.push(successor);
            }
        }
    }

    return pState;
}

void print(State pState) {
    std::cout << "Left Bank: {C: " << pState.cannibalsOnLeftBank
              << ", M: " << pState.missionariesOnLeftBank
              << "}, Right bank: {C: " << pState.cannibalsOnRightBank
              << ", M: " << pState.missionariesOnRightBank << "}"
              << ", Boat: " << pState.boatPosition << std::endl;
}
