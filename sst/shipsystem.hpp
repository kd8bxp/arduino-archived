#ifndef shipsystem_hpp
#define shipsystem_hpp

//////////////////////
// Enterprise Class //
//////////////////////
class EnterpriseSystems
{
    public:
        virtual void navigation(){}
        virtual void shortscan(){}
        virtual void longscan(){}
        virtual void phasers(){}
        virtual void torpedoes(){}
        virtual void shields(){}
        virtual void damagecont(){}
        virtual void librarycomp(){}
        virtual void resigncommand(){}
        virtual void help(){}
};

class EnterpriseNavigation: public EnterpriseSystems
{
    public:
        void navigation();
};

class EnterpriseShortScan: public EnterpriseSystems
{
    public:
        void shortscan();
};

class EnterpriseLongScan: public EnterpriseSystems
{
    public:
        void longscan();
};

class EnterprisePhasers: public EnterpriseSystems
{
    public:
        void phasers();
};

class EnterpriseTorpedoes: public EnterpriseSystems
{
    public:
        void torpedoes();
};

class EnterpriseShields: public EnterpriseSystems
{
    public:
        void shields();
};

class EnterpriseDamageCont: public EnterpriseSystems
{
    public:
        void damagecont();
};

class EnterpriseLibraryComp: public EnterpriseSystems
{
    public:
        void librarycomp();
};

class EnterpriseResignCommand: public EnterpriseSystems
{
    public:
        void resigncommand();
};

class EnterpriseHelp: public EnterpriseSystems
{
    public:
        void help();
};

#endif