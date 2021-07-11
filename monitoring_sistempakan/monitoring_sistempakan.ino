//#include <ESP8266HTTPClient.h>

// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <DHT.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>

WiFiClient wifiClient;
  myservo.write(0);  myservo.write(0);
          delay(1000);
          myservo.write(60);
          delay(1000);
          delay(1000);
          myservo.write(60);
          delay(1000);
ThreeWire myWire(4,5,0); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
Servo myservo;
SoftwareSerial s(D7,D6);

DHT dht(D5, DHT11);
float humidityData;
float temperatureData;
String phValue1;

const char* ssid     = "realme7";
const char* password = "12345678";
const char* serverName = "http://iotlobster.masuk.web.id/api/sensor"; 

void setup () 
{
//    myservo.write(75);
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    
    s.begin(9600);
    dht.begin();
    Serial.begin(9600);
    myservo.attach(2);
     myservo.write(0);
     delay(1000);
//    myservo.write(75);
//    delay(1000);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
     
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
}

void loop () 
{
    if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifiClient, serverName);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");


    // Prepare your HTTP POST request data
    String httpRequestData = "&temp=" + String(dht.readTemperature()) + "&hum=" + String(dht.readHumidity()) +  "&ph=" + String(phValue1[0]);
    //String httpRequestData = "api_key=theiotprojects&sensor=HC-SR04&location=Home&distance=24.75";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();

    if (!now.IsValid())
    {
        Serial.println("RTC lost confidence in the DateTime!");
    }

    delay(1000); // ten seconds
    
    Serial.println(dht.readTemperature());
    Serial.println(dht.readHumidity());
    
    while(s.available()>0)
      {
        phValue1 += char(s.read());
      }

        Serial.println(phValue1[0]);
    
    
      if(now.Hour() == 11 & now.Minute() == 10 & now.Second() <= 5)
      {  
        Serial.print("Makan Pagi");
        
          myservo.write(60);
          delay(1000);
          myservo.write(0);
          delay(1000);
      }
      else if (now.Hour() == 11 & now.Minute() == 9 & now.Second() <= 5 )
      {
        Serial.print("Makan Sore");
        myservo.write(60);
          delay(1000);
          myservo.write(0);
          delay(1000);
  //      
      }
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}
