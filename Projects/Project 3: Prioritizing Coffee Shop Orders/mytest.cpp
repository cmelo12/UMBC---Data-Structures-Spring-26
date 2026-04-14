//mytest.cpp - test file for Shop and Region classes
#include "shop.h"
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
            m_generator = std::mt19937(m_device());
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        int result = 0;
        if(m_type == NORMAL){
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        double result = m_uniReal(m_generator);
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;
    std::uniform_int_distribution<> m_unidist;
    std::uniform_real_distribution<double> m_uniReal;

};

// Priority function 1: uses membership + item + count
// Lower values = higher priority (good for min-heap)
int priorityFn1(const Order& order) {
    return order.getMemebership() + order.getItem() + order.getCount();
}

// Priority function 2: uses points + membership
// Higher values = higher priority (good for max-heap)
int priorityFn2(const Order& order) {
    return order.getPoints() + order.getMemebership() + order.getItem();
}

class Tester{
    public: 

    // Shop Heap Property Tests
    bool insertMinHeapProperty();
    bool insertMaxHeapProperty();
    bool removeMinHeapOrder();
    bool removeMaxHeapOrder();

    // Leftist Heap Tests
    bool leftistNPLCheck();
    bool leftistPropertyCheck();

    // Priority Function Change Test
    bool changePriorityCheck();

    // Merge Tests
    bool mergeEmptyWithNormal();
    bool mergeDifferentPriorityThrows();

    // Copy Constructor Tests
    bool copyConstructorNormal();
    bool copyConstructorEdge();

    // Assignment Operator Tests
    bool assignmentOperatorNormal();
    bool assignmentOperatorEdge();

    // Exception Tests
    bool dequeueEmptyThrows();

private:
    // Check min-heap property on every node
    bool checkMinHeapProperty(Order* node, prifn_t priFn);
    
    // Check max-heap property on every node
    bool checkMaxHeapProperty(Order* node, prifn_t priFn);
    
    // Compute actual NPL and check against stored value
    int computeNPL(Order* node);
    bool checkNPLValues(Order* node);
    
    // Check leftist property (left NPL >= right NPL)
    bool checkLeftistProperty(Order* node);
    
    // Count nodes in a heap
    int countNodes(Order* node);
    
    // Check two heaps are identical in structure and data
    bool checkEqual(Order* a, Order* b, prifn_t priFn);

};

int main(){
    Tester tester;
    
    cout << "=========Min/Max Heap Insert Tests=========\n" << endl;

    cout << "Min-heap property after 300 insertions: "
        << (tester.insertMinHeapProperty() ? "Passed!"
            : "Failed") << endl;
    cout << "Max-heap property after 300 insertions: "
        << (tester.insertMaxHeapProperty() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Min/Max Heap Remove Tests=========\n" << endl;

    cout << "Min-heap removal order correct: "
        << (tester.removeMinHeapOrder() ? "Passed!"
            : "Failed") << endl;
    cout << "Max-heap removal order correct: "
        << (tester.removeMaxHeapOrder() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Leftist Heap Tests=========\n" << endl;

    cout << "Leftist NPL checks: "
        << (tester.leftistNPLCheck() ? "Passed!"
            : "Failed") << endl;
    cout << "Leftist Heap Properties Check: "
        << (tester.leftistPropertyCheck() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Priority Function Change Test=========\n" << endl;

    cout << "Rebuild After Priority Change: "
        << (tester.changePriorityCheck() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Merge Tests=========\n" << endl;

    cout << "Merge Empty Queue with Normal Queue: "
        << (tester.mergeEmptyWithNormal() ? "Passed!"
            : "Failed") << endl;
    cout << "Merge Different Priority Functions (Throws): "
        << (tester.mergeDifferentPriorityThrows() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Copy Constructor Tests=========\n" << endl;

    cout << "Copy Constructor Normal Case: "
        << (tester.copyConstructorNormal() ? "Passed!"
            : "Failed") << endl;
    cout << "Copy Constructor Edge Case (empty): "
        << (tester.copyConstructorEdge() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Assignment Operator Tests=========\n" << endl;

    cout << "Assignment Operator Normal Case: "
        << (tester.assignmentOperatorNormal() ? "Passed!"
            : "Failed") << endl;
    cout << "Assignment Operator Edge Case (empty): "
        << (tester.assignmentOperatorEdge() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    cout << "=========Exception Tests=========\n" << endl;

    cout << "Dequeue empty queue throws out_of_range: "
        << (tester.dequeueEmptyThrows() ? "Passed!"
            : "Failed") << endl;
    cout << endl;

    return 0;    

}//end main

/// Shop Heap Property Tests ///

/**
 * insertMinHeapProperty
 * Insert 300 orders into a min-heap, then traverse and verify
 * that every node's priority <= its children's priorities.
 * @return true if min-heap property holds at every node
 */
bool Tester::insertMinHeapProperty(){
    Shop shop(priorityFn1, MINHEAP, SKEW, 10, 1);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        shop.insertOrder(order);
    }

    //check size
    if (shop.numOrders() != INSERTIONS)
        return false;

    //check min-heap property at every node
    if (!checkMinHeapProperty(shop.m_heap, priorityFn1))
        return false;

    return true;

}//end func

/**
 * insertMaxHeapProperty
 * Insert 300 orders into a max-heap, then traverse and check
 * that every node's priority is correct.
 * @return true if max-heap property holds at every node
 */
bool Tester::insertMaxHeapProperty(){
    Shop shop(priorityFn2, MAXHEAP, SKEW, 10, 2);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        shop.insertOrder(order);
    }

    if (shop.numOrders() != INSERTIONS)
        return false;

    if (!checkMaxHeapProperty(shop.m_heap, priorityFn2))
        return false;

    return true;

}//end func

/**
 * removeMinHeapOrder
 * Insert 300 orders into a min-heap, then remove all of them.
 * Each removal should return an order with min heap priority kept.
 * @return true if all removals happen in non-decreasing priority order
 */
bool Tester::removeMinHeapOrder(){
    Shop shop(priorityFn1, MINHEAP, SKEW, 10, 3);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        shop.insertOrder(order);
    }

    //remove all and check order
    int prevPriority = priorityFn1(shop.getNextOrder());

    for (int i = 1; i < INSERTIONS; i++){
        Order current = shop.getNextOrder();
        int currentPriority = priorityFn1(current);

        //in min-heap, each removal should be >= previous
        if (currentPriority < prevPriority)
            return false;
        
        prevPriority = currentPriority;
    }

    //queue should be empty now
    if (shop.numOrders() != 0)
        return false;

    return true;

}//end func

/**
 * removeMaxHeapOrder
 * Insert 300 orders into a max-heap, then remove all of them.
 * Each removal should return an order with max heap priority kept.
 * @return true if all removals happen in non-increasing priority order
 */
bool Tester::removeMaxHeapOrder(){
    Shop shop(priorityFn2, MAXHEAP, SKEW, 10, 4);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        shop.insertOrder(order);
    }

    //remove all and check order
    int prevPriority = priorityFn2(shop.getNextOrder());

    for (int i = 1; i < INSERTIONS; i++){
        Order current = shop.getNextOrder();
        int currentPriority = priorityFn2(current);

        //in max-heap, each removal should be <= previous
        if (currentPriority > prevPriority)
            return false;
        
        prevPriority = currentPriority;
    }

    if (shop.numOrders() != 0)
        return false;

    return true;

}//end func

/// Leftist Heap Tests ///

/**
 * leftistNPLCheck
 * Insert 300 orders into a leftist heap, then verify that every node
 * has the correct NPL value (1 + min(left NPL, right NPL)).
 * @return true if all NPL values are correct
 */
bool Tester::leftistNPLCheck(){
    Shop shop(priorityFn1, MINHEAP, LEFTIST, 10, 5);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        shop.insertOrder(order);
    }

    //check that every node has correct NPL
    if (!checkNPLValues(shop.m_heap))
        return false;

    return true;

}//end func

/**
 * leftistPropertyCheck
 * Verify leftist heap property: at every node, left child NPL >= right child NPL.
 * Also verify it's not a splay tree.
 * @return true if leftist property holds and tree is not a splay tree
 */
bool Tester::leftistPropertyCheck(){
    Shop shop(priorityFn1, MINHEAP, LEFTIST, 10, 6);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        shop.insertOrder(order);
    }

    //check leftist property: left NPL >= right NPL at every node
    if (!checkLeftistProperty(shop.m_heap))
        return false;

    //verify this is not a splay tree
    if (shop.m_heap != nullptr){
        int leftCount = countNodes(shop.m_heap->m_left);
        int rightCount = countNodes(shop.m_heap->m_right);

        if (leftCount < rightCount)
            return false;
    }

    //also verify min-heap property holds 
    if (!checkMinHeapProperty(shop.m_heap, priorityFn1))
        return false;

    return true;

}//end func

/// Priority Function Change Test ///

/**
 * changePriorityCheck
 * Insert 300 orders with priorityFn1 as min-heap, then switch to
 * priorityFn2 as max-heap. Verify the heap is correctly rebuilt
 * with the same data but the new priority function.
 * @return true if rebuilt heap has correct size and max-heap property
 */
bool Tester::changePriorityCheck(){
    Shop shop(priorityFn1, MINHEAP, SKEW, 10, 7);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        shop.insertOrder(order);
    }

    //verify initial state
    if (shop.numOrders() != INSERTIONS)
        return false;

    //change priority function and heap type
    shop.setPriorityFn(priorityFn2, MAXHEAP);

    //size should be the same
    if (shop.numOrders() != INSERTIONS)
        return false;

    //should now satisfy max-heap property with new function
    if (!checkMaxHeapProperty(shop.m_heap, priorityFn2))
        return false;

    //verify the priority function was actually changed
    if (shop.getPriorityFn() != priorityFn2)
        return false;

    if (shop.getHeapType() != MAXHEAP)
        return false;

    return true;

}//end func

/// Merge Tests ///

/**
 * mergeEmptyWithNormal
 * Create an empty Shop and a normally populated Shop, then merge.
 * The result should contain all orders from the populated shop
 * and the rhs should be empty after the merge.
 * @return true if merge works correctly
 */
bool Tester::mergeEmptyWithNormal(){
    Shop emptyShop(priorityFn1, MINHEAP, SKEW, 10, 8);
    Shop normalShop(priorityFn1, MINHEAP, SKEW, 10, 9);

    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 50;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        normalShop.insertOrder(order);
    }

    //merge empty with normal
    emptyShop.mergeWithQueue(normalShop);

    //empty shop should now have all orders
    if (emptyShop.numOrders() != INSERTIONS)
        return false;

    //normal shop should be empty after merge
    if (normalShop.numOrders() != 0)
        return false;

    //heap property should still hold
    if (!checkMinHeapProperty(emptyShop.m_heap, priorityFn1))
        return false;

    return true;

}//end func

/**
 * mergeDifferentPriorityThrows
 * Attempt to merge two queues with different priority functions.
 * Should throw a domain_error exception.
 * @return true if domain_error is thrown
 */
bool Tester::mergeDifferentPriorityThrows(){
    Shop shop1(priorityFn1, MINHEAP, SKEW, 10, 10);
    Shop shop2(priorityFn2, MAXHEAP, SKEW, 10, 11);

    //insert one order into each
    Order order1(COFFEE, ONE, TIER1, 100, MINCUSTID, MINORDERID);
    Order order2(LATTE, PAIR, TIER2, 200, MINCUSTID + 1, MINORDERID + 1);

    shop1.insertOrder(order1);
    shop2.insertOrder(order2);

    try {
        shop1.mergeWithQueue(shop2);
        //if no exception thrown, test fails
        return false;
    } catch (const domain_error& e){
        //correct exception was thrown
        return true;
    } catch (...){
        //wrong exception type
        return false;
    }

}//end func

/// Copy Constructor Tests ///

/**
 * copyConstructorNormal
 * Create a Shop with 300 orders, copy it, then verify everything is correct
 * @return true if copy is correct
 */
bool Tester::copyConstructorNormal(){
    Shop original(priorityFn1, MINHEAP, LEFTIST, 10, 12);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        original.insertOrder(order);
    }

    //copy constructor
    Shop copy(original);

    //same size
    if (copy.numOrders() != original.numOrders())
        return false;

    //different root pointers
    if (copy.m_heap == original.m_heap)
        return false;

    //same properties
    if (copy.getPriorityFn() != original.getPriorityFn())
        return false;
    if (copy.getHeapType() != original.getHeapType())
        return false;
    if (copy.getStructure() != original.getStructure())
        return false;

    //heap property should hold on the copy
    if (!checkMinHeapProperty(copy.m_heap, priorityFn1))
        return false;

    //check that data matches by dequeuing both
    if (!checkEqual(original.m_heap, copy.m_heap, priorityFn1))
        return false;

    return true;

}//end func

/**
 * copyConstructorEdge
 * Copy an empty Shop. The copy should also be empty.
 * @return true if copy of empty shop works correctly
 */
bool Tester::copyConstructorEdge(){
    Shop original(priorityFn1, MINHEAP, SKEW, 10, 13);

    //copy empty shop
    Shop copy(original);

    if (copy.numOrders() != 0)
        return false;

    if (copy.m_heap != nullptr)
        return false;

    if (copy.getPriorityFn() != original.getPriorityFn())
        return false;

    if (copy.getHeapType() != original.getHeapType())
        return false;

    if (copy.getStructure() != original.getStructure())
        return false;

    return true;

}//end func

/// Assignment Operator Tests ///

/**
 * assignmentOperatorNormal
 * Create a Shop with 300 orders, assign it to another Shop.
 * Verify properties.
 * @return true if assignment works correctly
 */
bool Tester::assignmentOperatorNormal(){
    Shop original(priorityFn1, MINHEAP, LEFTIST, 10, 14);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    const int INSERTIONS = 300;

    for (int i = 0; i < INSERTIONS; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        original.insertOrder(order);
    }

    //assign to a different shop
    Shop assigned(priorityFn2, MAXHEAP, SKEW, 20, 15);
    assigned = original;

    //same size
    if (assigned.numOrders() != original.numOrders())
        return false;

    //deep copy: different root pointers
    if (assigned.m_heap == original.m_heap)
        return false;

    //properties should match original
    if (assigned.getPriorityFn() != original.getPriorityFn())
        return false;
    if (assigned.getHeapType() != original.getHeapType())
        return false;
    if (assigned.getStructure() != original.getStructure())
        return false;

    //heap property should hold
    if (!checkMinHeapProperty(assigned.m_heap, priorityFn1))
        return false;

    //data should match
    if (!checkEqual(original.m_heap, assigned.m_heap, priorityFn1))
        return false;

    return true;

}//end func

/**
 * assignmentOperatorEdge
 * Assign an empty Shop to a populated Shop. The result should be empty.
 * @return true if assignment of empty shop works correctly
 */
bool Tester::assignmentOperatorEdge(){
    Shop populated(priorityFn1, MINHEAP, SKEW, 10, 16);
    Random randPoints(MINPOINTS, MAXPOINTS);
    Random randItem(0, 5);
    Random randCount(0, 3);
    Random randMembership(0, 5);
    Random randCustID(MINCUSTID, MAXCUSTID);
    Random randOrderID(MINORDERID, MAXORDERID);

    //insert some orders
    for (int i = 0; i < 50; i++){
        Order order(
            static_cast<ITEM>(randItem.getRandNum()),
            static_cast<COUNT>(randCount.getRandNum()),
            static_cast<MEMBERSHIP>(randMembership.getRandNum()),
            randPoints.getRandNum(),
            randCustID.getRandNum(),
            randOrderID.getRandNum()
        );
        populated.insertOrder(order);
    }

    //assign empty shop to populated
    Shop emptyShop(priorityFn1, MINHEAP, SKEW, 10, 17);
    populated = emptyShop;

    //populated should now be empty
    if (populated.numOrders() != 0)
        return false;

    if (populated.m_heap != nullptr)
        return false;

    return true;

}//end func

/// Exception Tests ///

/**
 * dequeueEmptyThrows
 * Attempt to dequeue from an empty queue. 
 * Should throw an out_of_range exception.
 * @return true if out_of_range is thrown
 */
bool Tester::dequeueEmptyThrows(){
    Shop shop(priorityFn1, MINHEAP, SKEW, 10, 18);

    try {
        shop.getNextOrder();
        //if no exception thrown, test fails
        return false;
    } catch (const out_of_range& e){
        //correct exception was thrown
        return true;
    } catch (...){
        //wrong exception type
        return false;
    }

}//end func

/// Helper Functions Implementations ///

/**
 * checkMinHeapProperty
 * Recursively verifies that every node's priority is <= its children's priorities.
 * @param node The root of the subtree to check
 * @param priFn The priority function to use
 * @return true if min-heap property holds
 */
bool Tester::checkMinHeapProperty(Order* node, prifn_t priFn){
    if (node == nullptr)
        return true;
    
    int nodePriority = priFn(*node);

    if (node->m_left != nullptr){
        if (nodePriority > priFn(*(node->m_left)))
            return false;
    }

    if (node->m_right != nullptr){
        if (nodePriority > priFn(*(node->m_right)))
            return false;
    }

    return checkMinHeapProperty(node->m_left, priFn) && 
           checkMinHeapProperty(node->m_right, priFn);

}//end checkMinHeapProperty

/**
 * checkMaxHeapProperty
 * Recursively verifies that every node's priority is >= its children's priorities.
 * @param node The root of the subtree to check
 * @param priFn The priority function to use
 * @return true if max-heap property holds
 */
bool Tester::checkMaxHeapProperty(Order* node, prifn_t priFn){
    if (node == nullptr)
        return true;
    
    int nodePriority = priFn(*node);

    if (node->m_left != nullptr){
        if (nodePriority < priFn(*(node->m_left)))
            return false;
    }

    if (node->m_right != nullptr){
        if (nodePriority < priFn(*(node->m_right)))
            return false;
    }

    return checkMaxHeapProperty(node->m_left, priFn) && 
           checkMaxHeapProperty(node->m_right, priFn);

}//end checkMaxHeapProperty

/**
 * computeNPL
 * Computes the actual NPL of a node (shortest path to null).
 * @param node The node to compute NPL for
 * @return The actual NPL value (-1 for null nodes)
 */
int Tester::computeNPL(Order* node){
    if (node == nullptr)
        return -1;
    
    int leftNPL = computeNPL(node->m_left);
    int rightNPL = computeNPL(node->m_right);

    if (leftNPL < rightNPL)
        return 1 + leftNPL;
    else 
        return 1 + rightNPL;

}//end computeNPL

/**
 * checkNPLValues
 * Verifies that every node's stored NPL matches its computed NPL.
 * @param node The root of the subtree to check
 * @return true if all NPL values are correct
 */
bool Tester::checkNPLValues(Order* node){
    if (node == nullptr)
        return true;
    
    int actualNPL = computeNPL(node);

    if (node->m_npl != actualNPL)
        return false;

    return checkNPLValues(node->m_left) && 
           checkNPLValues(node->m_right);

}//end checkNPLValues

/**
 * checkLeftistProperty
 * Verifies that at every node, left child NPL >= right child NPL.
 * @param node The root of the subtree to check
 * @return true if leftist property holds
 */
bool Tester::checkLeftistProperty(Order* node){
    if (node == nullptr)
        return true;
    
    int leftNPL = (node->m_left != nullptr) ? node->m_left->m_npl : -1;
    int rightNPL = (node->m_right != nullptr) ? node->m_right->m_npl : -1;

    if (leftNPL < rightNPL)
        return false;

    return checkLeftistProperty(node->m_left) && 
           checkLeftistProperty(node->m_right);

}//end checkLeftistProperty

/**
 * countNodes
 * Counts the total number of nodes in a subtree.
 * @param node The root of the subtree
 * @return The number of nodes
 */
int Tester::countNodes(Order* node){
    if (node == nullptr)
        return 0;
    
    return 1 + countNodes(node->m_left) + countNodes(node->m_right);

}//end countNodes

/**
 * checkEqual
 * Recursively checks that two heaps have the same structure and
 * the same priority values at each corresponding node.
 * @param a Root of first heap
 * @param b Root of second heap
 * @param priFn Priority function to compare values
 * @return true if heaps are structurally and value-identical
 */
bool Tester::checkEqual(Order* a, Order* b, prifn_t priFn){
    if (a == nullptr && b == nullptr)
        return true;
    
    if (a == nullptr || b == nullptr)
        return false;
    
    //same priority and same order ID
    if (priFn(*a) != priFn(*b))
        return false;
    
    if (a->m_orderID != b->m_orderID)
        return false;

    return checkEqual(a->m_left, b->m_left, priFn) && 
           checkEqual(a->m_right, b->m_right, priFn);

}//end checkEqual