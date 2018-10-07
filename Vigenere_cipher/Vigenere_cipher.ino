void setup()
{
	Serial.begin(9600);
}

void loop()
{
	Serial.println("Now displaying encrypted message: ");   
      //encrypts message   
      Serial.println(Vigenere_encrypt("Secret Message","PassWord")); 

      Serial.println("Now displaying decrypted message: ");   
      //decrypts message   
      Serial.println(Vigenere_decrypt("~i) sj$hG(^FT`","PassWord")); 
      
      Serial.println("Now displaying generated key from password: ");
      //displays 94 char key, you probably wont need this
      //but I included it anyway
      Serial.println(Vigenere_getKey("PassWord")); 
      
      Serial.println("Now displaying Vigenere table");
      //displays table
      Vigenere_table("PassWord");
	
}

String Vigenere_encrypt(String text, String seed) {
  seed.trim();
  text.trim();

  String key = Vigenere_getKey(seed);

  String enc;
  for(int i = 0; i < text.length(); i++)
  {
    int X = (text[i]-32);
    int K = ((key[i%(key.length())])-32);

    int temp = (X+K)%95;

    enc += (char) (temp + 32);
  }
  return enc;
}

String Vigenere_table(String seed) {
  seed.trim();
  String key = Vigenere_getKey(seed);
  for(int i = 0; i < 95; i++) {
    Serial.print((char) (i+32));
  }

  Serial.print("\n");
  for(int i = 0; i < key.length(); i++) {    
    for(int j = 0; j < 95; j++) {
      int temp = key[i] - 32;
      Serial.print((char)(((temp+j)%95)+32));
    }
    Serial.print("\n");
  }
}

String Vigenere_decrypt(String text, String seed) {
  seed.trim();
  text.trim();

  String key = Vigenere_getKey(seed);
  String dec;
  for(int i = 0; i < text.length(); i++)  {
    int X = (text[i]-32);
    int K = ((key[i%(key.length())])-32);
    int temp = ((X-K)+95)%95;
    dec += (char) (temp + 32);
  }
  return dec;
}

String Vigenere_getKey(String seed) {
  seed.trim();
  int se_ed = 0;
  for(int i = 0; i < seed.length(); i++)  {
    se_ed += (int) (seed[i]);
  }
  randomSeed(se_ed);
  String key;
  for(int i = 0; i < 95; i++) {
    key += (char)(random(32,126));
  }
  return key;
}