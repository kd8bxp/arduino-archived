
//This function is used to get JSON strings from various websites
//just pass the URL of the website you want to use to the function.

void getJson(String url) {
  
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     HTTPClient http;  //Declare an object of class HTTPClient
     http.begin(url);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
     if (httpCode > 0) { //Check the returning code
       payload = http.getString();   //Get the request response payload
     
    }
 
    http.end();   //Close connection
 
  }
}
