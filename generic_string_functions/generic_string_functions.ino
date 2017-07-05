
String words = "Hello World";

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

Serial.print("WORD: ");
Serial.println(words);
Serial.print("MID: ");
Serial.println(Mid(words,6,11));
Serial.print("vMid: ");
Serial.println(vMid(words,0,5));
Serial.print("LEFT: ");
Serial.println(Left(words,5));
Serial.print("Right: ");
Serial.println(Right(words,5));

}

void loop() {
  // put your main code here, to run repeatedly:

}

String Mid(String& str, int pos1, int pos2)
{
    int i;
    String temp = "";
    for(i = pos1; i < pos2; i++)
    {
        temp += str[i];
    }

    return temp;
}

String vMid(String& str, int pos1, int length)
{
    int i;
    String temp = "";
    for(i = pos1; i < pos1+length; i++)
    {
        temp += str[i];
    }
    return temp;
}

String Left(String& str, int pos)
{
    int i;
    String temp = "";
    for(i = 0; i < pos; i++)
    {
        temp += str[i];
    }

    return temp;
}

String Right(String& str, int pos)
{
    int i;
    String temp = "";
    for( i = str.length()-pos; i < str.length(); i++)
      //i = strlen(str.c_str()); i > pos; i--)
    //pos; i < strlen(str.c_str()); i++)
    {
        temp += str[i];
    }
    return temp;
}


