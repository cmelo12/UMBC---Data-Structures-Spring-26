// UMBC - CMSC 341 - Spring 2026 - Proj2
#include "streak.h"

Streak::Streak(){
    m_root = nullptr;
    m_gridID = 0;
}

Streak::Streak(int grid, Tiger tigers[], int population){
    m_root = nullptr;
    m_gridID = grid;

}

Streak::~Streak(){
    clear();
    m_root = nullptr;
    m_gridID = 0;
}

/**
 * insert - Inserts a Tiger object into the AVL tree
 * @param tiger: The Tiger object to insert (passed by reference)
 * 
 * This function inserts a Tiger into the proper position based on m_id.
 * - Validates that the id is within MINID-MAXID range
 * - Rejects duplicate ids
 * - Allocates new memory for the Tiger node
 * - Maintains AVL balance after insertion
 */
void Streak::insert(const Tiger& tiger){
    //valid id
    if (tiger.getID() < MINID || tiger.getID() > MAXID)
        return;
    
    //duplicate
    if (search(tiger.getID()) != nullptr)
        return;

    m_root = insertHelper(m_root, tiger);

}//end insert

Tiger* Streak::insertHelper(Tiger* node, const Tiger& tiger){
    if (node == nullptr){
        Tiger * newNode = new Tiger 
        (tiger.getID(), tiger.getAge(), tiger.getGender(), 
        tiger.getState());

        return newNode;
    }//end if 

    //bst traversal 
    if (tiger.getID() < node->getID())
        node->setLeft(insertHelper(node->getLeft(), tiger));
    else if (tiger.getID() > node->getID())
        node->setRight(insertHelper(node->getRight(), tiger));

    //update height and balance tree
    updateHeight(node);
    return balance(node);
    

}//end insertHelper

/**
 * balance - Balances a node if it's unbalanced
 * @param node: The node to balance
 * @return: The new root of the subtree after balancing
 * 
 * Checks the balance factor and performs appropriate rotations:
 * - Left-Left case: Right rotation
 * - Right-Right case: Left rotation
 * - Left-Right case: Left rotation on left child, then right rotation
 * - Right-Left case: Right rotation on right child, then left rotation
 */
Tiger* Streak::balance(Tiger* node){
    if (node == nullptr)
        return nullptr;
    
    int balanceItem = getBalanceItem(node);

    //left heavy 
    if (balanceItem > 1){
        if (getBalanceItem(node->getLeft()) >= 0)
            return rotateRight(node);
        else{
            node->setLeft(rotateLeft(node->getLeft()));
            return rotateRight(node);
        }//end else

    }//end if
    
    //right heavy 
    if (balanceItem < -1){
        if (getBalanceItem(node->getRight()) <= 0)
            return rotateLeft(node);
        else{
            node->setRight(rotateRight(node->getRight()));
            return rotateLeft(node);
        }//end else
    
    }//end if

    return node;

}//end balance

/**
 * getBalanceFactor - Calculates the balance factor of a node
 * @param node: The node to check
 * @return: Balance factor (left height - right height)
 */
int Streak::getBalanceItem(Tiger* node) const {
    if (node == nullptr)
        return 0;
    
    Tiger* rightHeight = 0;
    Tiger* leftHeight = 0;
    int result = 0;

    rightHeight = node->getRight();
    leftHeight = node->getLeft();

    result = leftHeight - rightHeight;

    return result;

}//end getbalanceitem

/**
 * rotateLeft - Performs a left rotation
 * @param node: The node to rotate around
 * @return: The new root of the subtree
 * 
 */
Tiger* Streak::rotateLeft(Tiger* node){
    Tiger* newRoot = node->getRight();
    Tiger* subtree = newRoot->getLeft();

    //rotation
    newRoot->setLeft(node);
    node->setRight(subtree);

    //update height
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}//end rotateLeft

/**
 * rotateRight - Performs a right rotation
 * @param node: The node to rotate around
 * @return: The new root of the subtree
 */
Tiger* Streak::rotateRight(Tiger* node){
    Tiger* newRoot = node->getLeft();
    Tiger* subtree = newRoot->getRight();

    //rotation
    newRoot->setRight(node);
    node->setLeft(subtree);

    //update height
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;

}//end rotateright

/**
 * updateHeight - Updates the height of a node based on its children
 * @param node: The node to update
 */
void Streak::updateHeight(Tiger* node){
    if (node == nullptr)
        return;
    
    int leftHeight = getHeight(node->getLeft());
    int rightHeight = getHeight(node->getRight());
    node->setHeight(1 + max(leftHeight, rightHeight));

}//end updateHeight

/**
 * getHeight - Returns the height of a node
 * @param node: The node to check
 * @return: Height of the node, or -1 if null
 */
int Streak::getHeight(Tiger* node) const{
    if (node == nullptr)
        return -1;
    
    return node->getHeight();
}//end get height

/**
 * search - Searches for a Tiger with the given id
 * @param id: The id to search for
 * @return: Pointer to the Tiger if found, nullptr otherwise
 */
Tiger* Streak::search(int id){
    return searchHelper(m_root, id);

}//end search

/**
 * searchHelper - Recursive helper for search operation
 * @param node: Current node in the traversal
 * @param id: The id to search for
 * @return: Pointer to the Tiger if found, nullptr otherwise
 */
Tiger* Streak::searchHelper(Tiger* node, int id){
    if (node == nullptr)
        return nullptr;
    
    if (id < node->getID())
        return searchHelper(node->getLeft(), id);
    else if (id > node->getID())
        return searchHelper(node->getRight(), id);
    else 
        return node;

}//end search helper

/**
 * remove - Removes a Tiger with the specified id from the tree
 * @param id: The id of the Tiger to remove
 * 
 * This function finds and removes a Tiger node, then rebalances the tree.
 */
void Streak::remove(int id){
    if (id < MINID || id > MAXID)
        return;
    
    m_root = removeHelper(m_root, id);

}//end remove

Tiger* Streak::removeHelper(Tiger* node, int id){
    if (node == nullptr)
        return nullptr;
    
    //traversal
    if (id < node->getID())
        node->setLeft(removeHelper(node->getLeft(), id));
    else if (id > node->getID())
        node->setRight(removeHelper(node->getRight(), id));
    else{
        //node has no leaf
        if (node->getLeft() == nullptr && node->getRight() == nullptr){
            delete node;
            return nullptr;
        }
        //node has only right child
        else if (node->getLeft() == nullptr){
            Tiger* temp = node->getRight();
            delete node;
            return temp;
        }
        //node has only left child
        else if (node->getRight() == nullptr){
            Tiger* temp = node->getLeft();
            delete node;
            return temp;
        }
        //node has 2 children
        else{
            Tiger* successor = findMin(node->getRight());

            //copy data
            node->setID(successor->getID());
            node->setAge(successor->getAge());
            node->setGender(successor->getGender());
            node->setState(successor->getState());

            //remove
            node->setRight(removeHelper(node->getRight(), successor->getID()));
        }
    }//end else

    //update height and balance
    updateHeight(node);
    return balance(node);

}//end removeHelper

/**
 * clear - Deallocates all memory in the tree
 * 
 * Uses post-order traversal to delete all nodes and sets root to nullptr.
 */
void Streak::clear(){
    clearHelper(m_root);
    m_root = nullptr;

}//end clear

/**
 * clearHelper - Recursive helper for clear operation
 * @param node: Current node to delete
 * 
 * Uses post-order traversal (left, right, root) to safely delete all nodes.
 */
void Streak::clearHelper(Tiger* node){
    if (node == nullptr)
        return;
    
    //post order traversal and delete current node
    clearHelper(node->getLeft());
    clearHelper(node->getRight());
    delete node;

}//end clearHelper

int Streak::getSize() const {
    return sizeHelper(m_root);

}//end getsize

/**
 * sizeHelper - Recursive helper for getSize
 * @param node: Current node
 * @return: Number of nodes in the subtree
 */
int Streak::sizeHelper(Tiger* node) const {
    if (node == nullptr)
        return 0;
    
    return 1 + sizeHelper(node->getLeft() + sizeHelper(node->getRight()));

}//end sizehelper

/**
 * listTigers
 * Prints a list of all tigers in the tree to standard output
 * in ascending order of IDs. Each tiger's information is printed
 * on a new line in the format: id:age:gender:state
 */
void Streak::listTigers() const {
    listTigersHelper(m_root);

}//end list tigers

/**
 * listTigersHelper
 * Recursive helper function that performs an in-order traversal
 * of the AVL tree to print tigers in ascending order by ID
 * @param node The current node being visited in the traversal
 */
void Streak::listTigersHelper(Tiger* node) const {
    if (node == nullptr)
        return;
    
    listTigersHelper(node->m_left);
    cout << node->m_id << ":" << node->getAgeStr() << ":"
        << node->getGenderStr() << ":" << node->getStateStr() 
        << endl;
    
    listTigersHelper(node->m_right);

}//end listtigershelper

bool Streak::setState(int id, STATE state){
    Tiger* current = m_root;
    bool flag = false;

    while (current != nullptr){
        if (id == current->m_id){
            current->m_state = state;
            return (!flag);
        }
        else if (id < current->m_id)
            current = current->m_left;
        else 
            current = current->m_right;
    }//end while

    return flag;

}//end set state

/**
 * removeDead
 * Traverses the tree and removes all tigers with DEAD state.
 * The tree remains a balanced AVL tree after all removals.
 */
void Streak::removeDead(){
    int dead = findDead(m_root);
    while (dead != -1){
        remove(dead);
        dead = findDead(m_root);
    }

}//end remove dead

/**
 * findDead
 * Recursive helper that searches for a tiger with DEAD state
 * @param node The current node being visited in the traversal
 * @return The ID of a dead tiger if found, -1 otherwise
 */

 int Streak::findDead(Tiger* node) const{
    if (node == nullptr)
        return -1;
    
    if (node->m_state == DEAD)
        return node->m_id;
    
    int leftResult = findDead(node->m_left);

    if (leftResult != -1)
        return leftResult;
    
    return findDead(node->m_right);

 }//end finddead

bool Streak::findTiger(int id) const {
    Tiger* current = m_root;
    bool found = false;

    if (current == nullptr)
        return found;
    while (id != current->m_id){
        if (id < current->m_id)
            current = current->m_left;
        else 
            current = current->m_right;
        
        if (current == nullptr)    
            return found;
    }//end while

    found = true;
    return (found);

}//end findTiger

/**
 * count
 * Counts all tigers with the given age
 * @param age The age category to count 
 * @return The number of tigers with the specified age
 */
int Streak::count(AGE age) const{
    return ageCounter(m_root, age);

}//end countage

/**
 * ageCounter
 * Recursive function that traverses the tree counting tigers
 * with the given age
 * @param node The current node being visited in the traversal
 * @param age The age category to count
 * @return The count of tigers with specified age in this subtree
 */
int Streak::ageCounter(Tiger* node, AGE age) const{
    int count = 0;

    if (node == nullptr)
        return count;   

    if (node->m_age == age)
        count += 1;
    
    return count + ageCounter(node->m_left, age)
            + ageCounter(node->m_right, age);
            
}//end count age

/**
 * count
 * Counts all tigers with the given state
 * @param state The state category to count 
 * @return The number of tigers with the specified state
 */
int Streak::count(STATE state) const{
   return stateCounter(m_root, state);

}

/**
 * stateCounter
 * Recursive function that traverses the tree counting tigers
 * with the given state
 * @param node The current node being visited in the traversal
 * @param state The state category to count
 * @return The count of tigers with specified state in this subtree
 */
int Streak::stateCounter(Tiger* node, STATE state) const{
    int count = 0;

    if (node == nullptr)
        return count;
    
    if (node->m_state == state)
        count += 1;

    return count + stateCounter(node->m_left, state) 
            + stateCounter(node->m_right, state);

}//end state counter

void Streak::dumpTree() const {dump(m_root);}
void Streak::dump(Tiger* aTiger) const{
    if (aTiger != nullptr){
        cout << "(";
        dump(aTiger->m_left);//first visit the left child
        cout << aTiger->m_id << ":" << aTiger->m_height;//second visit the node itself
        dump(aTiger->m_right);//third visit the right child
        cout << ")";
    }
}

/////////////////////////////////////////////////////////////////
///////////////Grid Class///////////////////////////////////////
/////////////////////////////////////////////////////////////////
/**
 * Grid (default constructor)
 * Initializes an empty splay tree
 */
Grid::Grid(){
    m_root = nullptr;

}//end grid

/**
 * ~Grid (destructor)
 * Deallocates all Streak nodes in the splay tree
 * @pre None
 * @post All memory is freed, m_root is nullptr
 */
Grid::~Grid(){
    clearGrid(m_root);
    m_root = nullptr;

}

/**
 * clearGrid
 * Recursively deallocates all Streak nodes in the splay tree
 * using post-order traversal
 * @param node The current node to delete
 * @pre None
 * @post All nodes in subtree are deallocated
 */
void Grid::clearGrid(Streak* node){
    if (node == nullptr)
        return;
    
    clearGrid(node->m_left);
    clearGrid(node->m_right);
    delete node;

}//end clear grid

/**
 * insert
 * Inserts a new Streak with the given grid ID and tigers into the 
 * splay tree.
 * After insertion, the tree is reorganized via splay operation
 * @param grid The unique grid ID for the new Streak
 * @param tigers Array of Tiger objects to populate the Streak
 * @param population The size of the tigers array
 * @pre None
 * @post If successful, new Streak is at root of splay tree
 * @return true if insertion successful, false if invalid ID or
 * duplicate.
 */
bool Grid::insert(int grid, Tiger tigers[], int population){
    //valid id check
    if (grid < MINID || grid > MAXID)
        return false;
    
    //duplicate check
    if (findStreak(m_root, grid) != nullptr)
        return false;
    
    Streak* newNode = new Streak (grid, tigers, population);

    m_root = insertHelper(m_root, newNode);
    m_root = organize(m_root, grid);

    return true;
    
}//end insert 

/**
 * findStreak
 * Searches for a Streak with the given grid ID (no splay)
 * @param node The current node in traversal
 * @param grid The grid ID to search for
 * @return Pointer to the Streak if found, nullptr otherwise
 */
Streak* Grid::findStreak(Streak* node, int grid){
    if (node == nullptr)
        return nullptr;
    
    if (grid == node->m_gridID)
        return node;
    else if (grid < node->m_gridID)
        return findStreak(node->m_left, grid);
    else    
        return findStreak(node->m_right, grid);
    
}//end findstreak

/**
 * insertHelper
 * Standard BST insertion (no balancing, no splay)
 * @param node The current node in traversal
 * @param newStreak The new Streak node to insert
 * @return The root of the modified subtree
 */
Streak* Grid::insertHelper(Streak* node, Streak* newStreak){
    if (node == nullptr)
        return newStreak;
    
    if (newStreak->m_gridID < node->m_gridID)
        node->m_left = insertHelper(node->m_left, newStreak);
    else if (newStreak->m_gridID > node->m_gridID)
        node->m_right = insertHelper(node->m_right, newStreak);

    return node;
}//end insert helper

/**
 * rotateRight
 * Performs a right rotation on the given node
 * @param node The node to rotate
 * @return The new root of the subtree after rotation
 */
Streak* Grid::rotateRight(Streak* node){
    Streak* leftChild = node->m_left;
    
    node->m_left = leftChild->m_right;
    leftChild->m_right = node;

    return leftChild;

}//end rotate right

/**
 * rotateLeft
 * Performs a left rotation on the given node
 * @param node The node to rotate
 * @return The new root of the subtree after rotation
 */
Streak* Grid::rotateLeft(Streak* node){
    Streak* rightChild = node->m_right;

    node->m_right = rightChild->m_left;
    rightChild->m_left = node;

    return rightChild;
}//end rotate left

/**
 * organize
 * Reorganizes the tree with the most recently accessed
 * node becoming the root through node rotations.
 * @param node The root of the current subtree
 * @param grid The grid ID of the node to splay
 * @return The new root after splaying
 */
Streak* Grid::organize(Streak* node, int grid){
    //node is null of it is at the root
    if (node == nullptr || node->m_gridID == grid)
        return node;
    
    //target in left subtree
    if (grid < node->m_gridID){
        if (node->m_left == nullptr) //target not in tree
            return node;
        
        if (grid < node->m_left->m_gridID){
            node->m_left->m_left = organize(node->m_left->m_left, grid);
            node = rotateRight(node);
        }
        else if (grid > node->m_left->m_gridID){
            node->m_left->m_right = organize(node->m_left->m_right, grid);

            if (node->m_left->m_right != nullptr)
                node->m_left = rotateLeft(node->m_left);
        }
        
        if (node->m_left == nullptr)
            return node;
        else 
            return rotateRight(node);
    }//end outter if
    //target in right subtree
    else {
        if (node->m_right == nullptr)
            return node;
        
        if (grid < node->m_right->m_gridID){
            node->m_right->m_left = organize(node->m_right->m_left, grid);

            if (node->m_right->m_left != nullptr)
                node->m_right = rotateRight(node->m_right);
        }
        else if (grid > node->m_right->m_gridID){
            node->m_right->m_right = organize(node->m_right->m_right, grid);
            node = rotateLeft(node);
        }

        if (node->m_right == nullptr)
            return node;
        else 
            return rotateLeft(node);

    }//end outer else

}//end organize

/**
 * count by state
 * Finds all tigers with a specific state in the specified grid
 * Accessing the grid triggers a splay operation
 * @param grid The grid ID to search in
 * @param state The state to count (ALIVE, DEAD)
 * @pre None
 * @post The accessed Streak is splayed to root
 * @return The count of tigers with specified state, 0 on error
 */
int Grid::count(int grid, STATE state){
    if (grid < MINID || grid > MAXID)
        return 0;
    
    Streak* streak = findStreak(m_root, grid);

    if (streak == nullptr)
        return 0;
    
    m_root = organize(m_root, grid);

    return streak->count(state);

}//end count state

/**
 * count by age
 * Finds all tigers with a specific age in the specified grid
 * Accessing the grid triggers a splay operation
 * @param grid The grid ID to search in
 * @param age The age category to count (CUB, YOUNG, OLD)
 * @pre None
 * @post The accessed Streak is splayed to root
 * @return The count of tigers with specified age, 0 on error
 */
int Grid::count(int grid, AGE age){
    if (grid < MINID || grid > MAXID)//valid id check
        return 0;
    
    //find streak with given grid id
    Streak* streak = findStreak(m_root, grid);

    if (streak == nullptr)
        return 0;
    
    m_root = organize(m_root, grid);

    return streak->count(age);

}//end count age

/**
 * removeTiger
 * Removes tiger(s) from the specified grid.
 * If all is false, removes the specific tiger with the given ID.
 * If all is true, removes all dead tigers from the grid.
 * @param grid The grid ID to remove from.
 * @param tiger The tiger ID to remove (used only if all is false)
 * @param all If true, remove all dead tigers; 
 *        if false, remove specific tiger.
 * @pre None
 * @post The accessed Streak is splayed to root.
 * @return true if operation successful, false on error.
 */
bool Grid::removeTiger(int grid, int tiger, bool all){
    //valid id check 
    if (grid < MINID || grid > MAXID)
        return false;
    
    //find streak with given grid id
    Streak* streak = findStreak(m_root, grid);

    if (streak == nullptr)
        return false;
    
    //splay property
    m_root = organize(m_root, grid);

    if (all){
        //remove all dead tigers
        streak->removeDead();
        return true;
    } else {
        //valid tiger id
        if (tiger < MINID || tiger > MAXID)
            return false;
        
        //tiger exist? if so, remove it 
        if (!streak->findTiger(tiger))
            return false;
        
        streak->remove(tiger);
        
        return true;
    }

}//end removetiger

/**
 * getGridHeight
 * Returns the height of the tree within the specified grid
 * @param grid The grid ID to get height from
 * @pre None
 * @post The accessed Streak is splayed to root
 * @return The height of the Streak's Tiger tree, -1 on error
 */
int Grid::getGridHeight(int grid){
    //valid id change
    if (grid < MINID || grid > MAXID)
        return -1;

    //find streak
    Streak* streak = findStreak(m_root, grid);

    if (streak == nullptr)
        return -1;

    //splay
    m_root = organize(m_root, grid);

    //return height
    if (streak->m_root == nullptr)
        return -1;
    else    
        return streak->m_root->m_height;

}//end getgridheight

/**
 * setState
 * Changes the state of a given tiger in the given grid
 * @param grid The grid ID containing the tiger
 * @param tiger The tiger ID to modify
 * @param state The new state to set (ALIVE, DEAD)
 * @pre None
 * @post The accessed Streak is splayed to root
 * @return true if operation successful, false on error
 */
bool Grid::setState(int grid, int tiger, STATE state){
    // valid grid and tiger id
    if (grid < MINID || grid > MAXID)
        return false;

    if (tiger < MINID || tiger > MAXID)
        return false;

    //find grid
    Streak* streak = findStreak(m_root, grid);

    if (streak == nullptr)
        return false;
    
    //splay
    m_root = organize(m_root, grid);

    return streak->setState(tiger, state);
    
}//end setstate

void Grid::dump(bool verbose) const{
	dumpHelper(m_root, verbose);
    cout << endl;
}

void Grid::dumpHelper(Streak* root, bool verbose) const{
    if (root != nullptr){
        {
            cout << "(";
            dumpHelper( root->m_left, verbose );
            if (verbose)
                cout << root->m_gridID << ":" << root->m_root->m_id;
            else
                cout << root->m_gridID;
            dumpHelper( root->m_right, verbose );
            cout << ")";
        }
    }
}