#include "art.h"

class Tester{
    public:

    //constructor tests
    bool constructorError();
    bool constructorEdge();
    bool constructorNormal();

    //create test
    bool createError();

    //copy constructor tests
    bool copyConstructorEdge();
    bool copyConstructorNormal();

    //assignment operator tests
    bool assignmentNormal(); 
    bool assignmentSelf();
    bool assignmentEdge();

    //left2right tests (l2r)
    bool l2rNormal();
    bool l2rDifferentHeight();
    bool l2rNormalToEmpty();
    bool l2rEmptyToNormal();
    bool l2rSelfAppend();

    //top2bottom tests (t2b)
    bool t2bNormal();
    bool t2bDifferentWidth();
    bool t2bNormalToEmpty();
    bool t2bEmptyToNormal();
    bool t2bSelfAppend();

};//end tester

int main(){
    Tester test;

    cout << "=========Constructor Tests=========\n" << endl;

    cout << "Constructor Normal Test: " << (test.constructorNormal() ? "Passed!" : "Failed") << endl;
    cout << "Constructor Edge Test: " << (test.constructorEdge() ? "Passed!" : "Failed") << endl;
    cout << "Constructor Error Test: " << (test.constructorError() ? "Passed!" : "Failed") << endl;

    cout << "\n=========Create Test=========\n" << endl;
    cout << "Create Error Test: " << (test.createError() ? "Passed!" : "Failed") << endl;

    cout << "\n=========Copy Constructor Tests=========\n" << endl;
    cout << "Copy Constructor Normal Test: " << (test.copyConstructorNormal() ? "Passed!" : "Failed") << endl;
    cout << "Copy Constructor Edge Test: " << (test.copyConstructorEdge() ? "Passed!" : "Failed") << endl;

    cout << "\n=========Assignment Operator Tests=========\n" << endl;
    cout << "Assignment Normal Test: " << (test.assignmentNormal() ? "Passed!" : "Failed") << endl;
    cout << "Assignment Edge Test: " << (test.assignmentEdge() ? "Passed!" : "Failed") << endl;
    cout << "Assignment Self Test: " << (test.assignmentSelf() ? "Passed!" : "Failed") << endl;

    cout << "\n=========Left2Right Tests=========\n" << endl;
    cout << "Left2Right Normal Test: " << (test.l2rNormal() ? "Passed!" : "Failed") << endl;
    cout << "Left2Right Different Height Test: " << (test.l2rDifferentHeight() ? "Passed!" : "Failed") << endl;
    cout << "Left2Right Normal To Empty Test: " << (test.l2rNormalToEmpty() ? "Passed!" : "Failed") << endl;
    cout << "Left2Right Empty To Normal Test: " << (test.l2rEmptyToNormal() ? "Passed!" : "Failed") << endl;
    cout << "Left2Right Self Append Test: " << (test.l2rSelfAppend() ? "Passed!" : "Failed") << endl;

    cout << "\n=========Top2Bottom Tests=========\n" << endl;
    cout << "Top2Bottom Normal Test: " << (test.t2bNormal() ? "Passed!" : "Failed") << endl;
    cout << "Top2Bottom Different Width Test: " << (test.t2bDifferentWidth() ? "Passed!" : "Failed") << endl;
    cout << "Top2Bottom Normal To Empty Test: " << (test.t2bNormalToEmpty() ? "Passed!" : "Failed") << endl;
    cout << "Top2Bottom Empty To Normal Test: " << (test.t2bEmptyToNormal() ? "Passed!" : "Failed") << endl;
    cout << "Top2Bottom Self Append Test: " << (test.t2bSelfAppend() ? "Passed!" : "Failed") << endl;
    cout << endl;

    return 0;

}//end main




//Constructor tests
//
bool Tester::constructorError(){
    bool flag = true;
    Art newArt(-1, -10);

    if (newArt.m_height != 0)
        flag = false;
    if (newArt.m_width != 0)
        flag = false;
    if (newArt.m_painting != nullptr)
        flag = false;

    return flag;

}//end constructorError

bool Tester::constructorEdge(){
    bool flag = true;
    Art newArt(1, 1);

    if (newArt.m_height != 0)
        flag = false;
    if (newArt.m_width != 0)
        flag = false;
    if (newArt.m_painting != nullptr)
        flag = false;

    return flag;

}//end constructorEdge

bool Tester::constructorNormal(){
    bool flag = true;
    Art newArt(10, 10);

    if (newArt.m_height != 10)
        flag = false;
    if (newArt.m_width != 10)
        flag = false;
    if (newArt.m_painting == nullptr)
        flag = false; 
    
    return flag;

}//end constructorNormal

//Create 
//

bool Tester::createError(){
    bool flag = true;
    Art newArt;
    newArt.create(10);

    if (newArt.m_height != 0)
        flag = false;
    if (newArt.m_width != 0)
        flag = false;
    if (newArt.m_painting != nullptr)
        flag = false;

    return flag;

}//end createError

//Copy constructor
//

bool Tester::copyConstructorNormal(){
    bool flag = true;
    Art first(5, 8);
    first.create(10);
    Art copy(first);//copy constructor
    
    //diff memory locations
    if (copy.m_painting == first.m_painting)
        flag = false;
    
    //check values
    if (copy.m_height != first.m_height)
        flag = false;
    
    if (copy.m_width != first.m_width)
        flag = false;

    //check colors

    if (flag != false){
        for (int i = 0; i < first.m_height; i++){
            for (int j = 0; j < first.m_width; j++){
                if (copy.m_painting[i][j] != first.m_painting[i][j]){
                    flag = false;
                    break;
                }//end if
            }//end inner
            if (flag == false)
                break;
        }//end for 
    }
    return flag;

}//end copyConstructorNormal

bool Tester::copyConstructorEdge(){
    bool flag = true;
    Art first;
    Art copy(first);

    if (copy.m_height != 0)
        flag = false;
    if (copy.m_width != 0)
        flag = false;
    if (copy.m_painting != nullptr)
        flag = false;

    return flag;

}//end copyConstructorEdge

//Assignment operator
//

bool Tester::assignmentNormal(){
    bool flag = true;
    Art art1(5, 8);
    Art art2(3, 4);
    art1.create(10);
    art2.create(10);

    art2 = art1; //assignment operator 

    //diff memory locations
    if (art2.m_painting == art1.m_painting) 
        flag = false;
    
    //check values
    if (art2.m_height != art1.m_height)
        flag = false;
    if (art2.m_width != art1.m_width)
        flag = false; 
    
    //ck colors

    if (flag != false){
        for (int i = 0; i < art1.m_height; i++){
            for (int j = 0; j < art1.m_width; j++){
                if (art2.m_painting[i][j] != art1.m_painting[i][j]){
                    flag = false;
                    break;
                }//end if
            }//end inner
            if (flag == false)
                break;
        }//end for 
    }

    return flag;

}//end assignmentNormal

bool Tester::assignmentEdge(){
    bool flag = false;
    Art art1;
    Art art2(5, 5);
    art2.create(10);

    art2 = art1;

    if (art2.m_height != 0)
        flag = false;
    if (art2.m_width != 0)
        flag = false;
    if (art2.m_painting != nullptr)
        flag = false;
    
    return flag;

}//end assignmentEdge

bool Tester::assignmentSelf(){
    bool flag = true;
    Art art(5, 5);
    art.create(10);
    int originalHeight = art.m_height;
    int originalWidth = art.m_width;

    art = art; 

    if (art.m_height != originalHeight)
        flag = false;
    if (art.m_width != originalWidth)
        flag = false;
    if (art.m_painting == nullptr)
        flag = false;
    
        //ck colors
    if (flag != false){
        for (int i = 0; i< art.m_height; i++){
            for (int j = 0; j < art.m_width; j++){
                if (art.m_painting[i][j] < 10 || art.m_painting[i][j] > 99){
                    flag = false;
                    break;

                }//end if
                if (flag == false)
                    break;
            }//end inner for

        }//end 
    }//emd if

    return flag;
}//end `assignmentSelf

///left2right tests
//

bool Tester::l2rNormal(){
    bool flag = true;
    Art art1(5, 4);
    Art art2(5, 6);
    art1.create(10);
    art2.create(20);
    int originalWidth1 = art1.m_width;
    int** originalValues = new int*[art1.m_height];

    for (int i = 0; i < art1.m_height; i++){
        originalValues[i] = new int [art1.m_width];
        for (int j = 0; j < art1.m_width; j++){
            originalValues[i][j] = art1.m_painting[i][j];
        }//end inner
    }//outer for

    bool result = art1.left2Right(art2);

    if (result == false)
        flag = false;
    
    //check dimensions
    if (art1.m_height != 5)
        flag = false;
    if (art1.m_width != originalWidth1 + art2.m_width)
        flag = false;
    
    //originla values
    if (flag){
        for (int i = 0; i< art1.m_height; i++){
            for (int j = 0; j< originalWidth1; j++){
                if (art1.m_painting[i][j] != originalValues[i][j]){
                    flag = false;
                    break;
                }//inner if
            }//edn inner
            if (flag == false)
                break;
        }//end outer for
    }//end if

    //art2 values
    if (flag){
        for (int i = 0; i < art1.m_height; i++){
            for (int j = 0; j < art2.m_width; j++){
                if (art1.m_painting[i][originalWidth1 + j] != art2.m_painting[i][j]){
                    flag = false;
                    break;
                }//end if
            }//end inner for
            if (flag == false) 
                break;
        }//end outer for
    }//end outer if

    for (int i=0; i < art1.m_height; i++)
        delete[] originalValues[i];
    
    delete[] originalValues;

    return flag;
}//end l2rNormal

bool Tester::l2rDifferentHeight(){
    bool flag = true;
    Art art1(5, 4);
    Art art2(6, 6);
    art1.create(10);
    art2.create(20);
    bool result = art1.left2Right(art2);

    if (result)
        flag = false;
    
    //art1 ck
    if (art1.m_height != 5)
        flag = false;
    if (art1.m_width != 4)
        flag = false;

    return flag;

}//end l2rDifferentHeight

bool Tester::l2rNormalToEmpty(){
    bool flag = true;
    Art emptyObj;
    Art art (5, 4);
    art.create(10);
    bool result = emptyObj.left2Right(art);

    if (result == false)
        flag = false;

    //check dimensions of emptyObj
    if (emptyObj.m_height != art.m_height)
        flag = false;
    if (emptyObj.m_width != art.m_width)
        flag = false;
    
    //check values
    if (flag){
        for (int i = 0; i< art.m_height; i++){
            for (int j = 0; j< art.m_width; j++){
                if (emptyObj.m_painting[i][j] != art.m_painting[i][j]){
                    flag = false;
                    break;
                }//enf
            }//end inner for
            if (flag == false)
                break;
        }//end outer for 
    }//end if 
    
    return flag;

}//end l2rNormalToEmpty

bool Tester::l2rEmptyToNormal(){
    bool flag = true;
    Art art(5, 4);
    Art emptyObj;
    art.create(10);
    int originalHeight = art.m_height;
    int originalWidth = art.m_width;
    bool result = art.left2Right(emptyObj);

    if (result == false)
        flag =false;

    //ck dimensions
    if (art.m_height != originalHeight)
        flag = false;
    if(art.m_width != originalWidth)
        flag = false;
    
    return flag;

}//end l2rEmptyToNormal

bool Tester::l2rSelfAppend(){
    bool flag = true;
    Art art(5,4);
    art.create(10);
    int originalHeight = art.m_height;
    int originalWidth = art.m_width;
    int** originalValues = new int* [art.m_height];

    for (int i = 0; i < art.m_height ; i++){
        originalValues[i] = new int [art.m_width];
        for (int j = 0; j < art.m_width; j++){
            originalValues[i][j] = art.m_painting[i][j];
        }//end inner for
    }//end for

    bool result = art.left2Right(art);

    if (result == false)
        flag = false;
    
    //ck dimensions
    if (art.m_height != originalHeight || 
        art.m_width != originalWidth * 2)
            flag = false;
    
    //check values
    if (flag){
        for (int i = 0; i < art.m_height; i++){
            for (int j = 0; j< art.m_width; j++){
                if (art.m_painting[i][j] != originalValues[i][j]){
                    result = false;
                    break;
                }//end if
                if (art.m_painting[i][j + originalWidth] != originalValues[i][j]){
                    flag = false;
                    break;
                }//end
            }//end inner 
            if (flag == false)
                break;
        }//end outer for 
    }//end if

    //deallocate memory
    for (int i = 0; i < originalHeight; i++)
        delete[] originalValues[i];
    
    delete[] originalValues;

    return flag;

}//end l2rSelfAppend

//top2bottom tests
//

bool Tester::t2bNormal(){
    bool flag = true;
    Art art1(4, 5);
    Art art2(6, 5);
    art1.create(10);
    art2.create(20);
    int originalHeight1 = art1.m_height;
    int originalWidth1 = art1.m_width;
    int** originalValues = new int*[art1.m_height];

    for (int i =0; i < art1.m_height; i++){
        originalValues[i] = new int[art1.m_width];
        for (int j=0; j < art1.m_width; j++){
            originalValues[i][j] = art1.m_painting[i][j];
            
        }//end inner for
    }//end outer for

    bool result = art1.top2Bottom(art2);

    if (result == false)
        flag = false;
    
    //check dimensions
    if (art1.m_width != originalWidth1 || 
        art1.m_height != originalHeight1 + art2.m_height)
            flag = false;
    
    //check original values
    if (flag){
        for (int i = 0; i< originalHeight1; i++){
            for (int j = 0; j < originalWidth1; j++){
                if (art1.m_painting[i][j] != originalValues[i][j]){
                    flag = false;
                    break;
                }//end inner if
            }//end inner for
            if (flag ==false)
                break;
        }//end outer for 
    }//end if

    if (flag){
        for (int i = 0; i< art2.m_height; i++){
            for (int j = 0; j < originalWidth1; j++){
                if (art1.m_painting[i + originalHeight1][j] != art2.m_painting[i][j]){
                    flag = false;
                    break;
                }//end inner if
            }//end inner for
            if (flag ==false)
                break;
        }//end outer for 
    }//end if

    //deallocate memory
    for (int i = 0; i < originalHeight1; i++)
        delete[] originalValues[i];
    
    delete[] originalValues;

    return flag;

}//end `t2bNormal

bool Tester::t2bDifferentWidth(){
    bool flag = true;
    Art art1(4,5);
    Art art2(6, 8);
    art1.create(10);
    art2.create(20);
    bool result = art1.top2Bottom(art2);

    if (result)
        flag = false;
    
    //check art1
    if (art1.m_height != 4 || art1.m_width != 5)
        flag = false;
    
    return flag;

}//end t2bDifferentWidth

bool Tester::t2bNormalToEmpty(){
    bool flag = true;
    Art emptyObj;
    Art art(4, 5);
    art.create(10);
    bool result = emptyObj.top2Bottom(art); 

    if (result == false)
        flag = false;
    
    if (emptyObj.m_height != art.m_height 
        || emptyObj.m_width != art.m_width)
        flag = false;

    if (flag && emptyObj.m_painting != nullptr){
        for (int i = 0; i < art.m_height; i++){
            for (int j = 0; j < art.m_width; j++){
                if (emptyObj.m_painting[i][j] != art.m_painting[i][j]){
                    flag = false;
                    break;

                }//end
            }//end inner for 
            if (flag == false)
                break;
        }
    }//end if

    return flag;

}//end t2bNormalToEmpty

bool Tester::t2bEmptyToNormal(){
    bool flag = true;
    Art art(4, 5);
    Art emptyObj;
    art.create(10);
    int originalHeight = art.m_height;
    int originalWidth = art.m_width;
    bool result = art.top2Bottom(emptyObj);

    if (result == false || art.m_height != originalHeight 
    || art.m_width != originalWidth)
        flag = false;
    
    return flag;

}//end t2bEmptyToNormal

bool Tester::t2bSelfAppend(){
    bool flag = true;
    Art art(4, 5);
    art.create(10);
    int originalHeight = art.m_height;
    int originalWidth = art.m_width;
    int** originalValues = new int*[art.m_height];

    for (int i = 0; i < originalHeight; i++){
        originalValues[i] = new int[art.m_width];
        for (int j = 0; j < originalWidth; j++){
            originalValues[i][j] = art.m_painting[i][j];
        }//inner for
    }//end for

    bool result = art.top2Bottom(art);

    if (result == false)
        flag = false;
    
    //check dimensions
    if (art.m_width != originalWidth || 
        art.m_height != originalHeight * 2)
            flag = false; 

    //check values
    if (flag){
        for (int i = 0; i < originalHeight; i++){
            for (int j = 0; j < originalWidth; j++){
                if (art.m_painting[i][j] != originalValues[i][j]){
                    flag = false;
                    break;
                }//
                if (art.m_painting[i + originalHeight][j] != originalValues[i][j]){
                    flag = false;
                    break;
                }//
            }//end inner
            if (flag == false)
                break;
        }//end for
    }//end if

    //deallocate memory
    for (int i = 0; i < originalHeight; i++)
        delete[] originalValues[i];

    delete[] originalValues;

    return flag;

}//end t2bSelfAppend

