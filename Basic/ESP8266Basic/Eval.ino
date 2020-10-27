#define TFT_DEMO
#include "expression_parser_string.h"

extern char* _parser_error_msg;

String evaluate(String expr)
{
  //int status;
  //  expr = GetRidOfurlCharacters(expr);
  //  Serial.print("eval function ");
  //  Serial.print(expr);
  //  Serial.print("---");
  // check if the expression is empty; in that case returns an empty string
  if (expr == "")
  {
    return "";
  }
  delay(0);
  parser_result = parse_expression_with_callbacks( expr.c_str(), variable_callback, function_callback, NULL, &numeric_value, string_value  );
  if (_parser_error_msg != NULL)
  {
    PrintAndWebOut(String(_parser_error_msg));
    return F("error");
  }
  if (parser_result == PARSER_STRING)
    return string_value;
  else
    return FloatToString(numeric_value);

}





/**
 @brief user-defined variable callback function. see expression_parser.h for more details.
 @param[in] user_data pointer to any user-defined state data that is required, none in this case
 @param[in] name name of the variable to look up the value of
 @param[out] value output point to double that holds the variable value on completion
 @param[out] value output point to string that holds the variable value on completion
 @return PARSER_TRUE if the variable exists and value was set by the callback with the double-prevision result stored in value or
         PARSER_STRING if the variable exists and value was set by the callback with the string result stored in value_str or
         PARSER_FALSE otherwise.
*/
int variable_callback( void *user_data, const char *name, float *value, String *value_str  ) {
  // look up the variables by name

  VariableLocated = 0;




  String Name = String(name);
  delay(0);
  for (int i = 0; i < TotalNumberOfVariables; i++)
  {
      if (AllMyVariables[i].getName() == Name)
    {
      LastVarNumberLookedUp = i;
        *value =  atof(AllMyVariables[i].getVar().c_str());
        
        *value_str = AllMyVariables[i].getVar();
        //Serial.print("Variable "); Serial.print(Name); Serial.print(AllMyVaribles_format[i]);
        VariableLocated = 1;
        return AllMyVariables[i].Format; // returns the format of the variable : PARSER_TRUE if numeric, PARSER_STRING if string
    }
  }
  Name.toUpperCase();
  // now look for the constants
  if (Name == F("D0")) { *value = 16; return PARSER_TRUE;}
  if (Name == F("D1")) { *value =  5; return PARSER_TRUE;}
  if (Name == F("D2")) { *value =  4; return PARSER_TRUE;}
  if (Name == F("D3")) { *value =  0; return PARSER_TRUE;}
  if (Name == F("D4")) { *value =  2; return PARSER_TRUE;}
  if (Name == F("D5")) { *value = 14; return PARSER_TRUE;}
  if (Name == F("D6")) { *value = 12; return PARSER_TRUE;}
  if (Name == F("D7")) { *value = 13; return PARSER_TRUE;}
  if (Name == F("D8")) { *value = 13; return PARSER_TRUE;}
  if (Name == F("RX")) { *value =  3; return PARSER_TRUE;}
  if (Name == F("TX")) { *value =  1; return PARSER_TRUE;}

    
  // failed to find variable, return false
  *value = 0;
  *value_str = name;
  // against my will, I have been obliged to put a PARSER_STRING here instead of PARSER_FALSE; this will inhibit the error message if the
  // variable is not existing permitting to use any variable not initialised; :-)
  return PARSER_STRING;
}

/**
 @brief user-defined function callback. see expression_parser.h for more details.
 @param[in] user_data input pointer to any user-defined state variables needed.  in this case, this pointer is the maximum number of arguments allowed to the functions (as a contrived example usage).
 @param[in] name name of the function to evaluate
 @param[in] num_args number of arguments that were parsed in the function call
 @param[in] args list of parsed arguments (double precision numbers) - if the arg is not valid, the value is nan
 @param[out] value output evaluated result of the function call (double precision number)
 @param[in] args list of parsed arguments (strings) - if the arg is not valid, the value is '\0'
 @param[out] value output evaluated result of the function call (string)
 @return PARSER_TRUE if the function exists and was evaluated successfully with the double-prevision result stored in value or
         PARSER_STRING if the function exists and was evaluated successfully with the string result stored in value_str or
         PARSER_FALSE otherwise.
*/
int function_callback( void *user_data, const char *name, const int num_args, const float *args, float *value, String **args_str, String *value_str ) {
  int i, max_args;
  float tmp;
  String fname = String(name);
  fname.toLowerCase();
  delay(0);

  // example to show the user-data parameter, sets the maximum number of
  // arguments allowed for the following functions from the user-data function
  max_args = *((int*)user_data);


  // this is in first position as it is required to be as fast as possible
  if ( fname == F("udpread") && num_args == 0 ) {
    // function udpread()
    //Serial.println(UdpBuffer);
    *value_str = String(UdpBuffer);
    //UdpBuffer = ""; // clear the variable after the read
    return PARSER_STRING;
  }
  else if ( fname == F("udpremote") && num_args == 0 ) {
    // function udpremote()
    *value_str = UdpRemoteIP.toString() + String(F(":")) + String(UdpRemotePort);
    return PARSER_STRING;
  }
  else if ( fname == F("millis") && num_args == 0 ) // the function is millis()
  {
    // set return value and return true
    *value = millis();
    return PARSER_TRUE;
  }
  else if ( fname == F("ramfree") && num_args == 0 ) // the function is ramfree()
  {
    // set return value and return true
    *value = ESP.getFreeHeap();
    return PARSER_TRUE;
  }
  else if ( fname == F("rnd") && num_args == 1 ) // the function is rnd(seed)
  {
    // set return value and return true
    randomSeed(millis());
    *value = random(args[0]);
    return PARSER_TRUE;
  }
  else if ( fname == F("flashfree") && num_args == 0 ) // the function is flashfree()
  {
    // set return value and return true
    struct FSInfo {
      size_t totalBytes;
      size_t usedBytes;
      size_t blockSize;
      size_t pageSize;
      size_t maxOpenFiles;
      size_t maxPathLength;
    };
    SPIFFS.info(fs_info);
    int Flashfree = fs_info.totalBytes - fs_info.usedBytes;
    *value = (float) Flashfree;
    return PARSER_TRUE;
  }
  else if ( fname == F("id") && num_args == 0 ) {
    // example of the id()
    // set return value
    *value_str = String(ESP.getChipId());
    return PARSER_STRING;
  }
  else if ( fname == F("version") && num_args == 0 ) {
    // example of the id()
    // set return value
    *value_str = String(BasicVersion);
    return PARSER_STRING;
  }
  else if ( fname == F("htmlid") && num_args == 0 ) {
    // example of the htmlid()
    // set return value
    *value_str = String(LastElimentIdTag);
    return PARSER_STRING;
  }

  else if ( fname == F("timesetup") && num_args > 0 ) {
    String bla;
    SaveDataToFile("TimeZone", String(args[0]));
    SaveDataToFile("DaylightSavings", String(args[1]));
    configTime(LoadDataFromFile("TimeZone").toFloat() * 3600, LoadDataFromFile("DaylightSavings").toInt(), "pool.ntp.org", "time.nist.gov");
    *value_str = "";
    return PARSER_STRING;
  }
  else if ( fname == F("mid") && num_args == 2 ) {
    // example of the mid(string, start)
    // set return value
    if (args_str[0] != NULL)  // we should trigger an error if the argument is not a string
    {
      *value_str = args_str[0]->substring((int) args[1] - 1);
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("MID() : The first argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("mid") && num_args == 3 ) {
    // example of the mid(string, start, end)
    // set return value
    if (args_str[0] != NULL) 
    {
      *value_str = args_str[0]->substring((int) args[1] - 1, (int) (args[1] + args[2]) - 1 );
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("MID() : The first argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("right") && num_args == 2 ) {
    // example of the right(string, length)
    // set return value
    if (args_str[0] != NULL) 
    {
      *value_str = args_str[0]->substring(args_str[0]->length() - (int) args[1]);
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("RIGHT() : The first argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("left") && num_args == 2 ) {
    // example of the left(string, length)
    // set return value
    if (args_str[0] != NULL) 
    {    
      *value_str = args_str[0]->substring(0, (int) args[1]);
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("LEFT() : The first argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("word") && num_args > 1 ) {
    // example of the word(string, length)
    // set return value
    char bla;
    if ( (args_str[0] == NULL) || ( (num_args == 3) && (args_str[2] == NULL)) )
    {
      PrintAndWebOut(F("WORD() : The first and 3rd argument must be string!"));
      return PARSER_FALSE;
    }
    if (num_args == 2) bla = ' ';
    if (num_args == 3) bla = *args_str[2]->c_str();
    *value_str = getValue(*args_str[0], bla  , (int) args[1] - 1 );
    *value = -1;
    return PARSER_STRING;
  }
  else if ( fname == F("len") && num_args == 1 ) {
    // example of the len(string)
    // set return value
    if (args_str[0] != NULL)  // we should trigger an error if the argument is not a string
    {
    *value  = args_str[0]->length();
    return PARSER_TRUE;
  }
    else {
      PrintAndWebOut(F("LEN() : The argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("upper") && num_args == 1 ) {
    // example of the upper(string)
    // set return value
    if (args_str[0] != NULL)  // we should trigger an error if the argument is not a string
    {
      args_str[0]->toUpperCase();
      *value_str  =  *args_str[0];
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("UPPER() : The argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("lower") && num_args == 1 ) {
    // example of the lower(string)
    // set return value
    if (args_str[0] != NULL)  // we should trigger an error if the argument is not a string
    {
      args_str[0]->toLowerCase();
      *value_str  =  *args_str[0];
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("LOWER() : The argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( (fname == F("instr") || fname == F("instrrev")) && ( (num_args == 2) || (num_args == 3) ) ) {
    // example of the instr(string, string)
    // set return value
    if ( (args_str[0] != NULL) && (args_str[1] != NULL))
    {
      if (fname == F("instr"))
      {
        i = (num_args == 3) ? args[2] - 1 : 0;
        *value  = args_str[0]->indexOf(*args_str[1], i) + 1;
      }
      else
      {
        i = (num_args == 3) ? args[2] - 1 : - 1;
        *value  = args_str[0]->lastIndexOf(*args_str[1], i) + 1;
      }
      return PARSER_TRUE;
    }
    else {
      PrintAndWebOut(F("INSTR() : Both arguments must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("hex") ) {
    if ( num_args == 1 ) {
      // example of the hex(value)
      // set return value
      *value_str  = String((int) args[0], HEX);
      return PARSER_STRING;
    }
    else
      if ( num_args == 2 ) {   // example of the hex(value, nb_digits)
        String tmp = "0000000" + String((int) args[0], HEX);
        *value_str = tmp.substring(tmp.length() - args[1]);
            return PARSER_STRING;
      }
  }
  else if ( fname == F("oct") && num_args == 1 ) {
    // example of the oct(value)
    // set return value
    *value_str  = String((int) args[0], OCT);
    return PARSER_STRING;
  }
  else if ( fname == F("int") && num_args == 1 ) {
    // example of the int(value)
    // set return value
    *value  = (int) args[0];
    return PARSER_TRUE;
  }
  else if ( fname == F("chr") | fname == F("chr$") && num_args == 1 ) {
    // example of the chr(value)
    // set return value
    *value_str  = String((char) args[0]);
    return PARSER_STRING;
  }
  else if ( fname == F("asc") && num_args == 1 ) {
    // example of the asc(string) -> return the ascii code of the 1st char
    // set return value
    if (args_str[0] != NULL)
    {
      *value = (*args_str[0])[0];
      return PARSER_TRUE;
    }
    else {
      PrintAndWebOut(F("ASC() : The argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("val") && num_args == 1 ) {
    // function val(string) -> return the numeric value of the string
    // set return value
    if (args_str[0] != NULL)
    {
      *value = (int) strtol( args_str[0]->c_str(), 0, 10);
      return PARSER_TRUE;
    }
    else {
      PrintAndWebOut(F("VAL() : The argument must be a string!"));
      return PARSER_FALSE;
    }
  }
      else if ( fname == F("hextoint") && num_args == 1 ) {
    // function hextoint(string) -> return the numeric value of the hex string
    // set return value
    if (args_str[0] != NULL)
    {
      *value = (int) strtol( args_str[0]->c_str(), 0, 16);
      return PARSER_TRUE;
    }
    else {
      PrintAndWebOut(F("HEXTOINT() : The argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("replace") && num_args == 3 ) {
    // example of the replace(string, string to search for, string to replacement for}
    // set return value
    if ( (args_str[0] != NULL) && (args_str[1] != NULL) && (args_str[2] != NULL) )
    {
      args_str[0]->replace(*args_str[1], *args_str[2]);
      *value_str = *args_str[0];
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("REPLACE() : All the arguments must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("str") && num_args == 1 ) {
    // example of str(number) converts the number to string
    // set return value
    *value_str = FloatToString(args[0]);
    return PARSER_STRING;
  }
  else if ( fname == F("wifi.scan") && num_args == 0 ) {
    // function wifi.scan() -> no arguments
    // set return value
    *value = WiFi.scanNetworks() + 1;
    return PARSER_TRUE;
  }
  else if ( fname == F("wifi.ssid") && num_args == 1 ) {
    // function wifi.ssid(number)
    // set return value
    *value_str =  String(WiFi.SSID(args[0] - 1));
    return PARSER_STRING;
  }
  else if ( fname == F("wifi.rssi") && num_args == 1 ) {
    // function wifi.rssi(number)
    // set return value
    *value_str =  String(WiFi.RSSI(args[0] - 1));
    return PARSER_STRING;
  }
  else if ( fname == F("ip") && num_args == 0 ) {
    // function wifi.scan() -> no arguments
    // set return value
    if (WiFi.localIP() == INADDR_NONE)
      *value_str =  String(WiFi.softAPIP().toString());
    else
      *value_str =  String(WiFi.localIP().toString());
    return PARSER_STRING;
  }
  else if ( fname == F("wget") && num_args > 0 ) {
    // function wget(url) or wget (url, port)
    // set return value
    if (args_str[0] != NULL)
    { 
      if (num_args == 1)  *value_str  =  FetchWebUrl(*args_str[0], 80);
      else if (num_args == 2 )   *value_str  =  FetchWebUrl(*args_str[0], args[1]);
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("WGET() : The first arguments must be a string!"));
      return PARSER_FALSE;
    }
  }

  else if ( fname == F("sendts") && num_args == 3 ) {
    // function sendts(url, field)
    // set return value
    if ( (args_str[0] != NULL) && (args_str[1] != NULL) && (args_str[2] != NULL) )
    {
      FetchWebUrl(String(F("api.thingspeak.com/update?key=")) + *args_str[0] + "&field" + *args_str[1] + "=" + *args_str[2], 80);
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("SENDTS() : All the arguments must be a string!"));
      return PARSER_FALSE;
    }
  }

  else if ( fname == F("readts") && num_args == 3 ) {
    // function readts(url, field)
    // set return value
    if ( (args_str[0] != NULL) && (args_str[1] != NULL) && (args_str[2] != NULL) )
    {
      String MyOut =  FetchWebUrl(String(F("api.thingspeak.com/channels/")) + *args_str[1] + "/field/" + *args_str[2] + "/last.xml?api_key=" + *args_str[0], 80);
      MyOut = MyOut.substring(MyOut.indexOf(String("<field" + *args_str[2] + ">") ) + 8, MyOut.indexOf(String("</field" + *args_str[2] + ">")));
      *value_str = MyOut;
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("READTS() : All the arguments must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("readopenweather") && num_args == 2 ) {
    // function readopenweather(url, index)
    // set return value
    if (args_str[0] != NULL)
    {
      *value_str  =  FetchOpenWeatherMapApi(*args_str[0], String(args[1]));
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("ReadOpenWeather() : The first argument must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("json") && num_args == 2 ) {
    // function json(buffer, key)
    // set return value
    if ( (args_str[0] != NULL) && (args_str[1] != NULL) )
    {
      *value_str  =  Parsifal(*args_str[0], *args_str[1]);
      return PARSER_STRING;
    }
    else {
      PrintAndWebOut(F("JSON() : Both arguments must be a string!"));
      return PARSER_FALSE;
    }
  }
  else if ( fname == F("io") && num_args > 0 ) {
    // function json(buffer, key)
    // set return value
    *value  =  UniversalPinIO(*args_str[0], String(args[1]),  args[2]);
    return PARSER_TRUE;
  }
  else if ( fname == F("neo.setup") && num_args == 1 ) {
    // function neosetup(pin)
    if (pixels != NULL)
      delete pixels;
    pixels = new Adafruit_NeoPixel(512, args[0], NEO_GRB + NEO_KHZ800);
    pixels->begin();
    return PARSER_STRING;
  }
  else if ( fname == F("neo") && num_args > 0 ) {
    // function neo(led no, r, g, b)
    if (pixels == NULL)
    {
      pixels = new Adafruit_NeoPixel(512, 15, NEO_GRB + NEO_KHZ800);
      pixels->begin();
    }
    pixels->setPixelColor(args[0], pixels->Color(args[1], args[2], args[3]));
    pixels->show();
    return PARSER_STRING;
  }
  else if ( fname == F("neostripcolor")| fname == F("neo.stripcolor") && num_args > 0 ) {
    // function neostripcolor(start, end, r, g, b)
    if (pixels == NULL)
    {
      pixels = new Adafruit_NeoPixel(512, 15, NEO_GRB + NEO_KHZ800);   
      pixels->begin();
    }
    for (int LedNo = args[0]; (LedNo <= args[1]) && (LedNo < 512); LedNo++) {
      pixels->setPixelColor(LedNo, pixels->Color(args[2], args[3], args[4]));
      delay(0);
    }
    pixels->show();
    return PARSER_STRING;
  }
  else if ( fname == F("neocls") |  fname == F("neo.cls") && num_args == 0 ) {
    // function neocls()
    // set return value
    if (pixels == NULL)
    {
      pixels = new Adafruit_NeoPixel(512, 15, NEO_GRB + NEO_KHZ800);       
      pixels->begin();
    }
    for (int LedNo = 0; LedNo < 512  ; LedNo++) {
      pixels->setPixelColor(LedNo, pixels->Color(0, 0, 0));
      delay(0);
    }
    pixels->show();
    return PARSER_STRING;
  }  
  else if ( fname == F("temp") && num_args > 0 ) {
    // function temp(sensor #)
    // set return value
    *value  = sensors.getTempCByIndex(args[0]);
    return PARSER_TRUE;
  }
  else if (fname.startsWith(F("dht.")) )      // block DHT functions; this reduces the number of compares  
  {
    fname = fname.substring(4); // skip the term i2c.   
    if ( fname == F("setup") ) // dht.setup(model, pin) -> model can be 11, 21, 22
  {
    // function dht.setup( model, pin)
    uint8_t pin;
    uint8_t model;
    switch ( num_args )
    {
      case 1:
        model = args[0];
        pin = 5;
        break;
      case 2:
        model = args[0];
        pin = args[1];
        break;
      default:
        model = 21; // default DHT21
        pin = 5; // default pin
        break;
    } 
    if (dht != NULL)
      delete dht;
    dht = new DHT(pin, model);
    dht->begin();
    // set return value
    return PARSER_TRUE;
  }
    else if ( fname == F("temp") ) {
    // function dht.temp()
    // set return value
    if (dht == NULL)
    {
      dht = new DHT(5, 21);
      dht->begin();
    }
    *value  = dht->readTemperature();
    return PARSER_TRUE;
  }
    else if ( fname == F("hum") ) {
    // function dht.hum()
    // set return value
    if (dht == NULL)
    {
      dht = new DHT(5, 21);
      dht->begin();
    }    
    *value  = dht->readHumidity();
    return PARSER_TRUE;
  }
    else if ( fname == F("heatindex") ) {
    // function dht.heatindex()
    // set return value
    if (dht == NULL)
    {
      dht = new DHT(5, 21);
      dht->begin();
    }    
    *value  = dht->computeHeatIndex(dht->readTemperature(), dht->readHumidity(), false);
    return PARSER_TRUE;
  }
  }
  else if (fname.startsWith(F("i2c.")) )      // block I2C functions; this reduces the number of compares  
  {
    fname = fname.substring(4); // skip the term i2c.  
    if ( fname == F("begin") && num_args == 1 ) {
    // function i2c.begin(address)
    // set return value
    Wire.beginTransmission((byte)args[0]);
    *value_str  =  String(F(""));
    return PARSER_STRING;
  }
    else if ( fname == F("write") && num_args == 1 ) {
    // function i2c.write(byte to be written)
    // set return value
    *value_str  =  String(Wire.write((byte)args[0]));
    return PARSER_STRING;
  }
    else if ( fname == F("end") && num_args == 0 ) {
    // function i2c.end()
    // set return value
    *value_str  =  String(Wire.endTransmission());
    return PARSER_STRING;
  }
    else if ( fname == F("requestfrom") && num_args > 0 ) {
    // function i2c.requestfrom(address, qty)
    // set return value
    *value_str  =  String(Wire.requestFrom((byte)args[0], (byte)args[1]));
    return PARSER_STRING;
  }

    else if ( fname == F("available") && num_args == 0 ) {
    // function i2c.available()
    // set return value
    *value_str  =  String(Wire.available());
    return PARSER_STRING;
  }

    else if ( fname == F("read") && num_args == 0 ) {
    // function i2c.read()
    // set return value
    *value_str  =  String(Wire.read());
    return PARSER_STRING;
  }
  }
  else if ( fname == F("htmlvar") && num_args > 0 ) {
    // function json(buffer, key)
    // set return value

    *value_str  = String(String(F("VARS|")) + String(LastVarNumberLookedUp));
    return PARSER_STRING;
  }
  else if ( fname == F("unixtime") || fname == F("time") ) {
    // function time(format) or unixtime(value, format)  - value can be string or number
    time_t now;
    String *ar =  args_str[0];
    if (fname == F("time"))
    {
      now = time(nullptr);
    }
    else
    {
      if (ar != NULL) // the input value is a string; we convert it to number before
        now = strtol( args_str[0]->c_str(), 0, 10);
      else
        now = args[0];
      ar =  args_str[1];
    }
    // set return value
    *value_str = String(ctime(&now));

    if (ar != NULL)
    {
      ar->toUpperCase();
      ar->trim();
      ar->replace(F("TIME"),  value_str->substring( 11, 19));
      ar->replace(F("DOW"),   value_str->substring(  0, 3));
      ar->replace(F("MONTH"), value_str->substring(  4, 7));
      ar->replace(F("DAY"),   value_str->substring(  8, 10));
      ar->replace(F("HOUR"),  value_str->substring( 11, 13));
      ar->replace(F("MIN"),   value_str->substring( 14, 16));
      ar->replace(F("SEC"),   value_str->substring( 17, 19));
      ar->replace(F("YEAR"),  value_str->substring( 20, 24));
      // set return value
      *value_str = *ar;
    }
    return PARSER_STRING;
  }
  else if ( fname == F("gpio1reset") && num_args == 0 )
  {
     // function gpi01reset()  -> restore the normal functionality of the pin GPIO1 (TX)
     pinMode(1, SPECIAL);
     return PARSER_STRING;
  }
  else if (fname.startsWith(F("spi.")) )      // block SPI functions; this reduces the number of compares  
  {
    fname = fname.substring(4); // skip the term spi.
    if ( fname == F("setup") && num_args > 0 ) {
    SPI.begin();
    switch(num_args)
    {
      case 1:  //spi.setup(speed)
        SPI.beginTransaction( SPISettings(args[0], MSBFIRST, SPI_MODE0)); 
        break;
      case 2:  //spi.setup(speed, SPI_MODE)
        SPI.beginTransaction( SPISettings(args[0], MSBFIRST, args[1])); 
        break;    
      case 3:  //spi.setup(speed, SPI_MODE, MSBFIRST)
        SPI.beginTransaction( SPISettings(args[0], args[2], args[1])); 
        break;                
    }
      // set return value
      *value = 0;
     return PARSER_TRUE;
  }
    else if ( fname == F("byte") && num_args == 1 ) {    // let a = spi.write(value_number) -> send and receive a  single byte
    *value = SPI.transfer(args[0]);
    return PARSER_TRUE;
  }
    else if ( fname == F("string") && num_args == 2 ) { // let a$ = spi.string(value_string, length) -> send and receive a buffer of 'length' bytes
    String ret;
    char c;
    ret.reserve(args[1]);
    for (i=0; i<args[1]; i++)
    {
      c = SPI.transfer(args_str[0]->c_str()[i]);
      ret.concat(c);
    }
    *value_str = ret;
    return PARSER_STRING;
  }  
    else if ( fname == F("hex") && num_args == 2 ) { // let a$ = spi.hex(value_string, length) -> send and receive a buffer of 'length' bytes; each byte is a 2 char hex string
    if (args_str[0] == NULL)  { PrintAndWebOut(F("spi.hex() : First argument must be a string!")); return PARSER_FALSE; }
    String ret;
    char c, t;
    String s;
    ret.reserve(args[1] * 2); // each received byte is 2 chars
    for (i = 0; i < (args[1] * 2); i += 2)
    {
      s = args_str[0]->substring(i , i + 2);
      t = (char) strtol( s.c_str(), 0, 16);
      c = SPI.transfer(t);
      s = String( c, HEX);
      if (s.length() == 1) s = "0" + s;
      ret.concat(s);
    }
    *value_str = ret;
    return PARSER_STRING;
  }    
  }
  else if ( fname == F("eval") && num_args == 1) {
    if (args_str[0] == NULL)  { PrintAndWebOut(F("eval() : The argument must be a string!")); return PARSER_FALSE; }
    evaluate(*args_str[0]);
    *value_str = string_value;
    *value = numeric_value;
    return parser_result;
  }
  else if (fname.startsWith(F("ir.")) )      // block Infrared functions; this reduces the number of compares
  {
      fname = fname.substring(3); // skip the term ir.
      if ( fname == F("send.setup") && num_args == 1 ) {
        // function ir.send.init(pin)   define the pin to be used for the irsend
        // set return value
        if (irsend != NULL)
          delete irsend;
        irsend = new IRsend(args[0]);
        irsend->begin();
        return PARSER_STRING;
      }
      else if ( fname == F("send.nec") && num_args == 2 ) {
        // function ir.send.nec(code, len)   send a NEC code with the first argument and the lenght on the 2nd
        if (args_str[0] == NULL)  { PrintAndWebOut(F("ir.send.nec() : The first argument must be a string!")); return PARSER_FALSE; }
        irsend->sendNEC(HexToLongInt(*args_str[0]), (int) args[1]);
        return PARSER_STRING;
      }
      else if ( fname == F("send.sony") && num_args == 2 ) {
        // function ir.send.nec(code, len)   send a SONY code with the first argument and the lenght on the 2nd
        if (args_str[0] == NULL)  { PrintAndWebOut(F("ir.send.sony() : The first argument must be a string!")); return PARSER_FALSE; }
        irsend->sendSony(HexToLongInt(*args_str[0]), (int) args[1]);
        return PARSER_STRING;
      }      
      else if ( fname == F("recv.setup") && num_args == 1 ) {
        // function ir.recv.init(pin)   define the pin to be used for the irsend
        // set return value
        if (irrecv != NULL)
          delete irrecv;
        irrecv = new IRrecv(args[0]);
        irrecv->enableIRIn(); // Start the receiver
        return PARSER_STRING;
      }
      else if ( fname == F("recv.get") && num_args == 0 ) {
        // function ir.recv.get()   gets the code received
        // set return value
        *value_str = String(IRresults.value, HEX);
        irrecv->resume(); // Receive the next value
        return PARSER_STRING;
      }
      else if ( fname == F("recv.full") && num_args == 0 ) {
        // function ir.recv.type()   gets the type of RC received
        // set return value
        *value_str =   String(IRresults.value, HEX) + ":" + IRtype(IRresults.decode_type) + ":" + String(IRresults.bits);
        irrecv->resume(); // Receive the next value
        return PARSER_STRING;
      }  
      else if ( fname == F("recv.dump") && num_args == 0 ) {
        // function ir.recv.dump()   verbose the code received permitting to identify the remote controller
        // set return value
        IRdump(&IRresults);
        *value_str = String(IRresults.value, HEX);
        irrecv->resume(); // Receive the next value
        return PARSER_STRING;
      }
  }
  else if (fname.startsWith(F("tft.")) )      // block TFT functions; this reduces the number of compares
  {
      fname = fname.substring(4); // skip the term tft.
      if ( fname == F("init") && num_args >= 2 ) {
        // function tft.init(TFT_CS, TFT_DC, Rotation)   init the ILI9341 display; Rotation can be from 0 to 3
        // create the class if not defined
        if (tft == NULL)
           tft = new Adafruit_ILI9341(args[0], args[1]);
        tft->begin();
        if (num_args > 2)
          tft->setRotation(args[2]);
        tft->fillScreen(ILI9341_BLACK);
        return PARSER_TRUE;
      }
      else if ( fname == F("fill") && num_args == 1 ) {
        // function tft.fill(color)   fill the screen with the color (565)
         tft->fillScreen(args[0]);
         return PARSER_TRUE;
      }
      else if ( fname == F("rgb") && num_args == 3 ) {
        // function tft.color(red, green, blue)    returns the color in 565 format from rgb 888
         *value = tft->color565(args[0], args[1], args[2]);
         return PARSER_TRUE;
      }  
      else if ( fname == F("line") && num_args == 5 ) {
        // function tft.line(x1, y1, x2, y2, color) 
         tft->drawLine(args[0], args[1], args[2], args[3], args[4]);
         return PARSER_TRUE;
      }  
      else if ( fname == F("rect") && num_args == 5 ) {
        // function tft.rect(x, y, width, height, color) 
         tft->drawRect(args[0], args[1], args[2], args[3], args[4]);
         return PARSER_TRUE;
      }  
      else if ( fname == F("rect.fill") && num_args == 5 ) {
        // function tft.rect.fill(x, y, width, height, color) 
         tft->fillRect(args[0], args[1], args[2], args[3], args[4]);
         return PARSER_TRUE;
      }  
      else if ( fname == F("rect.round") && num_args == 6 ) {
        // function tft.rect.round(x, y, width, height, radius, color) 
         tft->drawRoundRect(args[0], args[1], args[2], args[3], args[4], args[5]);
         return PARSER_TRUE;
      }  
      else if ( fname == F("rect.round.fill") && num_args == 5 ) {
        // function tft.rect.round.fill(x, y, width, height, radius, color) 
         tft->fillRoundRect(args[0], args[1], args[2], args[3], args[4], args[5]);
         return PARSER_TRUE;
      }
      else if ( fname == F("circle") && num_args == 4 ) {
        // function tft.circle(x, y, radius, color) 
         tft->drawCircle(args[0], args[1], args[2], args[3]);
         return PARSER_TRUE;
      }   
      else if ( fname == F("circle.fill") && num_args == 4 ) {
        // function tft.circle.fill(x, y, radius, color) 
         tft->fillCircle(args[0], args[1], args[2], args[3]);
         return PARSER_TRUE;
      }   
      else if ( fname == F("text.cursor") && num_args == 2 ) {
        // function tft.text.cursor(x, y) 
         tft->setCursor(args[0], args[1]);
         return PARSER_TRUE;
      } 
      else if ( fname == F("text.color") && num_args > 0 ) {
        // function tft.text.color(color, {backcolor})   // the back color is optional
        if (num_args == 1)
          tft->setTextColor(args[0]);
        else
          tft->setTextColor(args[0], args[1]);
        return PARSER_TRUE;
      } 
      else if ( fname == F("text.size") && num_args == 1 ) {
        // function tft.text.size(size) 
         tft->setTextSize(args[0]);
         return PARSER_TRUE;
      } 
      else if ( fname == F("print") && num_args == 1 ) {
        // function tft.text.size(size) 
         if (args_str[0] != NULL)
            tft->print(*args_str[0]);
         return PARSER_STRING;
      } 
      else if ( fname == F("println") && num_args == 1 ) {
        // function tft.text.size(size) 
         if (args_str[0] != NULL)
            tft->println(*args_str[0]);
         return PARSER_STRING;
      }
#ifdef TFT_DEMO
      else if ( fname == F("test") && num_args == 0 ) {
        ILI9341Demo();
        return PARSER_TRUE;
      }
#endif      
  }
  
  else
  if ( (i = Search_Array(fname)) != -1) // check if is an array
  {
    if ( num_args == 0) // the array is just defined as a name without arguments such as a$()
    {
      Serial.println(String(F("Array without argument ")) + fname  + String(i));
      return PARSER_FALSE;
    }
    else if ( num_args == 1)
    {
      if (basic_arrays[i].Format == PARSER_STRING) // string format
      {
        *value_str = basic_arrays[i].getString(args[0]);
        return PARSER_STRING;
      }
      else
      {
        *value = basic_arrays[i].getFloat(args[0]);
        return PARSER_TRUE;
      }
    }
  }

  // failed to evaluate function, return false
  return PARSER_FALSE;
}


String makeMeAString(String StringToReturnFromThisFunction)
{
  return StringToReturnFromThisFunction;
}


unsigned long HexToLongInt(String h)
{
  // this function replace the strtol as this function is not able to handle hex numbers greather than 7fffffff
  // I'll take char by char converting from hex to char then shifting 4 bits at the time
  int i;
  unsigned long tmp = 0;
  unsigned char c;
  int s = 0;
  h.toUpperCase();
  for (i = h.length() -1; i >=0 ; i--)
  {
    // take the char starting from the right
    c = h[i];
    // convert from hex to int
    c = c - '0';
    if (c > 9)
      c = c - 7;
    // add and shift of 4 bits per each char
    tmp += c << s;
    s += 4;
  }
  return tmp;
}



String IRtype(int decode_type)
{
  switch (decode_type)
  {
    case 1:
      return F("NEC");
      break;
    case 2:
      return F("SONY");
      break;
    case 3:
      return F("RC5");
      break;
    case 4:
      return F("RC6");
      break;      
    case 5:
      return F("DISH");
      break;
    case 6:
      return F("SHARP");
      break;
    case 7:
      return F("PANASONIC");
      break;
    case 8:
      return F("JVC");
      break;
    case 9:
      return F("SANYO");
      break;
    case 10:
      return F("MITSUBISHI");
      break;      
    case 11:
      return F("SAMSUNG");
      break;
    case 12:
      return F("LG");
      break;
     case 13:
      return F("WHYNTER");
      break;
    default:
      return F("UNKNOWN");
      break;     
  }
}

void IRdump(void *res) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  decode_results *results = (decode_results *) res;
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print(F("Unknown encoding: "));
  }
  else if (results->decode_type == NEC) {
    Serial.print(F("Decoded NEC: "));

  }
  else if (results->decode_type == SONY) {
    Serial.print(F("Decoded SONY: "));
  }
  else if (results->decode_type == RC5) {
    Serial.print(F("Decoded RC5: "));
  }
  else if (results->decode_type == RC6) {
    Serial.print(F("Decoded RC6: "));
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print(F("Decoded PANASONIC - Address: "));
    Serial.print(results->panasonicAddress, HEX);
    Serial.print(F(" Value: "));
  }
  else if (results->decode_type == LG) {
    Serial.print(F("Decoded LG: "));
  }
  else if (results->decode_type == JVC) {
    Serial.print(F("Decoded JVC: "));
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print(F("Decoded AIWA RC T501: "));
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print(F("Decoded Whynter: "));
  }
  Serial.print(results->value, HEX);
  Serial.print(F(" ("));
  Serial.print(results->bits, DEC);
  Serial.println(F(" bits)"));
  Serial.print(F("Raw ("));
  Serial.print(count, DEC);
  Serial.print(F("): "));

  for (int i = 1; i < count; i++) {
    if (i & 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.write('-');
      Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println();
}


#ifdef TFT_DEMO
void ILI9341Demo()
{
  testFillScreen();
  delay(500);
  testText();
  delay(500);
  testLines(ILI9341_CYAN);
  delay(500);
  testFastLines(ILI9341_RED, ILI9341_BLUE);
  delay(500);
  testRects(ILI9341_GREEN);
  delay(500);
  testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA);
  delay(500);
  testFilledCircles(10, ILI9341_MAGENTA);
  delay(500);
  testCircles(10, ILI9341_WHITE);
  delay(500);
  testTriangles();
  delay(500);
  testFilledTriangles();
  delay(500);
  testRoundRects();
  delay(500);
  testFilledRoundRects();
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft->fillScreen(ILI9341_BLACK);
  tft->fillScreen(ILI9341_RED);
  tft->fillScreen(ILI9341_GREEN);
  tft->fillScreen(ILI9341_BLUE);
  tft->fillScreen(ILI9341_BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft->fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft->setCursor(0, 0);
  tft->setTextColor(ILI9341_WHITE);  tft->setTextSize(1);
  tft->println("Hello World!");
  tft->setTextColor(ILI9341_YELLOW); tft->setTextSize(2);
  tft->println(1234.56);
  tft->setTextColor(ILI9341_RED);    tft->setTextSize(3);
  tft->println(0xDEADBEEF, HEX);
  tft->println();
  tft->setTextColor(ILI9341_GREEN);
  tft->setTextSize(5);
  tft->println("Groop");
  tft->setTextSize(2);
  tft->println("I implore thee,");
  tft->setTextSize(1);
  tft->println("my foonting turlingdromes.");
  tft->println("And hooptiously drangle me");
  tft->println("with crinkly bindlewurdles,");
  tft->println("Or I will rend thee");
  tft->println("in the gobberwarts");
  tft->println("with my blurglecruncheon,");
  tft->println("see if I don't!");
  return micros() - start;
}



unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft->width(),
                h = tft->height();

  tft->fillScreen(ILI9341_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft->drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft->drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft->fillScreen(ILI9341_BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft->drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft->drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft->fillScreen(ILI9341_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft->drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft->drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft->fillScreen(ILI9341_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft->drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft->drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft->width(), h = tft->height();

  tft->fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft->drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft->drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft->width()  / 2,
                cy = tft->height() / 2;

  tft->fillScreen(ILI9341_BLACK);
  n     = min(tft->width(), tft->height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft->drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft->width()  / 2 - 1,
                cy = tft->height() / 2 - 1;

  tft->fillScreen(ILI9341_BLACK);
  n = min(tft->width(), tft->height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft->fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft->drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft->width(), h = tft->height(), r2 = radius * 2;

  tft->fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft->fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft->width()  + radius,
                h = tft->height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft->drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft->width()  / 2 - 1,
                      cy = tft->height() / 2 - 1;

  tft->fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft->drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft->color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft->width()  / 2 - 1,
                   cy = tft->height() / 2 - 1;

  tft->fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft->fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft->color565(0, i, i));
    t += micros() - start;
    tft->drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft->color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft->width()  / 2 - 1,
                cy = tft->height() / 2 - 1;

  tft->fillScreen(ILI9341_BLACK);
  w     = min(tft->width(), tft->height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft->drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft->color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft->width()  / 2 - 1,
                cy = tft->height() / 2 - 1;

  tft->fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft->width(), tft->height()); i>20; i-=6) {
    i2 = i / 2;
    tft->fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft->color565(0, i, 0));
  }

  return micros() - start;
}
#endif
