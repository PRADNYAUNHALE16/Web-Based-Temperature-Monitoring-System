#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266_Simple.h>
#define ESP8266_SSID  "amit"
#define ESP8266_PASS  "amit1234"
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
ESP8266_Simple wifi(2,3);
const int   Relay1 = 13; // choose the pin for the relay
const int  Relay2  = 12; // choose the pin for the relay2
const int led=10;
void setup()
{
  pinMode(Relay1, OUTPUT);  
  pinMode(Relay2, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);  
  digitalWrite(led, LOW);  
  Serial.begin(9600); // Reduce this if your Arduino has trouble            talking so fast
  Serial.println("ESP8266 Demo Server Sketch");
  wifi.begin(115200);
  wifi.setupAsWifiStation(ESP8266_SSID, ESP8266_PASS, &Serial);
  static ESP8266_HttpServerHandler myServerHandlers[] = {
    { PSTR("GET /TEMP1"), httpTEMP1 },
   
    { PSTR("GET /TEMP2"), httpTEMP2 },       
    { PSTR("GET /Hetter"),  httpHetter  },
    { PSTR("GET /fan"),httpFan},
    { PSTR("GET "),      http404} 
  };
   wifi.startHttpServer(80, myServerHandlers, sizeof(myServerHandlers), 250, &Serial);
  Serial.println("( Now you can use your web browser to hit the IP address above. )");
   digitalWrite(led, HIGH);  
  // A blank line just for debug formatting 
  Serial.println();
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Temprature Monitoring ");
  lcd.setCursor(0,1);lcd.print("using IOT   ");delay(1000);
   delay(200);
   }
void loop()
{
  wifi.serveHttpRequest(); 
  return;
}
unsigned long httpTEMP1(char *buffer, int bufferLength)
{
  // empty the buffer
  memset(buffer,0,bufferLength);
  strncpy_P(buffer, PSTR("<h1>The temprature is</h1><p>temprature is: "), bufferLength-strlen(buffer));
  ultoa( temprature1() ,buffer+strlen(buffer),10);
  strncpy_P(buffer+strlen(buffer), PSTR("</p>"), bufferLength-strlen(buffer));        
  return ESP8266_HTML | 200;  
}
unsigned long httpTEMP2(char *buffer, int bufferLength)
{
  // empty the buffer
  memset(buffer,0,bufferLength);
  strncpy_P(buffer, PSTR("<h1>The temprature is</h1><p>temprature is: "), bufferLength-strlen(buffer));
  ultoa( temprature2() ,buffer+strlen(buffer),10);
  strncpy_P(buffer+strlen(buffer), PSTR("</p>"), bufferLength-strlen(buffer));        
  return ESP8266_HTML | 200;  
}
unsigned long temprature1()
{
    int sensorValue1 = analogRead(A0);
    int voltage1 = ((sensorValue1* (5.0 / 1023.0))*100)+2;
    return voltage1;  
}
unsigned long temprature2()
{
    int sensorValue1 = analogRead(A1);
    int voltage1 = ((sensorValue1* (5.0 / 1023.0))*100)+2;
    return voltage1;  
}
unsigned long httpHetter(char *buffer, int bufferLength)
{  
  static byte ledStatus = 0;
    // Let's toggle the usual D13 LED
  pinMode(12, OUTPUT);
  if(ledStatus)
  { 
    digitalWrite(12, LOW);
    ledStatus = 0;
  }
  else
  {
    digitalWrite(12, HIGH);
    ledStatus = 1;
  }    
   // And give a report of it's new status
  memset(buffer,0,bufferLength);
  strncpy_P(buffer, ledStatus ? PSTR("The LOAD1 Status Is: OFF") : PSTR("The LOAD1 Status Is: ON"), bufferLength-strlen(buffer));  
  return ESP8266_TEXT | 200;
}
unsigned long httpFan(char *buffer, int bufferLength)
{  
  static byte ledStatus = 0;
    // Let's toggle the usual D13 LED
  pinMode(13, OUTPUT);
  if(ledStatus)
  { 
    digitalWrite(13, LOW);
    ledStatus = 0;
  }
  else
  {
    digitalWrite(13, HIGH);
   ledStatus = 1;
  }    
    // And give a report of it's new status
  memset(buffer,0,bufferLength);
  strncpy_P(buffer, ledStatus ? PSTR("The LOAD2 Status Is: OFF") : PSTR("The LOAD2 Status Is: ON"), bufferLength-strlen(buffer));
    // And return the type and HTTP response code combined with "|" (bitwise or)
  // Valid  types are: ESP8266_HTML, ESP8266_TEXT, ESP8266_RAW
  // Valid  response codes are: any standard HTTP response code (typically, 200 for OK, 404 for not found, and 500 for error)
 return ESP8266_TEXT | 200;
}    
  // And give a report of it's new status
  memset(buffer,0,bufferLength);
  strncpy_P(buffer, ledStatus ? PSTR("The LIGHT Status Is: ON") : PSTR("The LIGHT Status Is: OFF"), bufferLength-strlen(buffer));  
  // And return the type and HTTP response code combined with "|" (bitwise or)
  // Valid  types are: ESP8266_HTML, ESP8266_TEXT, ESP8266_RAW
  // Valid  response codes are: any standard HTTP response code (typically, 200 for OK, 404 for not found, and 500 for error)
 }
unsigned long http404(char *buffer, int bufferLength)
{  
  memset(buffer, 0, bufferLength);  
  strcpy_P(buffer, PSTR("<h1>Temprature Monitoring using IOT </h1>\r\n<p>Try <a href=\"/TEMP1\">/TEMP1</a>, <a href=\"/Hetter\">/Hetter</a>, <a href=\"/TEMP2\">/TEMP2</a>,<a href=\"/Fan\">/Fan</a></p>"));
  return ESP8266_HTML | 404;
} 
