// CMSC 341 - Spring 2026 - Project 3
#include "shop.h"

/**
 * @brief Default constructor for Shop.
 * Initializes all member variables to safe default values.
 */
Shop::Shop(){ // empty object 
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = nullptr;
  m_heapType = NOTYPE;
  m_structure = NOSTRUCT;
  m_regPrior = 0;
  m_shopID = 0;

}//end constructor

/**
 * @brief Constructor for Shop.
 * @param priFn Pointer to the priority function used to compute order priorities.
 * @param heapType The type of heap (MINHEAP or MAXHEAP).
 * @param structure The heap structure (SKEW or LEFTIST).
 * @param regPrior The priority value of this shop in the Region.
 * @param id The unique identifier for this shop.
 */
Shop::Shop(prifn_t priFn, HEAPTYPE heapType, 
          STRUCTURE structure, int regPrior,
          int id){
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
    m_regPrior = regPrior;
    m_shopID = id;
    m_heap = nullptr;
    m_size = 0;
}//end default constructor

/**
 * @brief Destructor for Shop.
 * Deallocates all dynamically allocated memory and re-initializes member variables.
 */
Shop::~Shop(){
  clear();
}

/**
 * @brief Clears the heap and re-initializes all member variables.
 * Deletes all nodes in the heap, leaving it empty.
 */
void Shop::clear() {
  clearHeap(m_heap);
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = nullptr;
  m_heapType = NOTYPE;
  m_structure = NOSTRUCT;
  m_regPrior = 0;
  m_shopID = 0;

}//end clear

/**
 * @brief Recursively deallocates all nodes in the heap.
 * @param node Pointer to the current node being deleted.
 */
void Shop::clearHeap(Order* node){
  if (node == nullptr)
    return;
  
  clearHeap(node->m_left);
  clearHeap(node->m_right);
  delete node;

}//end clearHeap

/**
 * @brief Copy constructor for Shop.
 * Creates a deep copy of the rhs object. Functions correctly if rhs is empty.
 * @param rhs The Shop object to copy from.
 */
Shop::Shop(const Shop& rhs){
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;
  m_regPrior = rhs.m_regPrior;
  m_shopID = rhs.m_shopID;
  m_heap = copyHeap(rhs.m_heap);
  m_size = rhs.m_size;

}//end copy constructor

/**
 * @brief Recursively creates a deep copy of a heap subtree.
 * @param node Pointer to the current node being copied.
 * @return Pointer to the root of the newly copied subtree, or nullptr if node is null.
 */
Order* Shop::copyHeap(Order* node){
  if (node == nullptr)
    return nullptr;

  Order* newNode = new Order(*node);
  newNode->m_left = copyHeap(node->m_left);
  newNode->m_right = copyHeap(node->m_right);
  return newNode;

}//end copyheap

/**
 * @brief Assignment operator for Shop.
 * Creates a deep copy of rhs. Handles self-assignment. Does not use the copy constructor.
 * @param rhs The Shop object to assign from.
 * @return Reference to this Shop object.
 */
Shop& Shop::operator=(const Shop& rhs) {
  if (this != &rhs){
    clear();
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_regPrior = rhs.m_regPrior;
    m_shopID = rhs.m_shopID;
    m_heap = copyHeap(rhs.m_heap);
    m_size = rhs.m_size;

  }
  return *this;
  
}//end operator

/**
 * @brief Inserts an Order object into the heap.
 * Treats the new order as a single-node heap and merges it with the existing heap.
 * Maintains min-heap or max-heap property and correct NPL values for leftist heaps.
 * @param order The Order object to insert.
 * @return True if the order was successfully inserted, false if the order
 *         does not provide a valid priority value.
 */
bool Shop::insertOrder(const Order& order) {
  //field validation
  if (order.m_orderID < MINORDERID || order.m_orderID > MAXORDERID)
    return false;
  if (order.m_customerID < MINCUSTID || order.m_customerID > MAXCUSTID)
    return false;
  if (order.m_points < MINPOINTS || order.m_points > MAXPOINTS)
    return false;

  //create new node and merge 
  Order* newNode = new Order(order);
  newNode->m_left = nullptr;
  newNode->m_right = nullptr;
  newNode->m_npl = 0;
  m_heap = mergeHeap(m_heap, newNode);
  m_size++;

  return true;

}//end insertorder

/**
 * @brief Merges two heap subtrees based on the current structure type.
 * Handles both skew heap and leftist heap merge algorithms.
 * @param p1 Pointer to the root of the first subtree.
 * @param p2 Pointer to the root of the second subtree.
 * @return Pointer to the root of the merged heap.
 */
Order* Shop::mergeHeap(Order* p1, Order* p2){
  if (p1 == nullptr)
    return p2;
  if (p2 == nullptr)
    return p1;
  
  //check that p1 has higher priority
  if (m_heapType == MINHEAP){
    if (m_priorFunc(*p1) > m_priorFunc(*p2)){
      Order* temp = p1;
      p1 = p2;
      p2 = temp;
    }
  } else {
    if (m_priorFunc(*p1) < m_priorFunc(*p2)){
      Order* temp = p1;
      p1 = p2;
      p2 = temp;
    }
  }
    
  if (m_structure == SKEW){
    //swap children + merge p2 with left child
    Order* temp = p1->m_left;
    p1->m_left = p1->m_right;
    p1->m_right = temp;
    p1->m_left = mergeHeap(p2, p1->m_left);
  } else {
    //merge to right subtree and fix npl
    p1->m_right = mergeHeap(p1->m_right, p2);
    int leftNpl = (p1->m_left != nullptr) ? p1->m_left->m_npl : -1;
    int rightNpl = (p1->m_right != nullptr) ? p1->m_right->m_npl : -1;
    if (leftNpl < rightNpl){
      Order* temp = p1->m_left;
      p1->m_left = p1->m_right;
      p1->m_right = temp;
    }
    leftNpl = (p1->m_left != nullptr) ? p1->m_left->m_npl : -1;
    rightNpl = (p1->m_right != nullptr) ? p1->m_right->m_npl : -1;
    p1->m_npl = 1 + ((leftNpl < rightNpl) ? leftNpl : rightNpl);

  }

  return p1;

}//end mergeHeap

/**
 * @brief Merges the rhs queue into this queue.
 * Transfers all nodes from rhs to this heap, leaving rhs empty.
 * Both heaps must have the same priority function and data structure.
 * Self-merging is not allowed.
 * @param rhs The Shop queue to merge into this one.
 * @throws domain_error If priority functions or structures aren't the same,
 *             or if self-merging is attempted.
 */
void Shop::mergeWithQueue(Shop& rhs) {
  if (this == &rhs)
    throw domain_error("Can't merge the queue with itself.");
  if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure)
    throw domain_error("Can't merge queues with different structures or priority functions");
  
  m_heap = mergeHeap(m_heap, rhs.m_heap);
  m_size += rhs.m_size;
  rhs.m_heap = nullptr;
  rhs.m_size = 0;
}//end mergeWithQueue

/**
 * @brief Extracts and returns the highest priority order from the queue.
 * Removes the root node and merges its left and right subtrees.
 * Maintains the heap property after removal.
 * @return The highest priority Order object.
 * @throws out_of_range If the queue is empty.
 */
Order Shop::getNextOrder() {
  if (m_heap == nullptr)
    throw out_of_range("Empty queue.");
  
  Order result = *m_heap;
  Order* left = m_heap->m_left;
  Order* right = m_heap->m_right;
  delete m_heap;
  m_heap = mergeHeap(left, right);
  m_size--;

  //clear pointers on the order that got returned
  result.m_left = nullptr;
  result.m_right = nullptr;
  result.m_npl = 0;
  return result;

}//end getnextorder

/**
 * @brief Returns the current number of orders in the queue.
 * @return The number of orders in the heap.
 */
int Shop::numOrders() const{
  return m_size;

}//end numorders

/**
 * @brief Returns the current priority function pointer.
 * @return The priority function used to compute order priorities.
 */
prifn_t Shop::getPriorityFn() const {
  return m_priorFunc;

}//end getPrio

/**
 * @brief Sets a new priority function and heap type, then rebuilds the heap.
 * Reuses existing node memory — does not reallocate nodes.
 * @param priFn Pointer to the new priority function.
 * @param heapType The new heap type (MINHEAP or MAXHEAP).
 */
void Shop::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  m_priorFunc = priFn;
  m_heapType = heapType;
  reorganizeHeap();           

}//setPriorityFn

/**
 * @brief Sets a new data structure (SKEW or LEFTIST) and reorganizes the heap.
 * Transfers nodes from the current structure to the new one without re-creating them.
 * @param structure The new heap structure type.
 */
void Shop::setStructure(STRUCTURE structure){
  m_structure = structure;
  reorganizeHeap();

}//reorganizeHeap

/**
 * @brief Reorganizes the heap by collecting all existing nodes and reinserting them.
 * Reuses current node memory — no new allocations or deallocations.
 */
void Shop::reorganizeHeap(){
  if (m_size == 0)
    return;
  
  int size = m_size;
  Order** nodes = new Order*[size];
  int index = 0;
  collectNodes(m_heap, nodes, index);

  for (int i = 0; i < size; i++){
    nodes[i]->m_left = nullptr;
    nodes[i]->m_right = nullptr;
    nodes[i]->m_npl = 0;
  }

  m_heap = nullptr;
  m_size = 0;

  for (int i = 0; i < size; i++){
    m_heap = mergeHeap(m_heap, nodes[i]);
    m_size++;
  }

  delete [] nodes;

}//end reorgonizeHeap

/**
 * @brief Recursively collects all node pointers in the heap into an array.
 * @param node Pointer to the current node.
 * @param arr Array to store collected node pointers.
 * @param idx Reference to the current index in the array.
 */
void Shop::collectNodes(Order* node, Order** arr, int& index){
  if (node == nullptr)
    return;
  
  arr[index++] = node;
  collectNodes(node->m_left, arr, index);
  collectNodes(node->m_right, arr, index);

}//end collectnodes

/**
 * @brief Returns the current heap type.
 * @return The heap type (MINHEAP or MAXHEAP).
 */
HEAPTYPE Shop::getHeapType() const {
  return m_heapType;

}//getheaptype

/**
 * @brief Returns the current heap structure type.
 * @return The heap structure (SKEW or LEFTIST).
 */
STRUCTURE Shop::getStructure() const {
  return m_structure;
}//endgetstrcture

/**
 * @brief Prints the contents of the queue using preorder traversal.
 * Outputs each order's priority, order ID, item, and count.
 */
void Shop::printOrdersQueue() const {
  cout << "Orders in the Queue: " << endl;
  printPreOrder(m_heap);

}//end print orders

/**
 * @brief Recursively prints nodes in preorder traversal.
 * @param node Pointer to the current node being printed.
 */
void Shop::printPreOrder(Order* node) const {
  if (node == nullptr)
  return;

  cout << "[" << m_priorFunc(*node) << "] "
        << "Order ID: " << node->m_orderID
        << " | Item: " << node->getItemString()
        << " | Count: " << node->getCountString() << endl;

  printPreOrder(node->m_left);
  printPreOrder(node->m_right);

}//end printpreorder

void Shop::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    cout << "Shop " << m_regPrior << ": => ";
    dump(m_heap);
  }
  cout << endl;
}
void Shop::dump(Order *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(*pos) << ":" << pos->m_orderID;
    else
        cout << m_priorFunc(*pos) << ":" << pos->m_orderID << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Order& order) {
  sout << "Order ID: " << order.getOrderID() 
        << ", item: " << order.getItem()
        << ", count: " << order.getCount();
  return sout;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

/**
 * @brief Constructor for Region.
 * Allocates the array-based heap with 1-indexed storage.
 * @param size The maximum number of shops the region can hold.
 */
Region::Region(int size){
  if (size < 0){
    m_capacity = 0;
    m_size = 0;
    m_heap = new Shop[1];
    return;
  }

  m_capacity = size;
  m_size = 0;
  m_heap = new Shop[m_capacity + 1];

}//end dft constrcutor

/**
 * @brief Destructor for Region.
 * Deallocates the heap array.
 */
Region::~Region(){
  delete [] m_heap;
  m_heap = nullptr;
  m_size = 0;
  m_capacity = 0;

}//end destructor

/**
 * @brief Enqueues a Shop object into the min-heap.
 * Places the shop at the end of the array and calls upHeap to restore heap property.
 * Empty Shop objects are allowed.
 * @param aShop The Shop object to insert.
 * @return True if the shop was successfully added, false if the heap is full.
 */
bool Region::addShop(Shop & aShop){
  if (m_size >= m_capacity)
    return false;
  
  //add it to the end and redo heap property
  m_size++;
  m_heap[m_size] = aShop;
  upHeap(m_size);

  return true;

}//end addShop

void Region::upHeap(int index){
  //base case 
  if (index > ROOTINDEX){
    
    //compare with parent and swap if index has higher priority
    if (m_heap[index].m_regPrior < m_heap[index / 2].m_regPrior){
      Shop temp = m_heap[index];
      m_heap[index] = m_heap[index / 2];
      m_heap[index / 2] = temp;
      upHeap(index / 2);
    }//end inner if

  }//outer if

}//end upheap

/**
 * @brief Dequeues the Shop with the highest priority (smallest m_regPrior).
 * Moves the last element to root and calls downHeap to restore heap property.
 * @param aShop Reference to store the dequeued Shop object.
 * @return True if dequeue was successful, false if the heap is empty.
 */
bool Region::getShop(Shop & aShop){
  if (m_size == 0)
  return false;

  //dequeue root
  aShop = m_heap[ROOTINDEX];
  //move last node to root and restore minheap property
  m_heap[ROOTINDEX] = m_heap[m_size];
  m_size--;
  downHeap(ROOTINDEX);

  return true;

}//end getshop

/**
 * @brief Recursively restores min-heap property downward from the given index.
 * Finds the highest priority child and swaps if needed.
 * @param index The index of the node to bubble down.
 */
void Region::downHeap(int index){
  int leftChild = 2 * index;
  int rightChild = 2 * index + 1;
  int highestPriority = index;

  //check if left child has higher priority
  if (leftChild <= m_size && m_heap[leftChild].m_regPrior <   
          m_heap[highestPriority].m_regPrior)
            highestPriority = leftChild;

  if (rightChild <= m_size && m_heap[rightChild].m_regPrior <   
          m_heap[highestPriority].m_regPrior)
            highestPriority = rightChild;

  //if a child has a higher priority, swap and restore heap property
  if (highestPriority != index){
    Shop temp = m_heap[index];
    m_heap[index] = m_heap[highestPriority];
    m_heap[highestPriority] = temp;
    downHeap(highestPriority);
  }

}//end downheap

/**
 * @brief Dequeues the Nth highest priority Shop object.
 * Temporarily removes the first N-1 shops, extracts the Nth,
 * then re-enqueues the removed shops to preserve the heap state.
 * @param aShop Reference to store the Nth highest priority Shop.
 * @param n The rank of the shop to extract (1 = highest priority).
 * @return True if the operation was successful, false if n is out of range.
 */
bool Region::getNthShop(Shop & aShop, int n){
  //validation
  if (n < 1 || n > m_size)
    return false;

  //store (n-1)th shop
  Shop* temp = new Shop[n - 1];

  for (int i = 0; i < n - 1; i++)
    getShop(temp[i]);
  
  //dequeue the nth shop
  getShop(aShop);

  //restore heap property
  for (int i = 0; i < n - 1; i++)
    addShop(temp[i]);

  delete [] temp;

  return true;
}//end getnthshop

/**
 * @brief Changes the priority function and heap type of the Nth highest priority Shop.
 * Temporarily removes the first N-1 shops, modifies the Nth shop's priority function,
 * then re-enqueues all shops to preserve the heap state.
 * @param priFn Pointer to the new priority function.
 * @param heapType The new heap type (MINHEAP or MAXHEAP).
 * @param n The rank of the shop to modify (1 = highest priority).
 * @return True if the operation was successful, false if n is out of range.
 */
bool Region::setPriorityFn(prifn_t priFn, HEAPTYPE heapType, int n){
  //validation
  if (n < 1 || n > m_size)
    return false;

  //store (n-1)th shop
  Shop* temp = new Shop[n - 1];
  for (int i = 0; i < n - 1; i++)
    getShop(temp[i]);

  //dequeue nth shop
  Shop target;
  getShop(target);

  //change priority
  target.setPriorityFn(priFn, heapType);

  //put the shop back + restore the heap
  addShop(target);

  for(int i = 0; i < n - 1; i++)
    addShop(temp[i]);

  delete[] temp;
  
  return true;

}//end setPriorityFn

/**
 * @brief Changes the heap structure of the Nth highest priority Shop.
 * Temporarily removes the first N-1 shops, modifies the Nth shop's structure,
 * then re-enqueues all shops to preserve the heap state.
 * @param structure The new heap structure (SKEW or LEFTIST).
 * @param n The rank of the shop to modify (1 = highest priority).
 * @return True if the operation was successful, false if n is out of range.
 */
bool Region::setStructure(STRUCTURE structure, int n){
  //validate 
  if (n < 1 || n > m_size)
    return false;

  //store (n-1)th shop
  Shop* temp = new Shop[n - 1];
  for (int i = 0; i < n - 1; i++)
    getShop(temp[i]);

  //dequeue nth shop
  Shop target;
  getShop(target);

  //change strcuture
  target.setStructure(structure);

  //enqueue teh nth shop + restore heap
  addShop(target);
  
  for (int i = 0; i < n - 1; i++)
    addShop(temp[i]);
  
  delete [] temp;

  return true;

}//end setstrcuture

/**
 * @brief Dequeues the highest priority Order from the highest priority Shop.
 * Removes and discards any empty Shop objects that have higher priority
 * than the first Shop containing Order objects.
 * After extracting the order, the Shop is re-enqueued to preserve state.
 * @param order Reference to store the dequeued Order object.
 * @return True if an order was successfully dequeued, false if no orders exist.
 */
bool Region::getOrder(Order & order){
  if (m_size == 0)
    return false;
  
  Shop firstShop;
  
  //look for shop with orders
  while (m_size > 0){
    getShop(firstShop);

    if (firstShop.numOrders() > 0){
      //get highest priority order from shop found
      //and put it back
      order = firstShop.getNextOrder();
      addShop(firstShop);
      return true;
    }//end if

  }//end while

  return false;

}//end getorder

void Region::dump(){
    dump(ROOTINDEX);
    cout << endl;
}
void Region::dump(int index){
  if (index <= m_size){
    cout << "(";
    dump(index*2);
    cout << m_heap[index].m_regPrior;
    dump(index*2 + 1);
    cout << ")";
  }
}