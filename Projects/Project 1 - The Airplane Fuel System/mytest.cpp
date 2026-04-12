#include "fuel.h"

class Tester{
    public:
    bool addTankNormal();
    bool addTankError();
    bool removeTankNormal();
    bool removeTankError();
    bool findTankNormal();
    bool findTankError();
    bool totalFuelNormal();
    bool totalFuelError();
    bool addPumpNormal();
    bool addPumpError();
    bool removePumpNormal();
    bool removePumpError();
    bool drainNormal();
    bool drainError();
    bool overloadedOperator();

};

int main(){
    Tester test;
    
cout << "=========Add Tank Tests=========\n" << endl;

cout << "Add Tank Normal Test: " 
    << (test.addTankNormal() ? "Passed!" : "Failed") << endl;
cout << "Add Tank Error Test: " 
    << (test.addTankNormal() ? "Passed!" : "Failed") << endl;
cout << endl;

cout << "=========Remove Tank Tests=========\n" << endl;

cout << "Remove Tank Normal Test: " 
    << (test.removeTankNormal() ? "Passed!" : "Failed") << endl;
cout << "Remove Tank Error Test: " 
    << (test.removeTankError() ? "Passed!" : "Failed") << endl;
cout << endl;

cout << "=========Find Tank Tests=========\n" << endl;

cout << "Find Tank Normal Test: " 
    << (test.findTankNormal() ? "Passed!" : "Failed") << endl;
cout << "Find Tank Error Test: " 
    << (test.findTankError() ? "Passed!" : "Failed") << endl;
cout << endl;

cout << "=========Total Fuel Tests=========\n" << endl;

cout << "Total Fuel Normal Test: " 
    << (test.findTankNormal() ? "Passed!" : "Failed") << endl;
cout << "Total Fuel Error Test: " 
    << (test.findTankError() ? "Passed!" : "Failed") << endl;
cout << endl;

cout << "=========Add Pump Tests=========\n" << endl;

cout << "Add Pump Normal Test: " 
    << (test.addPumpNormal() ? "Passed!" : "Failed") << endl;
cout << "Add Tank Error Test: " 
    << (test.addPumpError() ? "Passed!" : "Failed") << endl;
cout << endl;

cout << "=========Remove Pump Tests=========\n" << endl;

cout << "Remove Pump Normal Test: " 
    << (test.removePumpNormal() ? "Passed!" : "Failed") << endl;
cout << "Remove Pump Error Test: " 
    << (test.removePumpError() ? "Passed!" : "Failed") << endl;
cout << endl;

cout << "=========Drain Tests=========\n" << endl;

cout << "Drain Normal Test: " 
    << (test.drainNormal() ? "Passed!" : "Failed") << endl;
cout << "Drain Error Test: " 
    << (test.drainNormal() ? "Passed!" : "Failed") << endl;
cout << endl;

}//end main

bool Tester::drainNormal(){
    FuelSys fuel;
    bool drain = true;

    fuel.addTank(1, 2000, 1000);  
    fuel.addTank(2, 2000, 500);   
    fuel.addPump(1, 10, 2); 

    // Case 1:requested fuel (300) < empty space of target (1500) all the fuel transfers
    drain = drain && fuel.drain(1, 10, 300);
    drain = drain && (fuel.m_current->m_next->m_tankFuel == 700);//1000-300=700 
    drain = drain && fuel.findTank(2);
    drain = drain && (fuel.m_current->m_next->m_tankFuel == 800);//500+300=800

    // Case 2: requested fuel (1000) > empty space of target (1200) only fills up to capacity
    fuel.addTank(3, 2000, 900);   // tank 3 empty space = 1100(2000-900)
    fuel.addPump(2, 20, 3);
    drain = drain && fuel.drain(2, 20, 1000);
    drain = drain && fuel.findTank(2);
    drain = drain && (fuel.m_current->m_next->m_tankFuel == 0);     
    drain = drain && fuel.findTank(3);
    drain = drain && (fuel.m_current->m_next->m_tankFuel == 2000);

    return drain;

}//end drain normal

bool Tester::drainError(){
    FuelSys fuel;
    bool error = true;

    fuel.addTank(1, 2000, 1000);
    fuel.addTank(2, 2000, 500);
    fuel.addPump(1, 10, 2);

    //source tank does not exist
    error = error && !(fuel.drain(99, 10, 300));

    //pump does not exist on the source tank
    error = error && !(fuel.drain(1, 99, 300));

    //destination tank does not exist
    fuel.addPump(1, 20, 12);  
    error = error && !(fuel.drain(1, 20, 300));

    return error;

}//end drain error

bool Tester::overloadedOperator() {
    FuelSys fuel;
    bool flag = true;

    // Build a system with 10 tanks and 10 pumps
    fuel.addTank(1,  2000, 1000);
    fuel.addTank(2,  2000, 500);
    fuel.addTank(3,  2000, 300);
    fuel.addTank(4,  2000, 200);
    fuel.addTank(5,  2000, 100);
    fuel.addTank(6,  2000, 600);
    fuel.addTank(7,  2000, 700);
    fuel.addTank(8,  2000, 800);
    fuel.addTank(9,  2000, 900);
    fuel.addTank(10, 2000, 1100);

    fuel.addPump(1,  10, 2);
    fuel.addPump(2,  20, 3);
    fuel.addPump(3,  30, 4);
    fuel.addPump(4,  40, 5);
    fuel.addPump(5,  50, 6);
    fuel.addPump(6,  60, 7);
    fuel.addPump(7,  70, 8);
    fuel.addPump(8,  80, 9);
    fuel.addPump(9,  90, 10);
    fuel.addPump(10, 100, 1);

    // new FuelSys object
    FuelSys copy;
    copy = fuel;

    //check all tanks exist in the copy
    flag = flag && copy.findTank(1);
    flag = flag && copy.findTank(2);
    flag = flag && copy.findTank(3);
    flag = flag && copy.findTank(4);
    flag = flag && copy.findTank(5);
    flag = flag && copy.findTank(6);
    flag = flag && copy.findTank(7);
    flag = flag && copy.findTank(8);
    flag = flag && copy.findTank(9);
    flag = flag && copy.findTank(10);

    // Check total fuel matches
    flag = flag && (copy.totalFuel() == fuel.totalFuel());

    // Verify copy after adding something new to original
    fuel.addTank(11, 2000, 999);
    flag = flag && !(copy.findTank(11));  

    // self-assignment
    copy = copy;
    flag = flag && copy.findTank(1);
    flag = flag && (copy.totalFuel() == 5500);  //sum of all tanks = 7200

    return flag;
    
}// end assignmentOperator


bool Tester::removePumpError(){
    FuelSys fuel;
    bool error = true;

    fuel.addTank(1, 1000, 500);
    fuel.addTank(2, 1000, 400);

    fuel.addPump(1, 10, 2);

    //removing a non existent pump
    error = error && !(fuel.removePump(1, 20));

    //removing pump from non existent tank
    error = error && !(fuel.removePump(12, 10));
    
    return error;

}//end remove pump error

bool Tester::removePumpNormal(){
    FuelSys fuel;
    bool removed = true;

    //create tanks and add pumps
    fuel.addTank(1, 1000, 500);
    fuel.addTank(2, 1000, 300);
    fuel.addTank(3, 1000, 400);

    fuel.addPump(1, 10, 2);
    fuel.addPump(2, 20, 3);
    fuel.addPump(3, 30, 1);
    
    //remove pumps from tanks
    removed = removed && fuel.removePump(1, 10);
    removed = removed && fuel.removePump(2, 20);
    removed = removed && fuel.removePump(3, 30);

    return removed;

}//end rev pump normal

bool Tester::addPumpNormal(){
    FuelSys fuel;
    bool pumpAdded = true;

    fuel.addTank(1, 2000, 1000);
    fuel.addTank(2, 2000, 500);
    fuel.addTank(3, 2000, 400);

    //adding pumps to tanks
    pumpAdded = pumpAdded && fuel.addPump(1, 10, 2);
    pumpAdded = pumpAdded && fuel.addPump(1, 20, 3);
    pumpAdded = pumpAdded && fuel.addPump(2, 30, 3);
    pumpAdded = pumpAdded && fuel.addPump(3, 20, 2);

    return pumpAdded;

}

bool Tester::addPumpError(){
    FuelSys fuel;
    bool error = true;
    
    fuel.addTank(1, 2000, 1000);
    fuel.addTank(2, 2000, 1100);

    //not adding duplicate pump id
    error = error && fuel.addPump(1, 10, 2);
    error = error && !(fuel.addPump(1, 10, 2));

    //non existent tank
    error = error && !(fuel.addPump(12, 10, 2));
    
    return error;
}

bool Tester::totalFuelNormal(){
    FuelSys fuel;
    bool total = true;

    fuel.addTank(1, 2000, 300);
    fuel.addTank(2, 2000, 400);

    total = total && (fuel.totalFuel() == 700);

    return total;

}//end fuel normal

bool Tester::totalFuelError(){
    FuelSys fuel;
    bool total = true;

    total = total && (fuel.totalFuel() == 0);

    return total;
}//end fuel error

bool Tester::findTankNormal(){
    FuelSys fuel;
    bool found = true;

    fuel.addTank(1, 1000, 500);
    fuel.addTank(2, 2000, 1000);
    fuel.addTank(3, 3000, 1500);
    fuel.addTank(4, 3000, 1400);
    fuel.addTank(5, 3000, 1300);
    fuel.addTank(6, 3000, 1200);
    fuel.addTank(7, 3000, 1100);
    fuel.addTank(8, 3000, 1110);

    //checking if tank is found
    //checking if tank is the next node

    found = found && fuel.findTank(1);
    found = found && (fuel.m_current->m_next->m_tankID == 1);

    found = found && fuel.findTank(2);
    found = found && (fuel.m_current->m_next->m_tankID == 2);

    found = found && fuel.findTank(3);
    found = found && (fuel.m_current->m_next->m_tankID == 3);

    found = found && fuel.findTank(4);
    found = found && (fuel.m_current->m_next->m_tankID == 4);

    found = found && fuel.findTank(5);
    found = found && (fuel.m_current->m_next->m_tankID == 5);

    found = found && fuel.findTank(6);
    found = found && (fuel.m_current->m_next->m_tankID == 6);

    found = found && fuel.findTank(7);
    found = found && (fuel.m_current->m_next->m_tankID == 7);

    found = found && fuel.findTank(8);
    found = found && (fuel.m_current->m_next->m_tankID == 8);
   
    return found;

}//end findtank normal

bool Tester::findTankError(){
    FuelSys fuel;
    bool error = true;

    fuel.addTank(1, 1000, 500);
    fuel.addTank(2, 2000, 1000);
    fuel.addTank(3, 3000, 1500);

    //looking for a nonexistent tank
    error = error && (fuel.findTank(12) == false);

    return error;

}//end find tank error

bool Tester::removeTankError(){
    FuelSys fuel;
    bool error = true;

    fuel.addTank(1, 1000, 500);
    fuel.addTank(2, 2000, 1000);
    fuel.addTank(3, 3000, 1500);

    //Check if tanks were properly removed
    error = error && (fuel.removeTank(12) == false);

    return error;

}//end remove normal

bool Tester::removeTankNormal(){
    FuelSys fuel;
    bool tankRemoved = true;

    fuel.addTank(1, 1000, 500);
    fuel.addTank(2, 2000, 1000);
    fuel.addTank(3, 3000, 1500);
    fuel.addTank(4, 3000, 1400);
    fuel.addTank(5, 3000, 1300);
    fuel.addTank(6, 3000, 1200);
    fuel.addTank(7, 3000, 1100);
    fuel.addTank(8, 3000, 1110);

    //Check if tanks were properly removed
    tankRemoved = tankRemoved && fuel.removeTank(1);
    tankRemoved = tankRemoved && (fuel.findTank(1) == false);

    tankRemoved = tankRemoved && fuel.removeTank(2);
    tankRemoved = tankRemoved && (fuel.findTank(2) == false);

    tankRemoved = tankRemoved && fuel.removeTank(3);
    tankRemoved = tankRemoved && (fuel.findTank(3) == false);

    tankRemoved = tankRemoved && fuel.removeTank(4);
    tankRemoved = tankRemoved && (fuel.findTank(4) == false);

    tankRemoved = tankRemoved && fuel.removeTank(5);
    tankRemoved = tankRemoved && (fuel.findTank(5) == false);

    tankRemoved = tankRemoved && fuel.removeTank(6);
    tankRemoved = tankRemoved && (fuel.findTank(6) == false);

    tankRemoved = tankRemoved && fuel.removeTank(7);
    tankRemoved = tankRemoved && (fuel.findTank(7) == false);

    tankRemoved = tankRemoved && fuel.removeTank(8);
    tankRemoved = tankRemoved && (fuel.findTank(8) == false);

    return tankRemoved;

}//end remove normal

bool Tester::addTankNormal(){
    FuelSys fuel;
    bool tankInserted = true;

    //inserting tanks    
    tankInserted = tankInserted && fuel.addTank(1, 1000, 500);
    tankInserted = tankInserted && fuel.addTank(2, 2000, 1000);
    tankInserted = tankInserted && fuel.addTank(3, 3000, 1500);
    tankInserted = tankInserted && fuel.addTank(4, 3000, 1400);
    tankInserted = tankInserted && fuel.addTank(5, 3000, 1300);
    
    //checking if all of them can be found
    tankInserted = tankInserted && fuel.findTank(1);
    tankInserted = tankInserted && fuel.findTank(2);
    tankInserted = tankInserted && fuel.findTank(3);
    tankInserted = tankInserted && fuel.findTank(4);
    tankInserted = tankInserted && fuel.findTank(5);

    return tankInserted;

}//end testAddtankNormal

bool Tester::addTankError(){
    FuelSys fuel;
    bool error = true;

    //Case 1: inserting tank id < 0
    error = error && (fuel.addTank(-1, 1000, 500) == false);
    error = error && (fuel.findTank(-1) == false);

    //Case 2: duplicate tank id
    error = error && fuel.addTank(1, 1000, 500);
    error = error && (fuel.addTank(1, 2000, 1000) == false);


    return error;

}//end tank error