
 String x = "hello to my world";
  String y;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.print("x.substring(0,5) ");
Serial.println(x.substring(0,5));

}

void loop() {

 /*
  //cout<< "x = "<< x<< endl;

  //y = x.substr(0, 5); //0 = left
  //cout<< "y = left(x, 5): "<< y<< endl;

  y = x.substr(6, 2);
  cout<< "y = mid(x, 6, 2): "<< y<< endl;

  y = x.substr(9, 2);
  cout<< "y = mid(x, 9, 2): "<< y<< endl;

  y = x.substr(12, 5);
  cout<< "y = mid(x, 12, 5): "<< y<< endl;

  y = x.substr(9, 2);
  cout<< "y = mid(x, 9, 2): "<< y<< endl;

  //get from some position without number of 
  //chars needed
  //gets the string from the pos to end of string
  y = x.substr(9);
  cout<< "y = x from 9 to EOS: "<< y<< endl;

  //get from some position without number of 
  //chars needed
  //also means right, if you calculate it 
  //from the end if string
  // -1 needed because C++ uses 0 based indexes
  y = x.substr(x.length() - 1 - 5);
  cout<< "y = rigth(x, 5): "<< y<< endl;

  //basic string is UNICODE, so use wide string
  //look at w prefix for string and cout
  wstring wx = L"hello world";
  wcout<< wx<< endl;

  //basic string:
  BSTR bstr = SysAllocString(wx.c_str());
  wcout<< bstr<< endl;
  SysFreeString(bstr);
  return 0;
  */
}
