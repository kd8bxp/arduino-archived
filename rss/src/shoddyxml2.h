#pragma once

typedef struct {
  char *name;
  char *attValue;
} attribute_t;

class shoddyxml {
  public:
    shoddyxml();

    void init(int started = 0);
    void parse(int c);
    void parse();

    char stringBuffer[32];
    attribute_t attributes[16];
    int numAttributes;

    /* user supplied functions */
    virtual int getChar() = 0;
    virtual void foundXMLDecl() = 0;
    virtual void foundPI(char *s) = 0;
    virtual void foundSTag(char *s, int numAttributes, attribute_t attributes[]) = 0;
    virtual void foundETag(char *s) = 0;
    virtual void foundEmptyElemTag(char *s, int numAttributes, attribute_t attributes[]) = 0;
    virtual void foundSection(char *s) = 0;
    virtual void foundCharacter(char c) = 0;
    virtual void foundElement(char *s) = 0;

  private:
    enum status_t {
      INXML, LEFTBRACKET, MAYBELBEX,
      INPI, INSTAG, INETAG, INLBEX,
      INSECTION, INCDATA, INCHARACTER, MAYBECOMMENT, INCOMMENT, INELEMENT
    } status;
    int subStatus;

    int xmlStarted;
    int sbPosition;

    void resetStatus();

    void parseInXML(int c);
    void parseLeftBracket(int c);
    void parseMayBeLBEx(int c);
    void parseMayBeComment(int c);
    void parseInPI(int c);
    void parseInSTag(int c);
    void parseInETag(int c);
    void parseInLBEx(int c);
    void parseInSection(int c);
    void parseInCDATA(int c);
    void parseInCharacter(int c);
    void parseInComment(int c);
    void parseInElement(int c);
};
