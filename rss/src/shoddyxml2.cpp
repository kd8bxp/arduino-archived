#include "shoddyxml2.h"
#include <stdio.h>
#include <string.h>
#include <Arduino.h>

/* public methds */
shoddyxml::shoddyxml() {
  resetStatus();
}

void shoddyxml::init(int started) {
  this->xmlStarted = started;
}

void shoddyxml::resetStatus() {
  status = INXML;
  subStatus = 0;

  sbPosition = 0;

  for (int i = 0; i < numAttributes; i++) {
    if (attributes[i].name) {
      free(attributes[i].name);
    }
    if (attributes[i].attValue) {
      free(attributes[i].attValue);
    }
  }
  numAttributes = 0;
}

void shoddyxml::parse(int c) {
  switch (status) {
    case INXML:
      parseInXML(c);
      break;
    case LEFTBRACKET:
      parseLeftBracket(c);
      break;
    case MAYBELBEX:
      parseMayBeLBEx(c);
      break;
    case INPI:
      parseInPI(c);
      break;
    case INSTAG:
      parseInSTag(c);
      break;
    case INETAG:
      parseInETag(c);
      break;
    case INSECTION:
      parseInSection(c);
      break;
    case INCDATA:
      parseInCDATA(c);
      break;
    case INCHARACTER:
      parseInCharacter(c);
      break;
    case INCOMMENT:
      parseInComment(c);
      break;
    case INELEMENT:
      parseInElement(c);
      break;
    case MAYBECOMMENT:
      parseMayBeComment(c);
    default:
      break;
  }
}

void shoddyxml::parse() {
  int c;

  while ((c = getChar()) != EOF) {
    parse(c);
  }
}

void shoddyxml::parseInXML(int c) {
  if (c == '<') {
    status = LEFTBRACKET;
  } else if (strchr(" \t\r\n", c) == NULL) {
    status = INCHARACTER;
    parseInCharacter(c);
  } else {
    /* do nothing */
  }
}

void shoddyxml::parseLeftBracket(int c) {  // <
  if (c == '?') {
    status = INPI;
  } else if (c == '/') {
    status = INETAG;
  } else if (c == '!') {
    status = MAYBELBEX;
  } else {
    status = INSTAG;
    parseInSTag(c);
  }
}

void shoddyxml::parseMayBeLBEx(int c) {  // <!
  if (c == '[') {
    status = INSECTION;
  } else if (c == '-') {
    status = MAYBECOMMENT;
  } else {
    status = INELEMENT;
    parseInElement(c);
  }
}

void shoddyxml::parseMayBeComment(int c) { // <!-
  if (c == '-') {
    status = INCOMMENT;
  } else {
    status = INSTAG;
    parseInSTag('!');
    parseInSTag('-');
    parseInSTag(c);
  }
}

void shoddyxml::parseInPI(int c) {
  switch (subStatus) {
    case 0:
      if (c != '?') {
        if (strchr(" \t\r\n", c) == NULL) {
          stringBuffer[sbPosition++] = c;
        } else {
          stringBuffer[sbPosition] = 0;
          subStatus = 1;
          if (strcmp(stringBuffer, "xml") == 0) {
            xmlStarted = 1;
            foundXMLDecl();
          } else {
            if (xmlStarted) {
              foundPI(stringBuffer);
            }
          }
        }
      } else {
        subStatus = 2;
      }
      break;
    case 1:
      if (c == '?') {
        subStatus = 2;
      } else {
        /* shoddy */
      }
      break;
    case 2:
      if (c == '>') {
        resetStatus();
      } else {
        stringBuffer[sbPosition++] = '?';
        stringBuffer[sbPosition++] = c;
        subStatus = 0;
      }
      break;
    default:
      break;
  }
}

void shoddyxml::parseInSTag(int c) { // <
  static int nameLength = 1;
  static int attValueLength = 0;
  static char *name = NULL;
  static char *attValue = NULL;

  switch (subStatus) {
    case 0: // <
      if (c == '>') {  // <stag>
        stringBuffer[sbPosition] = 0;
        if (xmlStarted) {
          foundSTag(stringBuffer, 0, NULL);
        }
        resetStatus();
      } else if (c == '/') {
        subStatus = 1;
      } else if (strchr(" \t\r\n", c) != NULL) { // found S
        stringBuffer[sbPosition++] = 0;
        sbPosition = 0;
        subStatus = 99;
      } else {
        stringBuffer[sbPosition++] = c;
      }
      break;
    case 1: // /
      if (c == '>') { // <stag/>
        stringBuffer[sbPosition] = 0;
        if (xmlStarted) {
          foundEmptyElemTag(stringBuffer, 0, NULL);
        }
        resetStatus();
      } else {
        stringBuffer[sbPosition++] = '/';
        stringBuffer[sbPosition++] = c;
        subStatus = 0;
      }
      break;
    case 2: // attribute name
      if (c == '=') {
        subStatus = 3;
        name[nameLength - 1] = 0;
      } else if (c == '/') {
        subStatus = 5;
      } else if (c == '>') {  // <stag name="attValue">
        if (xmlStarted) {
          foundSTag(stringBuffer, numAttributes, attributes);
          resetStatus();
        }
      } else {
        name = (char *)realloc(name, ++nameLength);
        name[nameLength - 2] = c;
      }
      break;
    case 3: // Eq
      if (c == '\"') {
        subStatus = 4;
      }
      break;
    case 4: // AttValue
      attValue = (char *)realloc(attValue, ++attValueLength);
      if (c == '\"') {
        attValue[attValueLength - 1] = 0;
        attributes[numAttributes].name = strdup(name);
        attributes[numAttributes].attValue = strdup(attValue);
        numAttributes++;
        free(name);
        name = 0;
        nameLength = 1;

        free(attValue);
        attValue = 0;
        attValueLength = 0;

        subStatus = 99;
      } else {
        attValue[attValueLength - 1] = c;
      }
      break;
    case 5:
      if (c == '>') { // <stag name="attValue" />
        if (xmlStarted) {
          foundEmptyElemTag(stringBuffer, numAttributes, attributes);
        }
        resetStatus();
      } else {
        /* shoddy */
      }
      break;
    case 99: /* skip char */
      if (strchr(" \t\r\n", c) == NULL) {
        subStatus = 2;
        parseInSTag(c);
      }
      break;
    default:
      break;
  }
}

void shoddyxml::parseInETag(int c) {
  if (c == '>') {
    stringBuffer[sbPosition] = 0;
    if (xmlStarted) {
      foundETag(stringBuffer);
    }
    resetStatus();
  } else if (strchr(" \t\r\n", c) != NULL) {
    stringBuffer[sbPosition] = 0;
    sbPosition = 0;
    subStatus = 1;
  } else {
    if (subStatus == 0) {
      stringBuffer[sbPosition++] = c;
    }
  }
}

void shoddyxml::parseInLBEx(int c) {

}

void shoddyxml::parseInSection(int c) {
  switch (subStatus) {
    case 0:
      if (c == '[') {
        stringBuffer[sbPosition] = 0;
        sbPosition = 0;
        if (strcmp(stringBuffer, "CDATA") == 0) {
          subStatus = 0;
          status = INCDATA;
        } else {
          subStatus = 1;
        }
      } else {
        stringBuffer[sbPosition++] = c;
      }
      break;
    case 1:
      if (c == ']') {
        subStatus = 2;
      } else {
        stringBuffer[sbPosition++] = c;
        subStatus = 0;
      }
      break;
    case 2:
      if ( c == ']') {
        subStatus = 3;
      } else {
        stringBuffer[sbPosition++] = ']';
        stringBuffer[sbPosition++] = c;
        subStatus = 0;
      }
      break;
    case 3:
      if (c == '>') {
        if (xmlStarted) {
          foundSection(stringBuffer);
        }
        resetStatus();
      } else {
        stringBuffer[sbPosition++] = ']';
        stringBuffer[sbPosition++] = ']';
        stringBuffer[sbPosition++] = c;
        subStatus = 0;
      }
      break;
    default:
      break;
  }
}

void shoddyxml::parseInCDATA(int c) {
  switch (subStatus) {
    case 0:
      if (c == ']') {
        subStatus = 1;
      } else {
        if (xmlStarted) {
          foundCharacter(c);
        }
      }
      break;
    case 1:
      if (c == ']') {
        subStatus = 2;
      } else {
        if (xmlStarted) {
          foundCharacter(']');
          foundCharacter(c);
        }
        subStatus = 0;
      }
      break;
    case 2:
      if (c == '>') {
        resetStatus();
      } else if (c == '&') {
        subStatus = 3;
      } else {
        if (xmlStarted) {
          foundCharacter(']');
          foundCharacter(']');
          foundCharacter(c);
        }
        subStatus = 0;
      }
      break;
    case 3:
      if (c == 'g') {
        subStatus = 4;
      } else {
        if (xmlStarted) {
          foundCharacter('&');
          foundCharacter(c);
        }
        subStatus = 0;
      }
      break;
    case 4:
      if (c == 't') {
        subStatus = 5;
      } else {
        if (xmlStarted) {
          foundCharacter('&');
          foundCharacter('g');
          foundCharacter(c);
        }
        subStatus = 0;
      }
      break;
    case 5:
      if (c == ';') {
        resetStatus();
      } else {
        if (xmlStarted) {
          foundCharacter('&');
          foundCharacter('g');
          foundCharacter('t');
          foundCharacter(c);
        }
        subStatus = 0;
      }
      break;
    default:
      break;
  }
}

void shoddyxml::parseInCharacter(int c) {
  if (c == '<') {
    resetStatus();
    parseInXML(c);
  } else {
    if (xmlStarted) {
      foundCharacter(c);
    }
  }
}

void shoddyxml::parseInComment(int c) {
  switch (subStatus) {
    case 0:
      if (c == '-') {
        subStatus = 1;
      }
      break;
    case 1:
      if (c == '-') {
        subStatus = 2;
      } else {
        subStatus = 0;
      }
      break;
    case 2:
      if (c == '>') {
        resetStatus();
      }
      subStatus = 0;
      break;
    default:
      break;
  }
}

void shoddyxml::parseInElement(int c) { // <!
  switch (subStatus) {
    case 0: // <
      if (c == '>') {
        stringBuffer[sbPosition] = 0;
        if (xmlStarted) {
          foundElement(stringBuffer);
        }
        resetStatus();
      } else if (strchr(" \t\r\n", c) != NULL) { // found S
        stringBuffer[sbPosition++] = 0;
        sbPosition = 0;
        subStatus = 1;
      } else {
        stringBuffer[sbPosition++] = c;
      }
      break;
    case 1: // tagname
      if (c == '>') {
        if (xmlStarted) {
          foundElement(stringBuffer);
        }
        resetStatus();
      } else if ( c == '/') {
        subStatus = 2;
      } else { // attribute
        /* shoddy */
      }
      break;
    case 2:
      if (c == '>') {
        if (xmlStarted) {
          foundElement(stringBuffer);
        }
        resetStatus();
      } else {
        /* shoddy */
      }
      break;
    default:
      break;
  }
}
