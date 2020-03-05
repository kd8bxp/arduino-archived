//
//  Kyle Samson
//  C++ Programming II
//  Super Star Trek Body
//

#include "starTrek.hpp"
#include <math.h>
//#include <iomanip>
//#include <iostream>
//#include <string>
#include <unistd.h>

using namespace std;

string assignCommand;
string navSelect;
string playerInput;

//
// Row 1: Antares I-IV and Sirius I-IV
// Row 2: Rigel I-IV and Deneb I-IV
// Row 3: Procyon I-IV and Capella I-IV
// Row 4: Vega I-IV and Betelgeuse I-IV
// Row 5: Canopus I-IV and Aldebaran I-IV
// Row 6: Altair I-IV and Regulus I-IV
// Row 7: Sagittarius I-IV and Arcturus I-IV
// Row 8: Pollux I-IV and Spica I-IV
//
// extra "\0"s ensures numbers 1-8 are properly input
// in conjunction with while loop in StarTrek::enterpriseNAV()
string quadName[9][9] = {{"\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"},
{"\0", "ANTARES I", "ANTARES II", "ANTARES III", "ANTARES IV", 
"SIRIUS I", "SIRIUS II", "SIRIUS III", "SIRIUS IV"}, 
{"\0", "RIGEL I", "RIGEL II", "RIGEL III", "RIGEL IV", 
"DENEB I", "DENEB I", "DENEB III", "DENEB IV"},
{"\0", "PROCYON I", "PROCYON II", "PROCYON III", "PROCYON IV", 
"CAPELLA I", "CAPELLA II", "CAPELLA III", "CAPELLA IV"},
{"\0", "VEGA I", "VEGA II", "VEGA III", "VEGA IV",  
"BETELGEUSE I", "BETELGEUSE II", "BETELGEUSE III", "BETELGEUSE IV"},
{"\0", "CANOPUS I", "CANOPUS II", "CANOPUS III", "CANOPUS IV", 
"ALDEBARAN I", "ALDEBARAN I",  "ALDEBARAN III",  "ALDEBARAN IV"},
{"\0", "ALTAIR I", "ALTAIR II", "ALTAIR III", "ALTAIR IV", 
"REGULUS I", "REGULUS II", "REGULUS III", "REGULUS IV"},
{"\0", "SAGITTARIUS I", "SAGITTARIUS II", "SAGITTARIUS III", "SAGITTARIUS IV", 
"ARCTURUS I", "ARCTURUS II", "ARCTURUS III", "ARCTURUS IV"},
{"\0", "POLLUX I", "POLLUX II", "POLLUX III", "POLLUX IV", 
"SPICA I", "SPICA II", "SPICA III", "SPICA IV"}};

StarTrek::StarTrek() // constructor
{
    Serial.print("ENTERPRISE OBJECT BEING CREATED. ");
} 

StarTrek::~StarTrek() // destructor
{
    Serial.print("\nENTERPRISE OBJECT BEING DELETED. ");
    exit(1);
}

void StarTrek::play()
{
    titleCard();
    introduction();
    enterpriseInput();
}

void StarTrek::enterpriseNAV()
{
    Serial.print("TYPE Q FOR QUADRANT AND SECTOR NAVIGATION, OR S FOR SECTOR NAVIGATION. ENTER C TO CANCEL: ");
    while (cin >> navSelect)
    {
        if (navSelect == "Q" || navSelect == "q")
        {
            //
            // ask quadrant row
            Serial.print << "SPECIFY NEW OR CURRENT QUADRANT X COORDINATE.";
            Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
            while ((cin >> mQuadrantRow && mQuadrantRow < 1) || (mQuadrantRow > 8) || (mQuadrantRow == 0))
            {
                if (mQuadrantRow <= 1 && mQuadrantRow >= 8) {}
                cin.clear();
                cin.ignore();
                Serial.print << "ENSIGN CHEKOV REPORTS: INVALID COORDINATES CAPTAIN!" << endl;
                Serial.print << "SPECIFY QUADRANT X COORDINATE. ";
                Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
                continue;
            };

            //
            // ask quadrant column
            Serial.print << "SPECIFY NEW OR CURRENT QUADRANT Y COORDINATE.";
            Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
            while ((cin >> mQuadrantColumn && mQuadrantColumn < 1) || (mQuadrantColumn > 8) || (mQuadrantColumn == 0))
            {
                if (mQuadrantColumn <= 1 && mQuadrantColumn >= 8) {}
                cin.clear();
                cin.ignore();
                Serial.print << "ENSIGN CHEKOV REPORTS: INVALID COORDINATES CAPTAIN!" << endl;
                Serial.print << "SPECIFY QUADRANT Y COORDINATE. ";
                Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
                continue;
            };

            //
            // ask sector row
            Serial.print << "SPECIFY NEW SECTOR X COORDINATE.";
            Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
            while ((cin >> mRow && mRow < 1) || (mRow > BOARD_SIZE) || (mRow == 0))
            {
                if (mRow <= mRow && mRow >= BOARD_SIZE) {}
                cin.clear();
                cin.ignore();
                Serial.print << "ENSIGN CHEKOV REPORTS: INVALID COORDINATES CAPTAIN!" << endl;
                Serial.print << "SPECIFY SECTOR X COORDINATE. ";
                Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
                continue;
            };

            //
            // ask sector column
            Serial.print << "SPECIFY NEW SECTOR Y COORDINATE.";
            Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
            while ((cin >> mColumn && mColumn < 1) || (mColumn > BOARD_SIZE) || (mColumn == 0))
            {
                if (mColumn <= 1 && mColumn >= BOARD_SIZE) {}
                cin.clear();
                cin.ignore();
                Serial.print << "ENSIGN CHEKOV REPORTS: INVALID COORDINATES CAPTAIN!" << endl;
                Serial.print << "SPECIFY SECTOR Y COORDINATE. ";
                Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
                continue;
            };

            mEnergy = mEnergy - 100;

            if (mEnergy == 0)
            {
                Serial.print << "\n*** FATAL ERROR ***" << endl;
                Serial.print << "YOU'VE JUST STRANDED YOUR SHIP IN SPACE." << endl;
                Serial.print << "YOU HAVE INSUFFICIENT MANEUVERING ENERGY AND SHIELD CONTROL IS" << endl;
                Serial.print << "PRESENTLY INCAPABLE OF CROSS-CIRCUITING TO ENGINE ROOM!!!" << endl << endl;
                cin.get();

                Serial.print << "THE FEDERATION IS IN NEED OF A NEW STARSHIP COMMANDER";
                Serial.print << "\nFOR A SIMILAR MISSION. -- IF THERE IS A VOLUNTEER,";
                Serial.print << "\nLET HIM STEP FORWARD AND ENTER 'AYE' OR 'NAY'? ";
                while (cin >> assignCommand)
                {
                    if (assignCommand == "AYE" || assignCommand == "aye")
                    {
                        mEnergy = mEnergy + 3000;
                        introduction();
                        enterpriseInput();
                    }
                    else if (assignCommand == "NAY" || assignCommand == "nay")
                    {
                        exit(1);      
                    }
                    cin.clear();
                    cin.ignore();
                    Serial.print << "REPEAT: IF THERE IS A VOLUNTEER, LET HIM STEP FORWARD AND ENTER 'AYE' OR 'NAY': ";
                    continue;
                }
            }

            if (mRow == mKlingonRow && mColumn == mKlingonColumn)
            {
                mColumn = mColumn - 1;
                Serial.print << "\nENSIGN CHEKOV REPORTS:" << endl;
                Serial.print << "SECTOR IS OCCUPIED BY ENEMY KLINGON." << endl;
                Serial.print << "ENTERPRISE NAVIGATION IN QUADRANT " << mQuadrantRow << " , " << mQuadrantColumn << endl; 
                Serial.print << "STOPPED AT SECTOR " << mRow << " , " << mColumn << endl << endl;
                enterpriseInput();
            }

            else
            {
                Serial.print << "\nENSIGN CHEKOV REPORTS:";
                Serial.print << "\nCOURSE LAID IN " << quadName[mQuadrantRow][mQuadrantColumn] << " QUADRANT, CAPTAIN." << endl << endl;
                Serial.print << "QUADRANT            " << mQuadrantRow << " , " << mQuadrantColumn << endl;
                Serial.print << "SECTOR              " << mRow << " , " << mColumn << endl << endl;
                enterpriseInput();
            }
        }

        else if (navSelect == "S" || navSelect == "s")
        {
            //
            // ask sector row
            Serial.print << "SPECIFY NEW SECTOR X COORDINATE.";
            Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
            while ((cin >> mRow && mRow < 1) || (mRow > BOARD_SIZE) || (mRow == 0))
            {
                if (mRow <= mRow && mRow >= BOARD_SIZE) {}
                cin.clear();
                cin.ignore();
                Serial.print << "ENSIGN CHEKOV REPORTS: INVALID COORDINATES CAPTAIN!" << endl;
                Serial.print << "SPECIFY SECTOR X COORDINATE. ";
                Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
                continue;
            };

            //
            // ask sector column
            Serial.print << "SPECIFY NEW SECTOR Y COORDINATE.";
            Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
            while ((cin >> mColumn && mColumn < 1) || (mColumn > BOARD_SIZE) || (mColumn == 0))
            {
                if (mColumn <= 1 && mColumn >= BOARD_SIZE) {}
                cin.clear();
                cin.ignore();
                Serial.print << "ENSIGN CHEKOV REPORTS: INVALID COORDINATES CAPTAIN!" << endl;
                Serial.print << "SPECIFY SECTOR Y COORDINATE. ";
                Serial.print << "\nENTER A NUMBER BETWEEN 1 AND 8: ";
                continue;
            };

            mEnergy = mEnergy - 10;

            if (mEnergy == 0)
            {
                Serial.print << "\n*** FATAL ERROR ***" << endl;
                Serial.print << "YOU'VE JUST STRANDED YOUR SHIP IN SPACE." << endl;
                Serial.print << "YOU HAVE INSUFFICIENT MANEUVERING ENERGY AND SHIELD CONTROL IS" << endl;
                Serial.print << "PRESENTLY INCAPABLE OF CROSS-CIRCUITING TO ENGINE ROOM!!!" << endl << endl;
                cin.get();

                enterpriseXXX();
            }

            if (mRow == mKlingonRow && mColumn == mKlingonColumn)
            {
                mColumn = mColumn - 1;
                Serial.print << "\nENSIGN CHEKOV REPORTS:" << endl;
                Serial.print << "SECTOR IS OCCUPIED BY ENEMY KLINGON." << endl;
                Serial.print << "ENTERPRISE NAVIGATION IN QUADRANT " << mQuadrantRow << " , " << mQuadrantColumn << endl; 
                Serial.print << "STOPPED AT SECTOR " << mRow << " , " << mColumn << endl << endl;
                enterpriseInput();
            }

            else
            {
                Serial.print << "\nENSIGN CHEKOV REPORTS:";
                Serial.print << "\nCOURSE LAID IN " << quadName[mQuadrantRow][mQuadrantColumn] << " QUADRANT, CAPTAIN." << endl << endl;
                Serial.print << "QUADRANT            " << mQuadrantRow << " , " << mQuadrantColumn << endl;
                Serial.print << "SECTOR              " << mRow << " , " << mColumn << endl << endl;
                enterpriseInput();
            }
        }

        else if (navSelect == "C" || navSelect == "c")
        {
            enterpriseInput();
        }
        cin.clear();
        cin.ignore();
        Serial.print << "INVALID REQUEST. TYPE Q FOR QUADRANT AND SECTOR NAVIGATION, OR S FOR SECTOR NAVIGATION: ";
        continue;
    }
};

void StarTrek::enterpriseLRS()
{
    Serial.print << "LONG RANGE SCAN FOR QUADRANT " << mQuadrantRow << " , " << mQuadrantColumn << endl;
    Serial.print << "-------------------" << endl;
    Serial.print << ": 000 : 000 : 000 :" << endl;
    Serial.print << "-------------------" << endl;
    Serial.print << ": 000 : 110 : 000 :" << endl;
    Serial.print << "-------------------" << endl;
    Serial.print << ": 000 : 000 : 000 :" << endl;
    Serial.print << "-------------------" << endl;
    enterpriseInput();
};

void StarTrek::enterprisePHA()
{
    if (mKlingonQuadrantRow == mQuadrantRow && mKlingonQuadrantColumn == mQuadrantColumn)
    {
        Serial.print << "PHASERS LOCKED ON TARGET;   ENERGY AVAILABLE = " << mEnergy << endl;
        Serial.print << "NUMBER OF UNITS TO FIRE? ";
        while (!(cin >> firePhasers) || (firePhasers > 3000))
        {
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID NUMBER OF UNITS" << endl;
            Serial.print << "ENERGY AVAILABLE = " << mEnergy << endl;
            Serial.print << "\nNUMBER OF UNITS TO FIRE? ";
            continue;
        }

        Serial.print << firePhasers << " UNIT HIT ON KLINGON AT SECTOR " << mKlingonRow << " , " << mKlingonColumn << endl;
        mKlingonEnergy = mKlingonEnergy - firePhasers;

        if (mKlingonEnergy == 0)
        {
            Serial.print << "***KLINGON DESTROYED***";
            mScore = mScore + 10;
            mKlingonsRemaining--;

            if (mKlingonsRemaining == 0)
            {
                mScore = mScore + 100;
                Serial.print << "\n\nCONGRATULATIONS, CAPTAIN!"
                        << "\n\nTHE LAST KLINGON BATTLE CRUISER MENACING THE FEDERATION HAS BEEN DESTROYED."
                        << "\n\nYOUR EFFICIENCY RATING IS: " << mScore << endl << endl;
                cin.get();

                enterpriseXXX();
            }
        }
    }

    else
    {
        Serial.print << "ENERGY WILL BE EXPENDED INTO SPACE" << endl;
        Serial.print << "UNITS TO FIRE = ";
        while (!(cin >> firePhasers) || (firePhasers > 3000))
        {
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID NUMBER OF UNITS" << endl;
            Serial.print << "ENERGY AVAILABLE = " << mEnergy;
            Serial.print << "\nUNITS TO FIRE = ";
            continue;            
        }
        if (firePhasers == 0)
        {
            Serial.print << endl;
        }
        if ((firePhasers > 0) && (firePhasers < mEnergy))
        {
            mEnergy = mEnergy - firePhasers;
            Serial.print << firePhasers << " EXPENDED ON EMPTY SPACE.\n" << endl;
        }
        enterpriseInput();
    }
};

void StarTrek::enterpriseTOR()
{
    if (mTorpedo == 0)
    {
        Serial.print << "ALL PHOTON TORPEDOES EXPENDED." << endl;
    }
    if (mTorpedo > 0)
    {
        Serial.print << "PHOTON TORPEDOES LEFT = " << mTorpedo << endl;;
        Serial.print << "TORPEDO TARGET SECTOR X? ";
        while ((cin >> torpedoSectX && torpedoSectX == 0) || (torpedoSectX < 1) || (torpedoSectX > 9))
        {
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID SECTOR X COORDINATE.";
            Serial.print << "\nTORPEDO TARGET SECTOR X? ";
            continue;
        }

        Serial.print << "TORPEDO TARGET SECTOR Y? ";
        while ((cin >> torpedoSectY && torpedoSectY == 0) || (torpedoSectY < 1) || (torpedoSectY > 9))
        {
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID SECTOR Y COORDINATE.";
            Serial.print << "\nTORPEDO TARGET SECTOR Y? ";
            continue;
        }

        mTorpedo = mTorpedo - 1;

        if (torpedoSectX == mKlingonRow && torpedoSectY == mKlingonColumn)
        {
            mKlingonEnergy = mKlingonEnergy - 1000;
            Serial.print << "\n*** KLINGON DESTROYED ***";
            mScore = mScore + 10;
            mKlingonsRemaining--;

            if (mKlingonsRemaining == 0)
            {
                mScore = mScore + 100;
                Serial.print << "\n\nCONGRATULATIONS, CAPTAIN!"
                        << "\n\nTHE LAST KLINGON BATTLE CRUISER MENACING THE FEDERATION HAS BEEN DESTROYED."
                        << "\n\nYOUR EFFICIENCY RATING IS: " << mScore << endl << endl;
                cin.get();

                enterpriseXXX();
            }
        }

        /*
        Serial.print << "TORPEDO TRACK:" << endl;
        usleep(1500000);
        Serial.print << "                0 , 0" << endl;
        usleep(1500000);
        Serial.print << "                0 , 0" << endl;
        usleep(1500000);
        Serial.print << "                0 , 0" << endl;
        usleep(1500000);
        Serial.print << "                0 , 0" << endl;
        usleep(1500000);
        Serial.print << "PHOTON TORPEDO TEST SUCCESSFUL." << endl;
        */
    }
    enterpriseInput();
}

void StarTrek::enterpriseSHE()
{
    if (mShields == 1500)
    {
        Serial.print << "SHIELD CONTROL REPORTS:" << endl;
        Serial.print << "  'SHIELDS ARE AT MAXIMUM AMOUNT OF UNITS.'" << endl;
        Serial.print << "  (SHIELDS UNCHANGED.)\n";
        enterpriseInput();
    }

    if (mShields >= 0 && mShields < 1500)
    {
        Serial.print << "ENERGY AVAILABLE = " << mEnergy << " NUMBER OF UNITS TO SHIELDS? ";
        while (!(cin >> shieldUnitAlloc))
        {
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID NUMBER OF UNITS";
            Serial.print << "\nNUMBER OF UNITS TO SHIELDS ";
            continue;
        }

        if (shieldUnitAlloc == 0)
        {
            Serial.print << "  (SHIELDS UNCHANGED.)" << endl;
        }

        if (shieldUnitAlloc > 0)
        {
            if ((shieldUnitAlloc > mEnergy) || (shieldUnitAlloc == mEnergy))
            {
                Serial.print << "SHIELD CONTROL REPORTS:" << endl;
                Serial.print << "  'THIS IS NOT THE FEDERATION TREASURY.'" << endl;
                Serial.print << "  (SHIELDS UNCHANGED.)\n";
            }

            else if ((mShields + shieldUnitAlloc) > 1500)
            {
                mOverloadValue = (mShields + shieldUnitAlloc) - 1500;
                mEnergy = (mEnergy - shieldUnitAlloc) + mOverloadValue;
                mShields = 1500; // cap mShields to maximum of 1500 units
                Serial.print << "DEFLECTOR ROOM REPORTS:" << endl;
                Serial.print << "  'SHIELDS CAPPED AT " << mShields << " UNITS.'" << endl;
            }

            else if (shieldUnitAlloc < mEnergy)
            {
                mEnergy = mEnergy - shieldUnitAlloc;
                mShields = mShields + shieldUnitAlloc;
                Serial.print << "DEFLECTOR ROOM REPORTS:" << endl;
                Serial.print << "  'SHIELDS ARE NOW AT " << mShields << " UNITS PER YOUR COMMAND.'" << endl;
            }
        }
        enterpriseInput();
    }
};

void StarTrek::enterpriseDAM()
{
    Serial.print << "\nDEVICE             STATE OF REPAIR" << endl;
    Serial.print << "WARP ENGINES              0" << endl;
    Serial.print << "SHORT RANGE SENSORS       0" << endl;
    Serial.print << "LONG RANGE SENSORS        0" << endl;
    Serial.print << "PHASER CONTROL            0" << endl;
    Serial.print << "PHOTON TUBES              0" << endl;
    Serial.print << "DAMAGE CONTROL            0" << endl;
    Serial.print << "SHIELD CONTROL            0" << endl;
    Serial.print << "LIBRARY COMPUTER          0\n" << endl;
    enterpriseInput();
};

void StarTrek::enterpriseCOM()
{
    Serial.print << "AVAILABLE OPTIONS: 0, 1, 2, 3, 4, 5" << endl;
    Serial.print << "COMPUTER ACTIVE AND AWAITING COMMAND? ";
    while ((cin >> terminalInput && terminalInput < 0) || (terminalInput > 5))
    {
        if (terminalInput <= 0 && terminalInput >= 5) {}
        cin.clear();
        cin.ignore();
        Serial.print << "INVALID REQUEST. AWAITING COMMAND? ";
        continue;
    }
    if (terminalInput == 0)
    {
        Serial.print << "COMPUTER RECORD OF GALAXY FOR QUADRANT " << mQuadrantRow << " , " << mQuadrantColumn << endl << endl;
        Serial.print << "       1     2     3     4     5     6     7     8  "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----"
                "\n1     ***   ***   ***   ***   ***   ***   ***   *** "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----"
                "\n2     ***   ***   ***   ***   ***   ***   ***   *** "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----"
                "\n3     ***   ***   ***   ***   ***   ***   ***   *** "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----"
                "\n4     ***   ***   ***   ***   110   ***   ***   *** "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----"
                "\n5     ***   ***   ***   ***   ***   ***   ***   *** "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----"
                "\n6     ***   ***   ***   ***   ***   ***   ***   *** "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----"
                "\n7     ***   ***   ***   ***   ***   ***   ***   *** "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----"
                "\n8     ***   ***   ***   ***   ***   ***   ***   *** "
                "\n     ----- ----- ----- ----- ----- ----- ----- -----" << endl << endl;
    }
    if (terminalInput == 1)
    {
        Serial.print << "\nSTARDATE            3000" << endl;
        if (mQuadrantRow == mKlingonQuadrantRow && mQuadrantColumn == mKlingonQuadrantColumn)
        {
            Serial.print << "CONDITION           RED" << endl;
        }
        else
        {
            Serial.print << "CONDITION           GREEN" << endl;
        }
        Serial.print << "QUADRANT            " << mQuadrantRow << " , " << mQuadrantColumn << endl;
        Serial.print << "SECTOR              " << mRow << " , " << mColumn << endl;
        Serial.print << "PHOTON TORPEDOES    " << mTorpedo << endl;
        Serial.print << "TOTAL ENERGY        " << mEnergy << endl;
        Serial.print << "SHIELDS             " << mShields << endl;
        Serial.print << "KLINGONS REMAINING  " << mKlingonsRemaining << endl << endl;
    }
    if (terminalInput == 2)
    {
        if (mQuadrantRow != mKlingonQuadrantRow && mQuadrantColumn != mKlingonQuadrantColumn)
        {
            Serial.print << "SCIENCE OFFICER SPOCK REPORTS: SENSORS SHOW NO KLINGONS IN THIS QUADRANT." << endl;
        }
        else
        {
            mInitialTorpedoX = mColumn - mKlingonColumn;
            mInitialTorpedoY = mRow - mKlingonRow;
            Serial.print << "FROM ENTERPRISE TO KLINGON BATTLE CRUISER" << endl;
            Serial.print << "DIRECTION/DISTANCE = " << abs(mInitialTorpedoX) << " , " << abs(mInitialTorpedoY) << endl;
        }
    }
    if (terminalInput == 3)
    {
        if (mQuadrantRow != mStarbaseQuadrantRow && mQuadrantColumn != mStarbaseQuadrantColumn)
        {
            Serial.print << "SCIENCE OFFICER SPOCK REPORTS: SENSORS SHOW NO STARBASES IN THIS QUADRANT." << endl;
        }
        else
        {
            mStarbaseDDX = mColumn - mStarbaseColumn;
            mStarbaseDDY = mRow - mStarbaseRow;
            Serial.print << "FROM ENTERPRISE TO NEARBY STARBASE" << endl;
            Serial.print << "DIRECTION/DISTANCE = " << abs(mStarbaseDDX) << " , " << abs(mStarbaseDDY) << endl;
        }
    }
    if (terminalInput == 4)
    {
        Serial.print << "DIRECTION/DISTANCE CALCULATOR" << endl;
        Serial.print << "YOU ARE AT QUADRANT " << mQuadrantRow << " , " << mQuadrantColumn << " SECTOR " << mRow << " , " << mColumn << endl;
        Serial.print << "PLEASE ENTER" << endl;
        Serial.print << "  INITIAL COORDINATATES (X)? ";
        while ((cin >> mInitCoorX && mInitCoorX < 1) || (mInitCoorX > 8) || (mInitCoorX == 0))
        {
            if (mInitCoorX <= 1 && mInitCoorX >= 8) {}
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID COORDINATES. ENTER INITIAL COORDINATATES (X)? ";
            continue;
        }
        Serial.print << "  INITIAL COORDINATATES (Y)? ";
        while ((cin >> mInitCoorY && mInitCoorY < 1) || (mInitCoorY > 8) || (mInitCoorY == 0))
        {
            if (mInitCoorY <= 1 && mInitCoorY >= 8) {}
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID COORDINATES. ENTER INITIAL COORDINATATES (Y)? ";
            continue;
        }

        Serial.print << "  FINAL COORDINATES (X) ";
        while ((cin >> mFinalCoorX && mFinalCoorX < 1) || (mFinalCoorX > 8) || (mFinalCoorX == 0))
        {
            if (mFinalCoorX <= 1 && mFinalCoorX >= 8) {}
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID COORDINATES. ENTER FINAL COORDINATES (X)? ";
            continue;
        }

        Serial.print << "  FINAL COORDINATES (Y) ";
        while ((cin >> mFinalCoorY && mFinalCoorY < 1) || (mFinalCoorY > 8) || (mFinalCoorY == 0))
        {
            if (mFinalCoorY <= 1 && mFinalCoorY >= 8) {}
            cin.clear();
            cin.ignore();
            Serial.print << "INVALID COORDINATES. ENTER FINAL COORDINATES (Y)? ";
            continue;
        }

        mDistanceX = mInitCoorX - mFinalCoorX;
        mDistanceY = mInitCoorY - mFinalCoorY;
        Serial.print << "DIRECTION/DISTANCE = " << abs(mDistanceX) << " , " << abs(mDistanceY) << endl;
        enterpriseInput();
    }
    if (terminalInput == 5)
    {
        Serial.print << "\n      1     2     3     4     5     6     7     8    " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----" 
                "\n1         ANTARES                  SIRIUS          " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----" 
                "\n2          RIGEL                   DENEB           " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----" 
                "\n3         PROCYON                 CAPELLA          " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----" 
                "\n4           VEGA                 BETELGEUSE        " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----" 
                "\n5         CANOPUS                ALDEBARAN         " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----" 
                "\n6          ALTAIR                 REGULUS          " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----" 
                "\n7       SAGITTARIUS               ARCTURUS         " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----" 
                "\n8          POLLUX                  SPICA           " 
                "\n    ----- ----- ----- ----- ----- ----- ----- -----\n" << endl;
    }
    enterpriseInput();
};

void StarTrek::enterpriseXXX()
{
    Serial.print << "THE FEDERATION IS IN NEED OF A NEW STARSHIP COMMANDER";
    Serial.print << "\nFOR A SIMILAR MISSION. -- IF THERE IS A VOLUNTEER,";
    Serial.print << "\nLET HIM STEP FORWARD AND ENTER 'AYE' OR 'NAY'? ";
    while (cin >> assignCommand)
    {
        if (assignCommand == "AYE" || assignCommand == "aye")
        {
            introduction();
            enterpriseInput();
        }
        else if (assignCommand == "NAY" || assignCommand == "nay") 
        {
            this->~StarTrek();
        }
        cin.clear();
        cin.ignore();
        Serial.print << "REPEAT: IF THERE IS A VOLUNTEER, LET HIM STEP FORWARD AND ENTER 'AYE' OR 'NAY': ";
        continue;
    }
};

void StarTrek::enterpriseHLP()
{
    Serial.print << "ENTER ONE OF THE FOLLOWING:\n"
            "   NAV  (TO SET COURSE)\n"
            "   SRS  (FOR SHORT-RANGE SENSOR SCAN)\n"
            "   LRS  (FOR LONG-RANGE SENSOR SCAN)\n"
            "   PHA  (TO FIRE PHASERS)\n"
            "   TOR  (TO FIRE PHOTON TORPEDOES)\n"
            "   SHE  (TO RAISE OR LOWER SHIELDS)\n"
            "   DAM  (FOR DAMAGE CONTROL REPORT)\n"
            "   COM  (TO CALL ON LIBRARY COMPUTER)\n"
            "   XXX  (TO RESIGN YOUR COMMAND)\n"
            "   HLP  (TO DISPLAY HELP)\n";
    enterpriseInput();
}

void StarTrek::enterpriseInput()
{   
    Serial.print << "COMMAND? ";
    cin >> playerInput;

    if (playerInput == "NAV" || playerInput == "nav")
    {
        enterpriseNAV();
        enterpriseInput();
    }

    else if (playerInput == "SRS" || playerInput == "srs")
    {
        drawBoard();
        enterpriseInput();
    }

    else if (playerInput == "LRS" || playerInput == "lrs")
    {
        enterpriseLRS();
        enterpriseInput();
    }

    else if (playerInput == "PHA" || playerInput == "pha")
    {
        enterprisePHA();
        enterpriseInput();
    }

    else if (playerInput == "TOR" || playerInput == "tor")
    {
        enterpriseTOR();
        enterpriseInput();
    }

    else if (playerInput == "SHE" || playerInput == "she")
    {
        enterpriseSHE();
        enterpriseInput();
    }

    else if (playerInput == "DAM" || playerInput == "dam")
    {
        enterpriseDAM();
        enterpriseInput();
    }

    else if (playerInput == "COM" || playerInput == "com")
    {
        enterpriseCOM();
        enterpriseInput();
    }

    else if (playerInput == "XXX" || playerInput == "xxx")
    {
        enterpriseXXX();
        enterpriseInput();
    }

    else if (playerInput == "HLP" || playerInput == "hlp")
    {
        enterpriseHLP();
        enterpriseInput();
    }

    else
    {
        Serial.print << "\nSCIENCE OFFICER SPOCK REPORTS: BEG YOUR PARDON, CAPTAIN?\n"
                "TYPE 'HLP' FOR AVAILABLE COMMANDS.\n";
        enterpriseInput();
    }
}

void StarTrek::drawBoard()
{
    Serial.print << "\n        " << quadName[mQuadrantRow][mQuadrantColumn];
    Serial.print << "\n   ----------------------\n";
    
    int indexRow, indexColumn, rowDiff, columnDiff;
    
    for (indexRow = 1; indexRow <= BOARD_SIZE; ++indexRow)
    {
        // set width between sectors
        Serial.print << setw(3) << '\0';
        
        rowDiff = mRow - indexRow;
        for (indexColumn = 1; indexColumn <= BOARD_SIZE; ++indexColumn) 
        {
            if (indexRow == mRow && indexColumn == mColumn)
            {
                Serial.print << "<*>"; // display Enterprise and its position
                continue;
            }
            
            columnDiff = mColumn - indexColumn; 

            if (indexRow == mKlingonRow  && indexColumn == mKlingonColumn)
            {
                if (mKlingonQuadrantRow == mQuadrantRow && mKlingonQuadrantColumn == mQuadrantColumn)
                {
                    Serial.print << "+K+"; // display a Klingon and its position
                    if (mKlingonEnergy == 0)
                    {
                        mKlingonRow = 0;
                        mKlingonColumn = 0;
                    }
                    continue;
                }
            }

            if (indexRow == mStarbaseRow  && indexColumn == mStarbaseColumn)
            {
                if (mStarbaseQuadrantRow == mQuadrantRow && mStarbaseQuadrantColumn == mQuadrantColumn)
                {
                    Serial.print << ">!<"; // display a Starbase and its position
                    continue;
                }
            }

            if ((indexColumn + indexRow)%2 == 0)
            {
                Serial.print << " . ";
            }
            else
            {
                Serial.print << " . ";
            }
        }

        for (indexColumn = 1; indexColumn <= BOARD_SIZE; ++indexColumn) 
        {
            if (indexRow == 1)
            {
                Serial.print << "      STARDATE            3000";
                break;
            }
            if (indexRow == 2)
            {
                if (mQuadrantRow == mKlingonQuadrantRow && mQuadrantColumn == mKlingonQuadrantColumn)
                {
                    Serial.print << "      CONDITION           RED";
                }
                else
                {
                    Serial.print << "      CONDITION           GREEN";
                }
                break;
            }
            if (indexRow == 3)
            {
                Serial.print << "      QUADRANT            " << mQuadrantRow << " , " << mQuadrantColumn;
                break;
            }
            if (indexRow == 4)
            {
                Serial.print << "      SECTOR              " << mRow << " , " << mColumn;
                break;
            }
            if (indexRow == 5)
            {
                Serial.print << "      PHOTON TORPEDOES    " << mTorpedo;
                break;
            }
            if (indexRow == 6)
            {
                Serial.print << "      TOTAL ENERGY        " << mEnergy;
                break;
            }
            if (indexRow == 7)
            {
                Serial.print << "      SHIELDS             " << mShields;
                break;
            }
            if (indexRow == 8)
            {
                Serial.print << "      KLINGONS REMAINING  " << mKlingonsRemaining;
                break;
            }
        }
        Serial.print << endl;
    }
    Serial.print << "   ----------------------\n";
    Serial.print << "   STARBASES 1    STARS 3\n";
    Serial.print << endl;
}

void StarTrek::instructions()
{
    Serial.print << "\nDO NOT TYPE IN ANY KEY AND HIT ENTER TO RECEIVE INSTRUCTIONS FOR 'SUPER STAR TREK'\n";
    cin.get();

    Serial.print << "\n1. WHEN YOU SEE \\COMMAND ?\\ PRINTED, ENTER ONE OF THE LEGAL "
            "\n     COMMANDS (NAV,SRS,LRS,PHA,TOR,SHE,DAM,COM, OR XXX)."
            "\n2. IF YOU SHOULD TYPE IN AN ILLEGAL COMMAND, YOU'LL GET A SHORT "
            "\n     LIST OF THE LEGAL COMMANDS PRINTED OUT."
            "\n3. SOME COMMANDS REQUIRE YOU TO ENTER DATA (FOR EXAMPLE, THE "
            "\n     'NAV' COMMAND COMES BACK WITH 'COURSE (1-9 ?'.) IF YOU "
            "\n     TYPE IN ILLEGAL DATA (LIKE NEGATIVE NUMBERS), THAT COMMAND "
            "\n     WILL BE ABORTED";
    cin.get();

    Serial.print << "\n     THE GALAXY IS DIVIDED INTO AN 8 X 8 QUADRANT GRID, "
            "\nAND EACH QUADRANT IS FURTHER DIVIDED INTO AN 8 X 8 SECTOR GRID.\n";
            
    Serial.print << "\n     YOU WILL BE ASSIGNED A STARTING POINT SOMEWHERE IN THE "
            "\nGALAXY TO BEGIN A TOUR OF DUTY AS COMMAND OF THE STARSHIP "
            "\n\\ENTERPRISE\\. YOUR MISSION: TO SEEK AND DESTROY THE FLEET OF "
            "\nKLINGON WARSHIPS WHICH ARE MENACING THE UNITED FEDERATION OF "
            "\nPLANETS.\n";

    Serial.print << "\n     YOU HAVE THE FOLLOWING COMMANDS AVAILABLE TO YOU AS CAPTAIN "
            "\nOF THE STARSHIP ENTERPRISE:";
    cin.get();

    Serial.print << "\n\\NAV\\ COMMAND = WARP ENGINE CONTROL --"
            "\n     COURSE IS IN A CIRCULAR NUMERICAL      4  3  2"
            "\n     VECTOR ARRANGEMENT AS SHOWN             . . ."
            "\n     INTEGER AND REAL VALUES MAY BE           ..."
            "\n     USED. (THUS COURSE 1.5 IS HALF-      5 ---+--- 1"
            "\n     WAY BETWEEN 1 AND 2)                     ..."
            "\n                                             . . ."
            "\n     VALUES MAY APPROACH 9.0, WHICH         6  7  8"
            "\n     ITSELF IS EQUIVALENT TO 1.0"
            "\n                                            COURSE"
            "\n     ONE WAR FACTOR IS THE SIZE OF"
            "\n     ONE QUADRANT. THEREFORE, TO GET "
            "\n     FROM QUADRANT 6,5 TO 5,5, YOU WOULD "
            "\n     USE COURSE 3 WARP FACTOR 1.";
    cin.get();

    Serial.print << "\n\\SRS\\ COMMAND = SHORT RANGE SENSOR SCAN"
            "\n     SHOW YOU A SCAN OF YOUR PRESENT QUADRANT.\n"

            "\n     SYMBOLOGY ON YOUR SENSOR SCREEN IS AS FOLLOWS:"
            "\n        -E- = YOUR STARSHIP'S POSITION"
            "\n        +K+ = KLINGON BATTLE CRUISER"
            "\n        <*> = FEDERATION STARBASE (REFUEL/REPAIR/RE-ARM HERE!)"
            "\n         *  = STAR\n"

            "\n     VALUES MAY APPROACH 9.0, WHICH "
            "\n     ITSELF IS EQUIVALENT TO 1.0\n"

            "\n     ONE WAR FACTOR IS THE SIZE OF"
            "\n     ONE QUADRANT. THEREFORE, TO GET "
            "\n     FROM QUADRANT 6,5 TO 5,5, YOU WOULD "
            "\n     USE COURSE 3 WARP FACTOR 1.\n"

            "\n     A CONDENSED 'STATUS REPORT' WILL ALSO BE PRESENTED.";
    cin.get();

    Serial.print << "\n\\LRS\\ COMMAND = LONG RANGE SENSOR SCAN"
            "\n     SHOWS CONDITIONS IN SPACE FOR ONE QUADRANT ON EACH SIDE"
            "\n     THE SCAN IS CODED IN THE FORM \\###\\, WHERE THE UNITS DIGIT"
            "\n     IS THE NUMBER OF STARS, THE TENS DIGIT IS THE NUMBER OF"
            "\n     STARBASES, AND THE HUNDREDS DIGIT IS THE NUMBER OF"
            "\n     KLINGONS.\n"

            "\n     EXAMPLE - 207 = 2 KLINGONS, NO STARBASES, & 7 STARS.";
    cin.get();

    Serial.print << "\n\\PHA\\ COMMAND = PHASER CONTROL."
            "\n     ALLOWS YOU TO DESTROY THE KLINGON BATTLE CRUISERS BY"
            "\n     ZAPPING THEM WITH SUITABLY LARGE UNITS OF ENERGY TO"
            "\n     DEPLETE THEIR SHIELD POWER. (REMEMBER, KLINGONS HAVE"
            "\n     PHASERS TOO!)";
    cin.get();

    Serial.print << "\n\\TOR\\ COMMAND = PHOTON TORPEDO CONTROL"
            "\n     TORPEDO COURSE IS THE SAME AS USED IN WAR ENGINE CONTROL."
            "\n     IF YOU HIT THE KLINGON VESSEL, HE IS DESTROYED AND"
            "\n     CANNOT FIRE BACK AT YOU. IF YOU MISS, YOU ARE SUBJECT TO"
            "\n     HIS PHASER FIRE. IN EITHER CASE, YOU ARE ALSO SUBJECT TO"
            "\n     THE PHASER FIRE OF ALL OTHER KLINGONS IN THE QUADRANT\n"
            
            "\n     THE LIBRARY COMPUTER (\\COM\\ COMMAND) HAS AN OPTION TO"
            "\n     COMPUTE TORPEDO TRAJECTORY FOR YOU (OPTION 2)";
    cin.get();

    Serial.print << "\n\\SHE\\ COMMAND = SHIELD CONTROL"
            "\n     DEFINES THE NUMBER OF ENERGY UNITS TO BE ASSINGED TO THE"
            "\n     SHIELDS. ENERGY IS TAKEN FROM TOTAL SHIP'S ENERGY. NOTE "
            "\n     THAT THE STATUS DISPLAY TOTAL ENERGY INCLUDES SHIELD ENERGY";
    cin.get();

    Serial.print << "\n\\DAM\\ COMMAND = DAMAGE CONTROL REPORT"
            "\n     GIVES THE STATE OF REPAIR OF ALL DEVICES. WHERE A NEGATIVE"
            "\n     'STATE OF REPAIR' SHOWS THAT THE DEVICE IS TEMPORARILY"
            "\n     DAMAGED.";
    cin.get();

    Serial.print << "\n\\COM\\ COMMAND = LIBRARY-COMPUTER"
            "\nTHE LIBRARY-COMPUTER CONTAINS SIX OPTIONS:"
            "\nOPTION 0 = CUMULATIVE GALACTIC RECORD"
            "\n     THIS OPTION SHOWS COMPUER MEMORY OF THE RESULTS OF ALL"
            "\n     PREVIOUS SHORT AND LONG RANGE SENSOR SCANS"
            "\nOPTION 1 = STATUS REPORT"
            "\n     THIS OPTION SHOWS THE NUMBER OF KLINGONS, STARDATES,"
            "\n     AND STARBASES REMAINING IN THE GAME."
            "\nOPTION 2 = PHOTON TORPEDO DATA"
            "\n     WHICH GIVES DIRECTIONS AND DISTANCE FROM THE ENTERPRISE"
            "\n     TO ALL KLINGONS IN YOUR QUADRANT"
            "\nOPTION 3 = STARBASE NAV DATA"
            "\n     THIS OPTION GIVES DIRECTION AND DISTANCE TO ANY"
            "\n     STARBASE WITHIN YOUR QUADRANT"
            "\nOPTION 4 = DIRECTION/DISTANCE CALCULATOR"
            "\n     THIS OPTION ALLOWS YOU TO ENTER COORDINATES FOR"
            "\n     DIRECTION/DISTANCE CALCULATIONS"
            "\nOPTION 5 = GALACTIC /REGION NAME/ MAP"
            "\n     THIS OPTION PRINTS THE NAMES OF THE SIXTEEN MAJOR"
            "\n     GALACTIC REGIONS REFERRED TO IN THE GAME.";
    cin.get();
}

void StarTrek::introduction()
{
    Serial.print << "\n\n\n                ,------*------,\n"
            ",-------------   '---  ------'\n"
            "'--------- --'      / /\n"
            "     ,---' '-------/ /--,\n"
            "      '-----------------'\n\n"

            "THE USS ENTERPRISE === NCC-1701\n\n\n\n\n\n"

            "YOUR ORDERS ARE AS FOLLOWS:"
            "\n     DESTROY THE 8 KLINGON WARSHIPS WHICH HAVE INVADED"
            "\nTHE GALAXY BEFORE THEY CAN ATTACK FEDERATION HEADQUARTERS"
            "\nON STARDATE 3025. THIS GIVES YOU 25 DAYS. THERE ARE"
            "\n3 STARBASES IN THE GALAXY FOR RESUPPLYING YOUR SHIP.\n"

            "\nHIT ANY KEY EXCEPT RETURN WHEN READY TO ACCEPT COMMAND."

            "\n\nYOUR MISSION BEGINS WITH YOUR STARSHIP LOCATED"
            "\nIN THE GALACTIC QUADRANT, 'BETELGEUSE I'.\n\n";
    cin.get();

    // set default starting position
    mQuadrantRow = 4;
    mQuadrantColumn = 5;
    mRow = 6;
    mColumn = 2;

    mKlingonQuadrantRow = 4;
    mKlingonQuadrantColumn = 5;
    mKlingonRow = 2;
    mKlingonColumn = 7;

    mStarbaseQuadrantRow = 4;
    mStarbaseQuadrantColumn = 5;
    mStarbaseRow = 7;
    mStarbaseColumn = 7;
}

void StarTrek::titleCard()
{
    const char YES = 'Y';
    const char NO = 'N';
    char manual;

    Serial.print << "          *************************************\n"
            "          *                                   *\n"
            "          *                                   *\n"
            "          *      * * SUPER STAR TREK * *      *\n"
            "          *                                   *\n"
            "          *                                   *\n"
            "          *************************************\n";

    while(manual != YES && manual != NO)
    {
        Serial.print << "\n\nDO YOU NEED INSTRUCTIONS (Y/N) ? ";
        cin >> manual;
        manual = toupper(manual);
        if (manual == YES)
        {
            instructions();
        }
    }
}
