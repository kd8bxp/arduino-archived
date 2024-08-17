#include <WiFi.h>
#include "src/rssClass.h"

const char *ssid = "MySpectrumWiFi68-2G";
const char *password = "loudowl369";

const char *url = "http://www.hamqsl.com/solarxml.php";
const char *tag = "solardata";

rssClass reader;
const int rssArticleSize = 128;
const int rssNumArticle = 20;
const int rssGetInterval = 1800;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.printf("Connecting to WiFi ... ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.printf("done.\n");

  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
}

void loop() {
  struct tm timeInfo;
  char header[32];

  getLocalTime(&timeInfo);
  sprintf(header, "News at %04d/%02d/%02d %02d:%02d",
          timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
          timeInfo.tm_hour, timeInfo.tm_min);
  Serial.printf("\n%s\n", header);

  int n = reader.getArticles(url, tag, rssArticleSize, rssNumArticle);
  for (int i = 0; i < n; i++) {
    Serial.printf("%2d: %s\n", i, reader.itemData[i]);
  }
  delay(rssGetInterval*1000);
}
