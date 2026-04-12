// UMBC - CMSC 341 - Spring 2026 - Proj1
#include "fuel.h"

FuelSys::FuelSys(){
    m_current = nullptr;
}//end constructor 

FuelSys::~FuelSys(){
    clear();
}//end destrcutor

bool FuelSys::addTank(int tankID, int tankCap, int tankFuel = 0) {
    Tank* newTank = new Tank(tankID, tankCap, tankFuel); 
    bool flag = true;
    
    if (tankCap < MINCAP || tankFuel > tankCap 
            || m_current->m_tankID == tankID){
        flag = false;
        return flag;
    }
    
    if (m_current != nullptr){
        Tank* temp = temp = m_current->m_next;
        
        while (temp != m_current){
            if (temp->m_tankID == tankID){
                flag = false;
                return flag;
            }
            temp = temp->m_next;
        }//end inner while 

    }//end if

    if (m_current == nullptr){
        newTank->m_next = newTank;
        m_current = newTank;
    }else{
        newTank->m_next = m_current->m_next;
        m_current->m_next = newTank;
        m_current = newTank;
    }//end ifelse

    return flag;
}//end addTank

bool FuelSys::removeTank(int tankID){
    // Empty list
    if (m_current == nullptr) {
        return false;
    }

    auto clearPumps = [](Tank* tank) {
        Pump* pumpPtr = tank->m_pumps;
        while (pumpPtr != nullptr) {
            Pump* tempPump = pumpPtr;
            pumpPtr = pumpPtr->m_next;
            delete tempPump;
            tempPump = nullptr;
        } // end while 
        tank->m_pumps = nullptr;
    };

    // Single-node list
    if (m_current->m_next == m_current) {
        if (m_current->m_tankID == tankID) {
            clearPumps(m_current);
            delete m_current;
            m_current = nullptr;
            return true;
        } else {
            return false;
        } // end inner ifelse
    } // end outer if

    // Multi item list
    Tank* prev = m_current;

    while (prev->m_next != m_current && prev->m_next->m_tankID != tankID) {
        prev = prev->m_next;
    } // end while searching

    Tank* target = prev->m_next;

    if (target->m_tankID != tankID) 
        return false;

    prev->m_next = target->m_next;

    if (target == m_current) 
        m_current = target->m_next;

    clearPumps(target);
    delete target;
    target = nullptr;

    return true;

}//end remove tank

bool FuelSys::findTank(int tankID){
    bool flag = true;

    if (m_current == nullptr)
        return (!flag);

    if (m_current->m_next->m_tankID == tankID)
        return flag;
    
    Tank* temp = m_current->m_next;

    while (temp != nullptr){
        if (temp->m_next->m_tankID){
            m_current = temp;
            return flag;
        }//end if
        temp = temp->m_next;

    }//end while
    
    return (!flag);

}//end find tank

bool FuelSys::addPump(int tankID, int pumpID, int targetTank){
    bool flag = true;

    if (m_current == nullptr)
        return (!flag);
    
    Tank* tank = nullptr;
    bool targetExists = false;
    Tank* temp = m_current->m_next;
    
    while (temp != nullptr){
        if (temp->m_tankID == tankID)
            tank = temp;
        if (temp->m_tankID == targetTank)
            targetExists = true;       
    }//end while 

    if (m_current->m_tankID == tankID)
        tank == m_current;

    if(m_current->m_tankID == targetTank)
        targetExists = true;

    if (tank == nullptr || (!targetExists))
        return (!flag);

    Pump* tempPump = tank->m_pumps;

    while(tempPump != nullptr){
        if (tempPump->m_pumpID == pumpID)
            return (!flag);

        tempPump = tempPump->m_next;
    }//end while

    Pump* newPump = new Pump(pumpID, targetTank, tank->m_pumps);
    tank->m_pumps = newPump;

    return flag;    

}//end addpump

bool FuelSys::removePump(int tankID, int pumpID){
    bool flag = true;
    
    if (m_current == nullptr)
        return (!flag);
    
    Tank* tank = nullptr;
    Tank* temp = m_current->m_next;

    //traversing
    while (temp != nullptr){
        if (temp->m_tankID == tankID){
            tank = temp;
            break;
        }//end if
        temp = temp->m_next;

    }//end while

    if (tank == nullptr && m_current->m_tankID == tankID)
        tank = m_current;
    
    if (tank == nullptr)
        return (!flag);

    Pump* previousPump = nullptr;
    Pump* currentPump = tank->m_pumps;

    while (currentPump != nullptr){
        if (currentPump->m_pumpID == pumpID)
            break;
        previousPump = currentPump;
        currentPump = currentPump->m_next;
    }//end while

    if (currentPump == nullptr)
        return (!flag);
    
    if (previousPump == nullptr)
        tank->m_pumps = currentPump->m_next;
    else
        previousPump->m_next = currentPump->m_next;
    
    delete currentPump;
    currentPump = nullptr;

    return flag;

}//end removepump

int FuelSys::totalFuel() const{
    int fuel = 0;

    if (m_current == nullptr)
        return fuel;
    
    Tank* current = m_current;

    do {
        fuel += current->m_tankFuel;
        current = current->m_next;
    } while(current != nullptr);

    return fuel;
    
}//end totalfuel

bool FuelSys::drain(int tankID, int pumpID, int fuel){
    if (!findTank(tankID))
        return false;
    
    Tank* sourceTank = m_current;
    Pump* pump = nullptr;
    Pump* currentPump = sourceTank->m_pumps;

    while (currentPump != nullptr){
        if (currentPump->m_pumpID == pumpID){
            pump = currentPump;
            break;
        }//end if

        currentPump = currentPump->m_next;
    }//end while

    if (pump == nullptr)
        return false;
    
    if (!findTank(pump->m_target))
        return false;
    
    Tank* targetTank = m_current;
    int emptySpace = targetTank->m_tankCapacity - targetTank->m_tankFuel;
    int transferAmount = fuel;
    
    if (transferAmount > sourceTank->m_tankFuel)
        transferAmount = sourceTank->m_tankFuel;
    
    if (transferAmount > emptySpace)
        transferAmount = emptySpace;

    sourceTank->m_tankFuel -= transferAmount;
    sourceTank->m_tankFuel += transferAmount;

    return true;

}//end drain

bool FuelSys::fill(int tankID, int fuel){
    if (!findTank(tankID))
        return false;
    
    Tank* tank = m_current;
    int emptySpace = tank->m_tankCapacity - tank->m_tankFuel;

    if (fuel > emptySpace)
        tank->m_tankFuel = tank->m_tankCapacity;
    else 
        tank->m_tankFuel += fuel;

    return true;

}//end fill

const FuelSys & FuelSys::operator=(const FuelSys & rhs){
    if (this == &rhs)
        return *this;

    if (m_current != nullptr){
        Tank* current = m_current->m_next;
        
        while (current != m_current){
            Tank* nextTank = current->m_next;
            
            Pump* currentPump = current->m_pumps;
            while(current != nullptr){
                Pump* nextPump = currentPump->m_next;
                delete currentPump;
                currentPump = nextPump;
            }//end inner while

            delete current;
            current = nextTank;

        }//end outer while
        
        Pump* thisPump = m_current->m_pumps;

        while (thisPump != nullptr){
            Pump* nextPump = thisPump->m_next;
            delete thisPump;
            thisPump = nextPump;
        }//end while

        delete m_current;
        m_current = nullptr;

    }//end if

    if (rhs.m_current == nullptr)
        return *this;
    
    Tank* currentRhs = rhs.m_current;

    do {
        Tank* newTank = new Tank(currentRhs->m_tankID, currentRhs->m_tankFuel,
                                    currentRhs->m_tankCapacity);
        Pump* rhsPump = currentRhs->m_pumps;
        Pump* lastPump = nullptr;
        while(rhsPump != nullptr){
            Pump* newPump = new Pump (rhsPump->m_pumpID, rhsPump->m_target);

            if (lastPump == nullptr)
                newTank->m_pumps = newPump;
            else
                lastPump->m_next = newPump;
            
            lastPump = newPump;
            rhsPump = rhsPump->m_next;
        }//end while

        if (m_current == nullptr){
            m_current = newTank;
            m_current->m_next = m_current;
        } else {
            Tank* last = m_current;
            while (last->m_next != m_current)
                last = last->m_next;
            
            last->m_next = newTank;
            newTank->m_next = m_current;
        }//end if else

        currentRhs = currentRhs->m_next;

    }while(currentRhs != rhs.m_current);

    return *this;
    
}//end operator=

void FuelSys::dumpSys() const{
    if (m_current != nullptr){
        Tank* tempTank = m_current->m_next;//we start at front item
        //we continue visting nodes until we reach the cursor
        while(m_current != nullptr && tempTank != m_current){
            cout << "Tank " << tempTank->m_tankID << "(" << tempTank->m_tankFuel << " kg)" << endl;
            // now dump the targets for all pumps in this tank
            // we need to traverse the list of pumps
            dumpPumps(tempTank->m_pumps);
            tempTank = tempTank->m_next;
        }
        //at the end we visit the cursor (current)
        //this also covers the case that there is only one item
        cout << "Tank " << m_current->m_tankID << "(" << m_current->m_tankFuel << " kg)" << endl;
        dumpPumps(tempTank->m_pumps);
        cout << "The current tank is " << m_current->m_tankID << endl;
    }
    else
        cout << "There is no tank in the system!\n\n";
}

void FuelSys::dumpPumps(Pump* pumps) const{
    // this traverses the linked list to the end
    Pump* tempPump = pumps;
    while (tempPump != nullptr){
        cout << " => pump " << tempPump->m_pumpID << "(To tank " << tempPump->m_target << ")" << endl;
        tempPump = tempPump->m_next;
    }
}

void FuelSys::clear(){ //private helper to clear memory
    Tank* temp = m_current->m_next; 

    if (m_current == nullptr)
        return;
    
    while (temp != nullptr){
        Pump * pump = temp->m_pumps;

        while (pump != nullptr){
            Pump* tempNextPump = pump->m_next;
            delete pump;
            pump = tempNextPump;
        }//end inner while

        Tank* tempNextTank = temp->m_next;
        delete temp;
        temp = tempNextTank;

    }//end while

    Pump* pump = pump = m_current->m_pumps;

    while(pump != nullptr){
        Pump* nextPump = nextPump = pump->m_next;
        delete pump;
        pump = nextPump;
    }//end while 

    delete m_current;
    m_current = nullptr;

}//end clear