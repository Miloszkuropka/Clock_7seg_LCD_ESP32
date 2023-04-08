#define SEG_C 14 //poprawny 
#define SEG_E 4 //poprawny
#define SEG_D 12 //poprawny
#define SEG_B 3 //poprawny
#define SEG_G 2 //poprawny 
#define SEG_A 23 //poprawny
#define SEG_F 1 //poprawny 
#define RDP1 13 //poprawny
#define V1 19 
#define V2 5
#define V3 18
#define V4 33
#define BUTTON_PIN 39
#define BUTTON_PIN2 35
#define BUTTON_PIN3 32

#include <WiFi.h>
#include "time.h"
#include <LiquidCrystal_I2C.h>

const char* ssid     = "NETIASPOT-2.4GHz-B21B41";
const char* password = "Z2axEQHVgDHk";

const char* ntpServer = "0.pl.pool.ntp.org";
const long  gmtOffset_sec = 12;
const int   daylightOffset_sec = 3600;

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);


int lastState2 = HIGH;
int lastState = HIGH;
int lastState3 = HIGH;
int counter = 0;
int firstD = 0;
int secondD = 0;
int thirdD = 0;
int fourthD = 0;
int buzzer = 13;

String hour = "0";
String minute = "0";
String month = "0";
String day = "0";
int lightOn = 1;
int lightOn2 = 1;
int currentState;
int currentState2;
int currentState3;
int timeOrDate = 1;
bool backlightChanged = false;
int year = 0;

String text1 = "AAA";
String text2 = "BBB";

void clear(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
  digitalWrite(RDP1, HIGH);
}
void zero(){
  digitalWrite(SEG_G, HIGH);

  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
}
void one(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);

  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
}
void two(){
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);

  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_A, LOW);
}
void three(){
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);

  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_G, LOW);
}
void four(){
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);

  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_B, LOW);
}
void five(){
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_E, HIGH);

  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_A, LOW);
}
void six(){
  digitalWrite(SEG_B, HIGH);

  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_A, LOW);
}
void seven(){
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_G, HIGH);

  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_A, LOW);
}
void eight(){
  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
}
void nine(){
  digitalWrite(SEG_E, HIGH);
  
  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
}
int firstDigit(int n){
  return (n / 1000) % 10;
}
int secondDigit(int n){
    return (n / 100) % 10;
}
int thirdDigit(int n){
    return (n / 10) % 10;
}
int fourthDigit(int n){
    return n % 10;
}
void displayDigit(int n){
  switch (n){
    case 0: zero(); break;
    case 1: one(); break;
    case 2: two(); break;
    case 3: three(); break;
    case 4: four(); break;
    case 5: five(); break;
    case 6: six(); break;
    case 7: seven(); break;
    case 8: eight(); break;
    case 9: nine(); break;
    default: zero();
  }
}

void setup() {
  pinMode(buzzer, OUTPUT);
  digitalWrite (buzzer, HIGH) ;
  
  WiFi.begin(ssid, password);
    delay(1000);
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  delay(500);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }
  setenv("TZ", "GMT-1GMT-2,M3.5.0/2,M10.5.0/3", 1);
  tzset();
  time_t t = mktime(&timeinfo);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  pinMode(V1, OUTPUT);
  pinMode(V2, OUTPUT);
  pinMode(V3, OUTPUT);
  pinMode(V4, OUTPUT);
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(RDP1, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);

  digitalWrite(V1, HIGH);
  digitalWrite(V2, HIGH);
  digitalWrite(V3, HIGH);
  digitalWrite(V4, HIGH);

  delay(200);
  digitalWrite(SEG_A, HIGH);
  delay(200);
  digitalWrite(SEG_B, HIGH);
  delay(200);
  digitalWrite(SEG_C, HIGH);
  delay(200);
  digitalWrite(SEG_D, HIGH);
  delay(200);
  digitalWrite(SEG_E, HIGH);
  delay(200);
  digitalWrite(SEG_F, HIGH);
  delay(200);
  digitalWrite(SEG_G, HIGH);
  delay(200);

  lcd.init();                   
  lcd.backlight();

  lcd.setCursor(0, 0);
  // print message
  lcd.print("Czesc! <3");
  delay(2000);
  setText(timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);
  lcd.setCursor(0, 0);
    // print message
  lcd.print(text1);
  lcd.setCursor(0, 1);
  lcd.print(text2);
}


void loop() {
  Serial.println(timeOrDate);
  if (timeOrDate == 1) printLocalTime();
  else printLocalDate();
}

void printLocalTime(){
  
  Serial.println("TIME");
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  hour = timeinfo.tm_hour;
  int h = hour.toInt();
  minute = timeinfo.tm_min;
  firstD = thirdDigit(h);
  secondD = fourthDigit(h);
  thirdD = thirdDigit(minute.toInt());
  fourthD = fourthDigit(minute.toInt());
  if(timeinfo.tm_hour == 0 && timeinfo.tm_min == 0 && timeinfo.tm_sec < 5){
    setText(timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);
    lcd.setCursor(0, 0);
    // print message
    lcd.print(text1);
    lcd.setCursor(0, 1);
    lcd.print(text2);
  }

  if((lightOn2 == 1 && backlightChanged == false)){
    backlightChanged = true;
    counter = 0;
    lcd.backlight();
  }
  else if (lightOn2==1 && backlightChanged == true && counter < 10){
    counter++;
  }
  else{
    lightOn2 = 0;
    backlightChanged = false;
    lcd.noBacklight();
  }
for (int i = 0; i < 50 ; i++ ){
  if(lightOn==1){
  displayDigit(firstD);
  digitalWrite(V1, HIGH);
  delay(5); 
  digitalWrite(V1, LOW);
  displayDigit(secondD);
  digitalWrite(V2, HIGH);
  delay(5); 
  digitalWrite(V2, LOW);
  displayDigit(thirdD);
  digitalWrite(V3, HIGH);
  delay(5); 
  digitalWrite(V3, LOW);
  displayDigit(fourthD);
  digitalWrite(V4, HIGH);
  delay(5); 
  digitalWrite(V4, LOW);
  }
  currentState = digitalRead(BUTTON_PIN);

  if(lastState == LOW && currentState == HIGH){
    if(lightOn == 1) lightOn = 0;
    else lightOn = 1;
  }
  lastState = currentState;
  currentState2 = digitalRead(BUTTON_PIN2);
  //Serial.println(currentState + " " + currentState2);
  if(lastState2 == LOW && currentState2 == HIGH){
    if (timeOrDate == 0) timeOrDate = 1;
    else timeOrDate = 0;
  }
  lastState2 = currentState2;

  currentState3 = digitalRead(BUTTON_PIN3);
  //Serial.println(currentState + " " + currentState2);
  if(lastState3 == LOW && currentState3 == HIGH){
    if(lightOn2 == 1) lightOn2 = 0;
    else lightOn2 = 1;
  }
  lastState3 = currentState3;
  }
}


void printLocalDate(){
  Serial.println("DATE");
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  month = timeinfo.tm_mon;
  day = timeinfo.tm_mday;
  thirdD = thirdDigit(month.toInt()+1);
  fourthD = fourthDigit(month.toInt()+1);
  firstD = thirdDigit(day.toInt());
  secondD = fourthDigit(day.toInt());
  if(timeinfo.tm_hour == 0 && timeinfo.tm_min == 0 && timeinfo.tm_sec < 5){
    setText(timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);
    lcd.setCursor(0, 0);
    // print message
    lcd.print(text1);
    lcd.setCursor(0, 1);
    lcd.print(text2);
  }

  if((lightOn2 == 1 && backlightChanged == false)){
    backlightChanged = true;
    counter = 0;
    lcd.backlight();
  }
  else if (lightOn2==1 && backlightChanged == true && counter < 10){
    counter++;
  }
  else{
    lightOn2 = 0;
    backlightChanged = false;
    lcd.noBacklight();
  }
for (int i = 0; i < 50 ; i++ ){
  if(lightOn==1){
  displayDigit(firstD);
  digitalWrite(V1, HIGH);
  delay(5); 
  digitalWrite(V1, LOW);
  displayDigit(secondD);
  digitalWrite(V2, HIGH);
  delay(5); 
  digitalWrite(V2, LOW);
  displayDigit(thirdD);
  digitalWrite(V3, HIGH);
  delay(5); 
  digitalWrite(V3, LOW);
  displayDigit(fourthD);
  digitalWrite(V4, HIGH);
  delay(5); 
  digitalWrite(V4, LOW);
  }
  currentState = digitalRead(BUTTON_PIN);

  if(lastState == LOW && currentState == HIGH){
    if(lightOn == 1) lightOn = 0;
    else lightOn = 1;
  }
  lastState = currentState;
  currentState2 = digitalRead(BUTTON_PIN2);
  //Serial.println(currentState + " " + currentState2);
  if(lastState2 == LOW && currentState2 == HIGH){
    if (timeOrDate == 1) timeOrDate = 0;
    else timeOrDate = 1;
  }
  lastState2 = currentState2;
  }
  currentState3 = digitalRead(BUTTON_PIN3);
  //Serial.println(currentState + " " + currentState2);
  if(lastState3 == LOW && currentState3 == HIGH){
    if(lightOn2 == 1) lightOn2 = 0;
    else lightOn2 = 1;
  }
  lastState3 = currentState3;
}

void counterFunc(){
  firstD = firstDigit(counter);
  secondD = secondDigit(counter);
  thirdD = thirdDigit(counter);
  fourthD = fourthDigit(counter);
  for (int i = 0; i < 50 ; i++ ){
  displayDigit(firstD);
  digitalWrite(V1, HIGH);
  delay(5); 
  digitalWrite(V1, LOW);
  displayDigit(secondD);
  digitalWrite(V2, HIGH);
  delay(5); 
  digitalWrite(V2, LOW);
  displayDigit(thirdD);
  digitalWrite(V3, HIGH);
  delay(5); 
  digitalWrite(V3, LOW);
  displayDigit(fourthD);
  digitalWrite(V4, HIGH);
  delay(5); 
  digitalWrite(V4, LOW);
  }
  counter++;
}

void setText(int y, int m, int d){
  lcd.clear();
  switch(m){
    case 0: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 1: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 2: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 3: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Test przed";
                        text2 = "urodzinami ;)";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 4: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";;
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 5 : switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 6: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 7: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 8: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 9: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 10: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    case 11: switch(d){
                case 1: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 2: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 3: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 4: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 5: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 6: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 7: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 8: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 9: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 10: text1 = "Nie";
                         text2 = "Zmienione";
                break;
                case 11: text1 = "Nie";
                          text2 = "Zmienione";
                break;
                case 12: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 13: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 14: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 15: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 16: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 17: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 18: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 19: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 20: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 21: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 22: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 23: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 24: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 25: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 26: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 27: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 28: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 29: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 30: text1 = "Nie";
                        text2 = "Zmienione";
                break;
                case 31: text1 = "Nie";
                        text2 = "Zmienione";
                break;
              }
    break;
    default: text1 = "Dobrego";
      text2 = "dnia";
    break;
  }
}
