//
//  Kyle Samson
//  C++ Programming II
//  Super Star Trek Header
//

#ifndef starTrek_hpp
#define starTrek_hpp

const int BOARD_SIZE = 8;

class StarTrek
{
    public:
    StarTrek();
    ~StarTrek();

    void play();
    void titleCard();
    void instructions();
    void introduction();
    void enterpriseInput();
    void enterpriseNAV();
    void enterpriseLRS();
    void enterprisePHA();
    void enterpriseTOR();
    void enterpriseSHE();
    void enterpriseDAM();
    void enterpriseCOM();
    void enterpriseXXX();
    void enterpriseHLP();
    void drawBoard(); // Short-Range Scan/SRS

    private:
    int firePhasers;                        // Phasers
    int shieldUnitAlloc;                    // Shield Unit Allocator
    int torpedoQuadrantX = mQuadrantRow;    // Makes Torpedo Location the same as the Enterprise...
    int torpedoQuadrantY = mQuadrantColumn; // ...preventing player form destroying Klingons outside Klingon's Quadrant
    int torpedoSectX;                       // Torpedo Target Sector Row
    int torpedoSectY;                       // Torpedo Target Sector Column
    int terminalInput;                      // Library Computer Input

    // Enterprise Quadrant, Sector Rows and Columns
    int mQuadrantRow;
    int mQuadrantColumn;
    int mRow;
    int mColumn;

    // Direction/Distance Calculator Variables
    int mInitCoorX;
    int mInitCoorY;
    int mFinalCoorX;
    int mFinalCoorY;
    float mDistanceX;
    float mDistanceY;

    // Starbase Navigation Data Variables
    int mStarbaseDDX;
    int mStarbaseDDY;
    
    // Photon Torpedo Data Variables
    int mInitialTorpedoX;
    int mInitialTorpedoY;

    // Starbase Position Variables
    int mStarbaseQuadrantRow;
    int mStarbaseQuadrantColumn;
    int mStarbaseRow;
    int mStarbaseColumn;

    // Klingon Position Variables
    int mKlingonQuadrantRow;
    int mKlingonQuadrantColumn;
    int mKlingonRow;
    int mKlingonColumn;

    int mKlingonEnergy = 1000;
    int mKlingonsRemaining = 1;

    int mScore = 0;
    int mEnergy = 3000;
    int mShields = 0;
    int mOverloadValue; // Adds Leftover Energy Units back to mEnergy if mShields is greater than 1500
    int mTorpedo = 10;
};

#endif