// UMBC - CMSC 341 - Spring 2026 - Proj0
#include "art.h"
Art::Art(){
    m_height = 0;
    m_width = 0;
    m_painting = nullptr;
    
}//end default constructor

Art::Art(int height, int width){
    if (height > 1 && width > 1){
        m_height = height;
        m_width = width;
        m_painting = new int*[m_height];
        for (int i=0; i < m_height; i++){
            m_painting[i] = new int[m_width];
        }

    } else {
        m_height = 0;
        m_width = 0;
        m_painting = nullptr;
    }//end if 

}//end overloaded constructor

Art::~Art(){
    if (m_painting != nullptr){
        for (int i=0; i < m_height; i++){
            delete [] m_painting[i];
        }
        delete [] m_painting;
        m_painting = nullptr;
    }

}//end destructor

void Art::clear(){
    if (m_painting != nullptr){
        for (int i = 0; i < m_height; i++)
            delete [] m_painting[i];
        
        delete [] m_painting;
        m_height= 0;
        m_width = 0;
        m_painting = nullptr;
    }//end if

}//end clear

void Art::create(int initiate){
    if (m_painting != nullptr){
        Random colorGen(initiate, 99, UNIFORMINT);
        
        for (int i= 0; i < m_height; i++){
            for (int j = 0; j < m_width; j++){
                m_painting[i][j] =colorGen.getRandNum();
            }//end second loop
        }//end for 
    }//end for 

}//end create

void Art::dumpColors(string pixel) const{
    if (m_height > 0 && m_width > 0 && m_painting != nullptr){
        for (int i=1;i <= m_height;i++){
            for (int j=1;j<=m_width;j++){
                std::cout << "\x1b[38;5;" << to_string(m_painting[i-1][j-1]) << "m" << pixel << pixel << RESET;
            }
            cout << endl;
        }
        cout << endl;
    }
}

void Art::dumpValues() const{
    if (m_height > 0 && m_width > 0 && m_painting != nullptr){
        for (int i=1;i <= m_height;i++){
            for (int j=1;j<=m_width;j++){
                cout << m_painting[i-1][j-1] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

Art::Art(const Art& rhs){
    if (rhs.m_painting != nullptr){
        m_height = rhs.m_height;
        m_width = rhs.m_width;
        m_painting = new int*[m_height];

        for (int i = 0; i < m_height; i++){
            m_painting[i] =new int [m_width];
            for (int j = 0; j < m_width; j++){
                m_painting[i][j] = rhs.m_painting[i][j];
            }//end second loop
        }//end for

    } else {
        m_width = 0 ;
        m_height= 0;
        m_painting = nullptr;
    }//end if

}//end copy constructor


const Art& Art::operator=(const Art& rhs){
    if (this != &rhs){
        clear();

        if (rhs.m_painting != nullptr && rhs.m_width > 0 && rhs.m_height > 0){
            m_width = rhs.m_width;
            m_height = rhs.m_height;
            m_painting = new int*[m_height];

            for (int i= 0; i < m_height; i++){
                m_painting[i] = new int[m_width];
                for (int j = 0; j < m_width; j++){
                    m_painting[i][j] = rhs.m_painting[i][j];
                }//end inner for 
            }//end for

        } else {
            m_width = 0;
            m_height = 0;
            m_painting = nullptr;
        }//end inner if else
    }//end if 
    return *this;

}//end assignment operator

bool Art::left2Right(const Art& rhs){
    if (m_height != rhs.m_height && m_height != 0 && rhs.m_height != 0)
        return false;
    
    //empty
    if (rhs.m_height == 0 || rhs.m_width == 0 )
        return true;

    if (m_width == 0 || m_height == 0){
        *this = rhs;
        return true;

    }//end if
    
    Art temp(rhs); //copy of rhs
    int newWidth = m_width + rhs.m_width;
    int** newArt = new int*[m_height];

    for (int i =0; i < m_height; i++){
        newArt[i] = new int [newWidth];

        //copy it to the left side
        for (int j = 0; j <m_width; j++){
            newArt[i][j] = m_painting[i][j];
        
        }//end first inner for

        for (int j =0; j < temp.m_width; j++){
            newArt[i][m_width + j] = temp.m_painting[i][j];

        }//end second inner for
    }//end for

    //deallocate and update
    clear();

    m_painting = newArt;
    m_height = temp.m_height;
    m_width = newWidth;

    return true;

}//end left2Right

bool Art::top2Bottom(const Art& bottom){
    if (m_width != bottom.m_width && m_width != 0 && bottom.m_width != 0)
        return false;
    
    //empty
    if (bottom.m_height == 0 || bottom.m_width == 0)
        return true;

    if (m_height ==0 || m_width == 0){
        *this = bottom;
        return true;
    }//end if

    Art temp(bottom); //copy
    int newHeight = m_height + bottom.m_height;
    int** newArt = new int*[newHeight];

    for (int i = 0; i < m_height; i++){
        newArt[i] = new int[m_width];

        for(int j = 0; j < m_width; j++){
            newArt[i][j] = m_painting[i][j];
        }//end inner for

    }//end for
    
    for (int i = 0; i < m_height; i++){
        newArt[m_height + i] = new int[m_width];

        for (int j=0; j < m_width; j++){
            newArt[m_height + i][j] = temp.m_painting[i][j];
        }//end inner for
            
    }//end for

    //deallocate and update
    clear();

    m_painting = newArt;
    m_height = newHeight;
    m_width = temp.m_width;

    return true;

}//end top2Bottom

bool Art::reverse(){
    if (m_width == 0|| m_height == 0 || m_painting == nullptr)
        return false;
    
    //swap end objects of each row
    for (int i = 0; i< m_height; i++){
        for (int j = 0; j < m_width/2; j++){
            int temp = m_painting[i][j];
            
            m_painting[i][j] = m_painting[i][m_width - 1 - j];
            m_painting[i][m_width - 1 - j] = temp;

        }//end inner loop
    }//end for

    //reverse order of rows
    for (int i = 0; i < m_height / 2; i++){
        int* tempRow = m_painting[i];

        m_painting[i] = m_painting[m_height - 1 - i];
        m_painting[m_height - 1 - i] = tempRow;

    }//end for

    return true;

}//end reverse

bool Art::rotate(){
   if (m_height % 2 !=0 || m_width % 2 != 0)
        return false;
    
    if (m_width == 0 || m_height == 0 || m_painting == nullptr)
        return false;

    int halfHeight = m_height/2;
    int halfWidth = m_width/2;
    int** tempQ = new int*[halfHeight]; //temp storage for one quadrant

    for (int i=0; i < halfHeight; i++)
        tempQ[i] = new int[halfWidth];
    
    //copy the top left quadrant to temp storage
    for (int i = 0; i < halfHeight; i++)
        for (int j = 0; j < halfWidth; j++)
            tempQ[i][j] = m_painting[i][j];
    
    //move bottom left to top left
    for (int i = 0; i < halfHeight; i++)
        for (int j = 0; j < halfWidth; j++)
            m_painting[i][j] = m_painting[halfHeight + i][j];
    
    //move bottom right to bottom left
    for (int i = 0; i < halfHeight; i++)
        for (int j =0; j < halfWidth; j++)
            m_painting[halfHeight + i][j] = m_painting[halfHeight + i][halfWidth + j];
    
    //move top right to bottom right
    for (int i =0; i < halfHeight; i++)
        for (int j = 0; j <halfWidth; j++)
            m_painting[halfHeight + i][halfWidth + j] = m_painting[i][halfWidth + j];
    
    //move temp
    for (int i = 0; i< halfHeight; i++)
        for (int j = 0; j < halfWidth; j++)
            m_painting[i][halfWidth + j] = tempQ[i][j];
    
    //deallocate tempQ
    for (int i = 0; i < halfHeight; i++)
        delete[] tempQ[i];

    delete[] tempQ;

    return true;
        
}//end rotate
