#include "starTrek.hpp"
#include "shipsystem.hpp"
//#include <iomanip>
//#include <iostream>
//#include <string>
#include <unistd.h>

using namespace std;

void EnterpriseNavigation::navigation()
{
    Serial.println("Navigation");
}

void EnterpriseShortScan::shortscan()
{
    Serial.println("Short Range Scan");
}

void EnterpriseLongScan::longscan()
{
    Serial.println("Long Range Scan");
}

void EnterprisePhasers::phasers()
{
    Serial.println("Phasers");
}

void EnterpriseTorpedoes::torpedoes()
{
    Serial.println("Torpedoes");
}

void EnterpriseShields::shields()
{
    Serial.println("Shields");
}

void EnterpriseDamageCont::damagecont()
{
    Serial.println("Damage Control");
}

void EnterpriseLibraryComp::librarycomp()
{
    Serial.println("Library Computer");
}

void EnterpriseResignCommand::resigncommand()
{
    Serial.println("Resign Command");
}

void EnterpriseHelp::help()
{
    Serial.println ("Help");
}
