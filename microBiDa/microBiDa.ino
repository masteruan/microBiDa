#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <TimeLib.h>
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

String Prices[] = {};

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
  
  for (int i = 0; i < 4; i++){
    JsonObject obj = getData(apiList[i]);
    //String disclaimer = obj[String("disclaimer")];
    //float fprice = obj[String("bpi")][String(tickers[i])][String("rate_float")];
    String price = obj[String("bpi")][String(tickers[i])][String("rate")];
    obj.clear();
    Serial.println(price);
    //Serial.println(fprice);
    
    // Convert float into string
    //char strPrice[16];
    //itoa(price, strPrice, 10);
    //Prices[i] = strPrice;
    Prices[i] = price;
    
    //Serial.println(strPrice);
    //Serial.println(Prices[1]);
    delay(200);
  }

  Serial.print("USD --> ");
  Serial.println(Prices[0]);
  Serial.print("EUR --> ");
  Serial.println(Prices[1]);
  Serial.print("GBP --> ");
  Serial.println(Prices[2]);
  Serial.print("RUB --> ");
  Serial.println(Prices[3]);

  ////////////// display
  while (1){
  for (int i = 0; i < 3; i++){
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_samim_10_t_all);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
    //u8g2.drawStr(3,10,"Power Meter");
    //u8g2.drawStr(1,15,"Inst."); 
    u8g2.drawStr(3,15, Prices[0].c_str());
    
    u8g2.drawStr(89,15,"USD"); 
    u8g2.drawStr(3,32, Prices[1].c_str());
    
    u8g2.drawStr(89,32,"EUR"); 
    u8g2.sendBuffer();         
    delay(5000);
    
    //Second page
    u8g2.clearBuffer();        
    u8g2.setFont(u8g2_font_samim_10_t_all);  // u8g2_font_luBS12_tr choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2.drawStr(3,15, Prices[2].c_str());
    
    u8g2.drawStr(89,15,"GBP"); 
    u8g2.drawStr(3,32, Prices[3].c_str());
    
    u8g2.drawStr(89,32,"RUB");  
    u8g2.sendBuffer();
    delay(5000);
  }
  
  } 
  
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
