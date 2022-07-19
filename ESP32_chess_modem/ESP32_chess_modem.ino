#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <WiFiScan.h>
#include <ETH.h>
#include <WiFiClient.h>
#include <WiFiSTA.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiGeneric.h>
#include <stdint.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <AutoConnect.h>
#include <WebServer.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <ArduinoJson.h>
#include <ValueSetup.h>
#include <LevelSetup.h>
#include <ColorSetup.h>
#include <ESPRandom.h>
#include <Preferences.h>

AutoConnect Portal;
AutoConnectConfig config;
Preferences prefs;

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 102;
const int colorB = 178;

String inputString = "";
HTTPClient http, http2;
boolean wifiConnectionStatus = false;
String boardID;

boolean connectToWIFI(char *ssid, char *password);
String createUserMove(String fromMove, String toMove);
String removeLastChar(String input);
String generateOrReadBoardID();

void setup()
{
  Serial.begin(9600);
  delay(1000);

  config.apid = "RollingPawns";
  config.psk = "rollingpawnadmin";

  Portal.config(config);

  if (Portal.begin())
  {
    Serial.println("HTTP server:" + WiFi.localIP().toString());
    Serial.println("WIFI name:" + WiFi.SSID());
  }
  config.apip = WiFi.localIP();
  Portal.config(config);
  if (Portal.begin())
  {
    Serial.println("Setting current IP");
  }

  boardID = generateOrReadBoardID();

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.print("Let's Play Chess");
  lcd.setCursor(0, 1);
  lcd.print("Set attributes..");
  delay(3000);
}

void loop()
{
  Portal.handleClient();
  if ((WiFi.status() == WL_CONNECTED))
  {

    Serial.println("I got Connected");
    boolean game_over = false;
    String game_id = "";

    LevelSetup levelSetup(0, lcd);
    int level_count = levelSetup.doSetup();

    delay(1000);
    Serial.flush();

    ColorSetup colorSetup("white", lcd);
    String color = colorSetup.doSetup();

    Serial.println("********************************************");
    Serial.println("Starting a Game . .");
    Serial.println("********************************************");
    http.begin("http://rolling-pawn-chess.herokuapp.com/create_game");
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> create_request;

    create_request["board_id"] = boardID;
    create_request["with_engine"] = "true";
    create_request["color"] = color;
    create_request["engine_level"] = String(level_count);

    char json_str[600];
    serializeJsonPretty(create_request, json_str);
    Serial.println(json_str);

    int httpResponseCode = http.POST(json_str);
    Serial.println("HTTP STATUS:" + String(httpResponseCode));

    if (httpResponseCode > 0)
    {
      Serial.println("*******************   Game Started    **********************");
      String response_ui = http.getString();

      DynamicJsonDocument create_response(2048);
      deserializeJson(create_response, response_ui);

      game_id = create_response["game_id"].as<String>();

      if (color == "white")
      {
        lcd.clear();
        lcd.print("Game Started");
        lcd.setCursor(0, 1);
        lcd.print("Waiting for your move");
      }
      else
      {
        lcd.clear();
        lcd.print("Game Started:");
        lcd.setCursor(0, 1);
        lcd.print("Engine Move:" + create_response["initial_move"]["engine_move"]["from"].as<String>() + create_response["initial_move"]["engine_move"]["to"].as<String>());

        Serial.println("********************************************");
        Serial.println("Sending move to UI");
        Serial.println("********************************************");

        http.begin("https://rolling-pawn.herokuapp.com/move"); // This was JD's local machine. We need to host it on heroku
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String payload_ui = "from=" + create_response["initial_move"]["engine_move"]["from"].as<String>() + "&to=" + create_response["initial_move"]["engine_move"]["to"].as<String>();
        int httpResponseCode_ui = http.POST(payload_ui);
        String response_ui = http.getString();
        if (httpResponseCode_ui > 0)
        {

          Serial.println("*******************   SENT MOVE TO UI     **********************");
          Serial.println(response_ui);
          Serial.println("******************************************************************");
        }
        else
        {

          Serial.println("ERROR SENDING REQUEST TO UI");
          Serial.println(response_ui);
        }

        http.end();
      }

      Serial.println("******************************************************************");
    }
    else
    {
      Serial.println("ERROR SENDING REQUEST");
    }

    Serial.flush();
    //Gameplay

    do
    {
      String moveCombination[2];
      String userMove = "";
      int count = 0;
      String chessEngineMoves = "";
      String user_from_move = "";
      String user_to_move = "";

      do
      {
        if (Serial.available() > 0)
        {
          inputString = removeLastChar(Serial.readStringUntil('\n'));

          if (moveCombination[count - 1] != inputString)
          {
            moveCombination[count] = inputString;
            count++;
          }
        }

      } while (count < 2);

      user_from_move = moveCombination[0];
      user_to_move = moveCombination[1];
      userMove = createUserMove(user_from_move, user_to_move);

      Serial.println("Your move is:" + userMove);

      Serial.println("********************************************");
      Serial.println("Sending move to UI");
      Serial.println("********************************************");

      http.begin("https://rolling-pawn.herokuapp.com/move"); // This was JD's local machine. We need to host it on heroku
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String payload_ui = "from=" + user_from_move + "&to=" + user_to_move;
      int httpResponseCode_ui = http.POST(payload_ui);
      String response_ui = http.getString();
      if (httpResponseCode_ui > 0)
      {

        Serial.println("*******************   SENT MOVE TO UI     **********************");
        Serial.println(response_ui);
        Serial.println("******************************************************************");
      }
      else
      {

        Serial.println("ERROR SENDING REQUEST TO UI");
        Serial.println(response_ui);
      }

      http.end();

      Serial.println("********************************************");
      Serial.println("Asking chess engine for it's move . . . .");
      Serial.println("********************************************");

      http.begin("https://rolling-pawn-chess.herokuapp.com/play");
      http.addHeader("Content-Type", "application/json");

      StaticJsonDocument<200> gameplay_request;

      gameplay_request["game_id"] = game_id;
      gameplay_request["from"] = user_from_move;
      gameplay_request["to"] = user_to_move;

      char json_str_gameplay_req[600];
      serializeJsonPretty(gameplay_request, json_str_gameplay_req);

      Serial.println(json_str_gameplay_req);

      int httpResponseCode_gameplay = http.POST(json_str_gameplay_req);
      Serial.println("HTTP STATUS:" + String(httpResponseCode_gameplay));

      if (httpResponseCode_gameplay > 0)
      {

        Serial.println("*******************   CHESS ENGINE MOVE     **********************");
        String gameplay_response = http.getString();
        Serial.println(gameplay_response);

        DynamicJsonDocument gameplay_response_doc(2048);
        deserializeJson(gameplay_response_doc, gameplay_response);

        game_over = gameplay_response_doc["game_over"].as<boolean>();

        if (game_over)
        {
          lcd.clear();
          lcd.print("Game Ended");
          lcd.setCursor(0, 1);
          lcd.print("Start a new game");
        }
        else
        {
          lcd.clear();
          lcd.print("Engine Move:");
          lcd.setCursor(0, 1);
          lcd.print("FROM:" + gameplay_response_doc["engine_move"]["from"].as<String>() + "TO:" + gameplay_response_doc["engine_move"]["to"].as<String>());

          Serial.println("********************************************");
          Serial.println("Sending move to UI");
          Serial.println("********************************************");

          http.begin("https://rolling-pawn.herokuapp.com/move");
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");

          String payload_ui = "from=" + gameplay_response_doc["engine_move"]["from"].as<String>() + "&to=" + gameplay_response_doc["engine_move"]["to"].as<String>();
          int httpResponseCode_ui = http.POST(payload_ui);
          String response_ui = http.getString();
          if (httpResponseCode_ui > 0)
          {

            Serial.println("*******************   SENT MOVE TO UI     **********************");
            Serial.println(response_ui);
            Serial.println("******************************************************************");
          }
          else
          {

            Serial.println("ERROR SENDING REQUEST TO UI");
            Serial.println(response_ui);
          }

          http.end();
        }

        delay(2000);

        Serial.println("******************************************************************");
      }
      else
      {
        Serial.println("ERROR SENDING REQUEST");
      }

      http.end();
      Serial.flush();
      Serial.println("READY FOR YOUR NEXT MOVE . . .");

    } while (game_over == false);

    Serial.println("Game Ended!");
  }
}

//Create user move
String createUserMove(String fromMove, String toMove)
{
  String userMove = "";
  userMove = fromMove + toMove;
  return userMove;
}

//Remove new line from a String
String removeLastChar(String input)
{
  unsigned int stringLength = input.length();
  input.remove(stringLength - 1);
  return input;
}

//Generates a Board ID on first start and returns it
String generateOrReadBoardID()
{
  prefs.begin("rolling-pawns"); //Namespace is limited to 15 chars
  const char key[] = {'b', 'o', 'a', 'r', 'd', '-', 'i', 'd'};
  String boardID = prefs.getString(key, "NOT_FOUND"); //Returns NOT_FOUND if board id is not already stored
  const bool isBoardIDStored = !(boardID == "NOT_FOUND");
  if (isBoardIDStored)
  {
    return boardID;
  }
  uint8_t buffer[16];
  ESPRandom::uuid(buffer);
  String newBoardID = ESPRandom::uuidToString(buffer);
  prefs.putString(key, newBoardID);
  return newBoardID;
}