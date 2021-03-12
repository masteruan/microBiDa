#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

const char* ssid = "YOUR-SSID";
const char* password = "YOUR-PWD";

String btc = "https://api.coindesk.com/v1/bpi/currentprice/BTC.json";
String eur = "https://api.coindesk.com/v1/bpi/currentprice/EUR.json";
String gbp = "https://api.coindesk.com/v1/bpi/currentprice/GBP.json";
String rub = "https://api.coindesk.com/v1/bpi/currentprice/RUB.json";
String tickers[] = {"USD", "EUR", "GBP", "RUB"};
String apiList[] = {btc, eur, gbp, rub};
char *Prices[]={};
String strPrice;

DynamicJsonDocument doc(1024);
HTTPClient https;

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\n");
  
  u8g2.begin();
  u8g2.enableUTF8Print();
  
}

void loop(void) {
  JsonObject obj = getData(apiList[0]);
  
  float price = obj[String("bpi")][String(tickers[0])][String("rate_float")];
  
  char strPrice[16];
  itoa(price, strPrice, 10);
    
  Prices[0] = strPrice;
  
  delay(30);
  
  // USD display
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_luBS12_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  //u8g2.drawStr(3,10,"Power Meter");
  //u8g2.drawStr(1,15,"Inst."); 
  u8g2.drawStr(1,15, "BTC price USD");
  //u8g2.drawStr(100,15,Prices[0]); 
  u8g2.drawStr(2,32, Prices[0]);
  //u8g2.drawStr(90,32,"USD"); 
  u8g2.sendBuffer();         
  delay(5000);
  
  obj.clear();

  // Display EUR
  obj = getData(apiList[1]);
  price = obj[String("bpi")][String(tickers[1])][String("rate_float")];
  String price_txt = obj[String("bpi")][String(tickers[1])][String("rate")];
  
  itoa(price, strPrice, 10);
  Prices[0] = strPrice;
  delay(30);
  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_luBS12_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(1,15, "BTC price EUR");
  u8g2.drawStr(3,32, Prices[0]);
  //u8g2.drawStr(90,32,"EUR"); 
  u8g2.sendBuffer();         
  delay(5000);

  // display GBP
  obj = getData(apiList[2]);
  price = obj[String("bpi")][String(tickers[2])][String("rate_float")];
  itoa(price, strPrice, 10);
  Prices[0] = strPrice;
  delay(30);
  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_luBS12_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(1,15, "BTC price GBP");
  u8g2.drawStr(3,32, Prices[0]);
  //u8g2.drawStr(90,32,"GBP"); 
  u8g2.sendBuffer();         
  delay(5000);

  // display RUB
  obj = getData(apiList[3]);
  price = obj[String("bpi")][String(tickers[3])][String("rate_float")];
  itoa(price, strPrice, 10);
  Prices[0] = strPrice;
  delay(30);
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_luBS12_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(1,15, "BTC price RUB");
  u8g2.drawStr(3,32, Prices[0]);
  //u8g2.drawStr(90,32,"RUB"); 
  u8g2.sendBuffer();         
  delay(5000);
}

JsonObject getData(String dataUlr) {
  doc.clear();

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  https.begin(*client, dataUlr);
  int httpCode = https.GET();
  String payloadString = https.getString();
  https.end();

  deserializeJson(doc, payloadString);
  return doc.as<JsonObject>();
}
