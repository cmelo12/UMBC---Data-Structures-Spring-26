//mytest.cpp - test file for Streak and Grid classes
#include "streak.h"
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public: 

    // AVL Tree (Streak) Test Functions
    bool AVLBalanceAfterInsertions();
    bool BSTPropertyAfterInsertions();
    bool AVLBalanceAfterRemovals();
    bool BSTPropertyAfterRemovals();
    bool StreakRemoveMultiple();
    bool StreakFindTigerMultiple();
    
    // Splay Tree (Grid) Test Functions
    bool insertAndSplay();
    bool GridCountWithSplay();
    bool GridSetStateWithSplay();
    bool SplayBSTAfterMultipleOperations();
 
private:
    int getHeightHelper(Tiger* node);
    bool checkHeightsCorrect(Tiger* node);
    bool checkAVLBalance(Tiger* node);
    bool checkBSTProperty(Tiger* node, int minVal, int maxVal);
    bool checkSplayBSTProperty(Streak* node, int minVal, int maxVal);
    void collectInOrder(Tiger* node, vector<int>& ids);
    int countNodes(Tiger* node);
    int countStreakNodes(Streak* node);

};

int main(){
    Tester tester;
    
    cout << "=========AVL Insert Tests=========\n" << endl;

    cout << "AVL Balance after Insert: "
        << (tester.AVLBalanceAfterInsertions() ? "Passed!"
            : "Failed") << endl;
    cout << "BST property after Insert: "
        << (tester.BSTPropertyAfterInsertions() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========AVL Remove Tests=========\n" << endl;

    cout << "AVL Balance after Insert: "
        << (tester.AVLBalanceAfterRemovals() ? "Passed!"
            : "Failed") << endl;
    cout << "BST property after Insert: "
        << (tester.BSTPropertyAfterRemovals() ? "Passed!"
            : "Failed") << endl;
    cout << "Multiple removals test: "
        << (tester.StreakRemoveMultiple() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Find Tiger test=========\n" << endl;

    cout << "Find multiple tigers test: "
        << (tester.StreakFindTigerMultiple() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Splay tree Tests=========\n" << endl;

    cout << "Splay on insert test: "
        << (tester.insertAndSplay() ? "Passed!"
            : "Failed") << endl;
    cout << "Count and Splay test: "
        << (tester.GridCountWithSplay() ? "Passed!"
            : "Failed") << endl;
    cout << "Set State and splay test: "
        << (tester.GridSetStateWithSplay() ? "Passed!"
            : "Failed") << endl;
    cout << "BST property after multiple splays: "
        << (tester.SplayBSTAfterMultipleOperations() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    return 0;    

}//end main

/// Splay tree tests ///

/**
 * insertAndSplay
 * Test whether splay tree performs splay operations when inserting
 * After every insertion, check if inserted node is at root and BST preserved
 * @return true if test passes, false otherwise
 */
bool Tester::insertAndSplay(){
    Grid grid;
    Random randId(MINID, MAXID);
    Random randAge(0, 2);
    Random randGender(0, 2);

    const int INSERTIONS = 20;
    vector<int> insertedGridIds;

    //insert nodes and keep track of ids
    for (int i = 0; i < INSERTIONS; i++){
        //tiger for streak
        Tiger tigers[1];
        tigers[0] = Tiger(randId.getRandNum(),
                static_cast<AGE>(randAge.getRandNum()), 
                static_cast<GENDER>(randGender.getRandNum()));
        
        int gridID = randId.getRandNum();
        //check for duplicate
        bool duplicate = false;

        for (int j = 0; j < (int)insertedGridIds.size(); j++){
            if (insertedGridIds[j] == gridID){
                duplicate = true;
                break;
            }
        }//end inner loop

        if (duplicate == true)
            continue;

        grid.insert(gridID, tigers, 1);
        insertedGridIds.push_back(gridID);

        //the new node should be the root
        if (grid.m_root == nullptr)
            return false;
        
        if (grid.m_root->m_gridID != gridID)
            return false;
        
        //bst property check 
        if (!checkSplayBSTProperty(grid.m_root))
            return false;

    }//end outer for 

    return true;

}//end func

/**
 * testGridCountWithSplay
 * Test whether Grid::count returns correct value
 * and performs splay operation
 * @return true if test passes, false otherwise
 */
bool Tester::GridCountWithSplay(){
    Grid grid;
    Random randId(MINID, MAXID);
    Random randAge(0, 2);
    Random randGender(0, 2);

    const int GRIDS = 20;
    const int TIGERS_BY_GRID = 5;
    vector<int> gridIds;

    //insert grids with tiger nodes
    for (int i = 0; i < GRIDS; i++){
        Tiger tigers[TIGERS_BY_GRID];
        for (int j = 0; j < TIGERS_BY_GRID; j++){
            tigers[j] = Tiger(randId.getRandNum(),
                    static_cast<AGE>(randAge.getRandNum()),
                    static_cast<GENDER>(randGender.getRandNum()), 
                    ALIVE);
        }//inner loop j

        int gridID = randId.getRandNum();
        bool duplicate = false;

        for (int k = 0; k < (int)gridIds.size(); k++){
            if (gridIds[k] == gridID){
                duplicate = true;
                break;
            }
        }//inner loop k
        
        if (duplicate)
            continue;
        
        grid.insert(gridID, tigers, TIGERS_BY_GRID);
        gridIds.push_back(gridID);

    }//end outer for 

    if (gridIds.empty())
        return false;
    
    //access a grid that isn't the root to check
    //splay operation
    int target = gridIds[gridIds.size() / 2];
    int counter = grid.count(target, ALIVE);

    //the accessed node should be the root now
    if (grid.m_root == nullptr)
        return false;
    
    if (grid.m_root->m_gridID != target)
        return false;
    

    //BST property check
    if (!checkSplayBSTProperty(grid.m_root))
        return false;
    
    //count should be at least 0 to be valid
    if (counter < 0)
        return false;

    return true;

}//end func

/**
 * testGridSetStateWithSplay
 * Test whether Grid::setState works correctly and performs splay
 * @return true if test passes, false otherwise
 */
bool Tester::GridSetStateWithSplay(){
    Grid grid;
    Random randId(MINID, MAXID);
    Random randAge(0, 2);
    Random randGender(0, 2);

    const int GRIDS = 10;
    vector<int> tigerIDs;
    vector<int> gridIDs;

    //insert grids with tiger
    for (int i = 0; i < GRIDS; i++){
        int tigerID = randId.getRandNum();
        Tiger tiger[1];
        tiger[0] = Tiger (tigerID,
                    static_cast<AGE>(randAge.getRandNum()),
                    static_cast<GENDER>(randGender.getRandNum()), 
                    ALIVE);
        
        int gridID = randId.getRandNum();
        //duplicate check
        bool duplicate = false;
        
        for (int k = 0; k < (int)gridIDs.size(); k++){
            if (gridIDs[k] == gridID){
                duplicate = true;
                break;
            }
        }//k loop

        if(duplicate)
            continue;
        
        grid.insert(gridID, tiger, 1);
        gridIDs.push_back(gridID);
        tigerIDs.push_back(tigerID);
    }//end outer loop 

    //access non-root grid to check for splay op
    int targetIndex = gridIDs.size() / 2;
    int targetGrid = gridIDs[targetIndex];
    int targetTiger = tigerIDs[targetIndex];

    //check if it changes the state
    bool changedState = grid.setState(targetGrid,
                        targetTiger, DEAD);
    
    if (!changedState)
        return false;
    

    //check if state changed
    int deadCounter = grid.count(targetGrid, DEAD);
    if (deadCounter < 1)
        return false;

    //the node that had the state changed should
    //be the root because of splay property
    if(grid.m_root == nullptr)
        return false;
    
    if (grid.m_root->m_gridID != targetGrid)
        return false;
    
    //check bst property
    if (!checkSplayBSTProperty(grid.m_root))
        return false;
    

    return true;    

}//end fund

/**
 * SplayBSTAfterMultipleOperations
 * Test whether BST property is preserved after multiple splay operations
 * @return true if test passes, false otherwise
 */
bool Tester::SplayBSTAfterMultipleOperations(){
    Grid grid;
    Random randId(MINID, MAXID);
    Random randAge(0, 2);
    Random randGender(0, 2);

    const int GRIDS = 20;
    vector<int> gridIDs;

    //insert grids
    for (int i = 0; i < GRIDS; i++){
        Tiger tigers[1];
        tigers[0] = Tiger(randId.getRandNum(),
                static_cast<AGE>(randAge.getRandNum()), 
                static_cast<GENDER>(randGender.getRandNum()));
        
        int gridID = randId.getRandNum();
        //duplicate check
        bool duplicate = false;

        for (int j = 0; j < (int)gridIDs.size(); j++){
            if (gridIDs[j] == gridID){
                duplicate = true;
                break;
            }
        }

        if (duplicate)
            continue;
        
        grid.insert(gridID, tigers, 1);
        gridIDs.push_back(gridID);
    }//outer loop
    
    //Access different grids 
    for(int i = 0; i < (int)gridIDs.size(); i++){
        grid.count(gridIDs[i], ALIVE);
        
        //check bst property after splay
        if (!checkSplayBSTProperty(grid.m_root))
            return false;
    }

    //access in a different order
    for (int i = gridIDs.size() - 1; i >= 0; i--){
        grid.getGridHeight(gridIDs[i]);

        if (!checkSplayBSTProperty(grid.m_root))
            return false;
    }

    return true;

}//end func

/// AVL tree tests ///

/**
 * AVLBalanceAfterInsertions
 * Test whether AVL trees are balanced after a decent number of insertions
 * Visits all nodes and checks height values are correct
 * @return true if test passes, false otherwise
 */
bool Tester::AVLBalanceAfterInsertions(){
    Streak streak;
    Random randId (MINID, MAXID);
    Random randAge (0, 2);
    Random RandGender (0, 2);

    const int INSERTIONS = 300;

    //insert nodes
    for (int i = 0; i < INSERTIONS; i++){
        int id = randId.getRandNum();
        Tiger tiger(id,
                    static_cast<AGE>(randAge.getRandNum()),
                    static_cast<GENDER>(RandGender.getRandNum()));
        streak.insert(tiger);
    }

    //tree is balanced
    if (!checkAVLBalance(streak.m_root))
        return false;
    
    //correct heights
    if (!checkHeightsCorrect(streak.m_root))
        return false;

    return true;

}//end avlbalance

/**
 * BSTPropertyAfterInsertions
 * Test whether the BST property is preserved after all insertions
 * @return true if test passes, false otherwise
 */
bool Tester::BSTPropertyAfterInsertions(){
    Streak streak;
    Random randId (MINID, MAXID);
    Random randAge (0, 2);
    Random RandGender (0, 2);

    const int INSERTIONS = 300;

    //insert nodes
    for (int i = 0; i < INSERTIONS; i++){
        int id = randId.getRandNum();
        Tiger tiger(id,
                    static_cast<AGE>(randAge.getRandNum()),
                    static_cast<GENDER>(RandGender.getRandNum()));
        streak.insert(tiger);
    }

    //check if tree has bst property and if it's sorted
    if (!checkBSTProperty(streak.m_root))
        return false;
    
    vector<int> ids;
    collectInOrder(streak.m_root, ids);

    for (size_t i = 0; i < ids.size(); i++){
        if (ids[i] <= ids[i - 1])
            return false;
    }

    return true;
}//end

/**
 * AVLBalanceAfterRemovals
 * Test whether AVL trees are balanced after multiple removals
 * Insert database, remove half, check AVL property
 * @return true if test passes, false otherwise
 */
bool Tester::AVLBalanceAfterRemovals(){
    Streak streak;
    Random randId (MINID, MAXID);
    Random randAge (0, 2);
    Random RandGender (0, 2);

    const int INSERTIONS = 300;
    vector<int> insertedIds;

     //insert nodes and keep track of ids
    for (int i = 0; i < INSERTIONS; i++){
        int id = randId.getRandNum();
        Tiger tiger(id,
                    static_cast<AGE>(randAge.getRandNum()),
                    static_cast<GENDER>(RandGender.getRandNum()));
        streak.insert(tiger);

        if (!streak.findTiger(id)){
            streak.insert(tiger);
            insertedIds.push_back(id);
        }
    }

    //remove 1/2 of the nodes
    int removed = insertedIds.size() / 2;

    for (int i = 0; i < removed; i++)
        streak.remove(insertedIds[i]);
    
    //check avl property and heights
    if (!checkAVLBalance(streak.m_root) || 
            !checkHeightsCorrect(streak.m_root));
                return false;
    
    return true;       

}//end func

/**
 * BSTPropertyAfterRemovals
 * Test whether BST property is preserved after multiple removals
 * @return true if test passes, false otherwise
 */
bool Tester::BSTPropertyAfterRemovals(){
    Streak streak;
    Random randId (MINID, MAXID);
    Random randAge (0, 2);
    Random RandGender (0, 2);

    const int INSERTIONS = 300;
    vector<int> insertedIds;

     //insert nodes and keep track of ids
    for (int i = 0; i < INSERTIONS; i++){
        int id = randId.getRandNum();
        Tiger tiger(id,
                    static_cast<AGE>(randAge.getRandNum()),
                    static_cast<GENDER>(RandGender.getRandNum()));
        streak.insert(tiger);

        if (!streak.findTiger(id)){
            streak.insert(tiger);
            insertedIds.push_back(id);
        }
    }

    //remove 1/2 of the nodes
    int removed = insertedIds.size() / 2;

    for (int i = 0; i < removed; i++)
        streak.remove(insertedIds[i]);
    
    //check if tree has bst property and if it's sorted
    if (!checkBSTProperty(streak.m_root))
        return false;
    
    vector<int> ids;
    collectInOrder(streak.m_root, ids);

    for (size_t i = 0; i < ids.size(); i++){
        if (ids[i] <= ids[i - 1])
            return false;
    }

    return true;

}//end func
    
/**
 * StreakRemoveMultiple
 * Test whether Streak::remove works correctly for 50+ removals
 * @return true if test passes, false otherwise
 */
bool Tester::StreakRemoveMultiple(){
    Streak streak;
    Random randId (MINID, MAXID);
    Random randAge (0, 2);
    Random RandGender (0, 2);

    const int INSERTIONS = 300;
    const int REMOVALS = 50;
    vector<int> insertedIds;

     //insert nodes and keep track of ids
    for (int i = 0; i < INSERTIONS; i++){
        int id = randId.getRandNum();
        Tiger tiger(id,
                    static_cast<AGE>(randAge.getRandNum()),
                    static_cast<GENDER>(RandGender.getRandNum()));
        streak.insert(tiger);

        if (!streak.findTiger(id)){
            streak.insert(tiger);
            insertedIds.push_back(id);
        }
    }

    if ((int)insertedIds.size() < REMOVALS)
        return false;

    //remove 50 nodes
    for (int i = 0; i < REMOVALS; i++){
        int removeId = insertedIds[i];

        //check if node exists
        if (!streak.findTiger(removeId))
            return false;
        
        streak.remove(removeId);
        
        //check if node's gone
        if (streak.findTiger(removeId))
            return false;
    }

    //check if the rest of the nodes are still there
    for (int i = REMOVALS; i < (int)insertedIds.size(); i++){
        if (!streak.findTiger(insertedIds[i]))
            return false;    
    }
    
    return true;

}//end func

/**
 * StreakFindTigerMultiple
 * Test whether Streak::findTiger works correctly for 50+ searches
 * @return true if test passes, false otherwise
 */  
bool Tester::StreakFindTigerMultiple(){
    Streak streak;
    Random randId (MINID, MAXID);
    Random randAge (0, 2);
    Random RandGender (0, 2);

    const int INSERTIONS = 100;
    const int SEARCHES = 50;
    vector<int> insertedIds;

     //insert nodes and keep track of ids
    for (int i = 0; i < INSERTIONS; i++){
        int id = randId.getRandNum();
        Tiger tiger(id,
                    static_cast<AGE>(randAge.getRandNum()),
                    static_cast<GENDER>(RandGender.getRandNum()));
        streak.insert(tiger);

        if (!streak.findTiger(id)){
            streak.insert(tiger);
            insertedIds.push_back(id);
        }
    }

    //test finding existing tigers
    for (int i = 0; i < SEARCHES; i++){
        if (!streak.findTiger(insertedIds[i]))
            return false;
    }

    //test finding non existent ids
    for (int i = 0; i < SEARCHES; i++){
        int nonExistID = MINID - 1 - i;
        if (streak.findTiger(nonExistID))
            return false;
    }

    return true;

}//end func

/// Helper Functions Implementations///

/**
 * getHeightHelper
 * Calculates the actual height of a node by traversing its children
 * (not using stored height)
 * @param node The node to calculate height for
 * @return The actual height of the node
 */
int Tester::getHeightHelper(Tiger* node){
    if (node == nullptr)
        return -1;
    
    int leftHeight = getHeightHelper(node->m_left);
    int rightHeight = getHeightHelper(node->m_right);

    if (leftHeight > rightHeight)
        return 1 + leftHeight;
    else    
        return 1 + rightHeight;
    
}//end getheighthelper

/**
 * checkHeightsCorrect
 * Verifies all stored heights match actual heights
 * @param node The root of the subtree to check
 * @return true if all heights are correct, false otherwise
 */
bool Tester::checkHeightsCorrect(Tiger* node){
    if (node == nullptr)
        return true;
    
    int actualHeight = getHeightHelper(node);

    if (node->m_height != actualHeight)
        return false;

    return checkHeightsCorrect(node->m_left) && 
           checkHeightsCorrect(node->m_right);

}//end check

/**
 * checkAVLBalance
 * Verifies AVL balance property at all nodes
 * Balance factor must be -1, 0, or 1
 * @param node The root of the subtree to check
 * @return true if AVL property holds, false otherwise
 */
bool Tester::checkAVLBalance(Tiger* node){  
    if (node == nullptr)
        return true;
    
    int leftHeight = (node->m_left == nullptr) ? -1
                : node->m_left->m_height;
    int rightHeight = (node->m_right == nullptr) ? -1
                : node->m_right->m_height;
    int balance = leftHeight - rightHeight;

    if (balance > 1 || balance < -1)
        return false;

    return checkAVLBalance(node->m_left) && 
            checkAVLBalance(node->m_right);

}//end avlbalance

/**
 * checkBSTProperty
 * Verifies BST property for Tiger tree
 * All left descendants < node < all right descendants
 * @param node The root of the subtree to check
 * @param minVal Minimum valid value for this subtree
 * @param maxVal Maximum valid value for this subtree
 * @return true if BST property holds, false otherwise
 */
bool Tester::checkBSTProperty(Tiger* node, int minVal = MINID - 1, int maxVal = MAXID + 1){
    if (node == nullptr)
        return true;

    if (node->m_id <= minVal || node->m_id >= maxVal)
        return false;
    
    return checkBSTProperty(node->m_left, minVal,node->m_id) &&
         checkBSTProperty(node->m_right, minVal, node->m_id);

}//end bstprop

/**
 * checkSplayBSTProperty
 * Verifies BST property for splay tree
 * @param node The root of the subtree to check
 * @param minVal Minimum valid value for this subtree
 * @param maxVal Maximum valid value for this subtree
 * @return true if BST property holds, false otherwise
 */
bool Tester::checkSplayBSTProperty(Streak* node, int minVal = MINID - 1, int maxVal = MAXID + 1) {
    if (node == nullptr)
        return true;
    
    if (node->m_gridID <= minVal || node->m_gridID >= maxVal)
        return false;
    
    return checkSplayBSTProperty(node->m_left, minVal, node->m_gridID) &&
           checkSplayBSTProperty(node->m_right, node->m_gridID, maxVal);

}//end 

/**
 * collectInOrder
 * Collects tiger IDs in in-order traversal
 * @param node The root of the subtree
 * @param ids Vector to store collected IDs
 */
void Tester::collectInOrder(Tiger* node, vector<int>& ids) {
    if (node == nullptr)
        return;
    
    collectInOrder(node->m_left, ids);
    ids.push_back(node->m_id);
    collectInOrder(node->m_right, ids);

}//endcollect

/**
 * countNodes
 * Counts total nodes in Tiger tree
 * @param node The root of the subtree
 * @return Number of nodes in the subtree
 */
int Tester::countNodes(Tiger* node){
    int count = 0;

    if (node == nullptr)
        return 0;

    count = 1 + countNodes(node->m_left) + countNodes(node->m_right);
    
    return count;

}//end countnodes

/**
 * countStreakNodes
 * Counts total nodes in splay tree
 * @param node The root of the subtree
 * @return Number of nodes in the subtree
 */
int Tester::countStreakNodes(Streak* node){
    if (node == nullptr)
        return 0;
    
    return 1+ countStreakNodes(node->m_left) + countStreakNodes(node->m_right);

}//end 

