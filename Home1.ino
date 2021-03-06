// Line config
//6HxHdNubzHq9YhsmJEZsEVncoutgedaF20WcOCmbjqP บูม home 1
//kUkXE3ET5dBiaWeBinRvda7ZwJPswb6AF9GRL2l3l63  บาส กลุ่ม (นิชดาธานี)เจ้าของบ้าน Home 2
//odaUjYVOnykrCMKIxi34pPXhyIWxgN4jAXsdUhV5WrX  ธาม (หมู่บ้านอินโดจีน)
String LINE_TOKEN;
int Line_stage = 0;
#include <TridentTD_LineNotify.h>
#include "types.h"
#include "HardwareSerial.h";
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
String message;
String la;
String lon;
#include <FirebaseESP32.h>
#define FIREBASE_HOST "esp32-e7cd8-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "ZEg2cykyjznycfFZc74tIYbJg4Rm4H2QqjjYO2Si"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>
const char* ssid = "thananat   2.4G";
const char* password = "0891550015";
String chatId = "-632095692";
//Group chat id = -712734165
//Home 1          -632095692
//ChatID เก่า = 1945500587
String BOTtoken = "2047760599:AAEP9Le8LpUONj2KhMVLUCgKgFtf8AR0wZQ";
//5102185862:AAGqigXLwWyPhEGPFm_NmPljifXjZu-cW7k ล่าสุด
//2047760599:AAEP9Le8LpUONj2KhMVLUCgKgFtf8AR0wZQ Tele Line
//1947299280:AAG1OqCv7VgJoYzNO3acoJcL87FHiefW6f8 น้ำชา
WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);
FirebaseData firebaseData;
#include <NTPClient.h>
#include <WiFiUdp.h>
int currentHour;
int currentMinute;
int currentSecond;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
int Random = 0;
int Fansmoke = 0; //พัดลมดูดอากาศ
int flamewater = 0; //สปริงเกอร์
int LampCamera = 0; //ไฟกล้อง
int PIR_firebase = 0;
bool FullAlarm = false;
bool DayMode = false;
int Counter = 0;
int sendPhoto = 0;
bool Doorstat = HIGH;
bool Fanstat = HIGH;
int botRequestDelay = 1000;   // mean time between scan messages
long lastTimeBotRan;     // last time messages' scan has been done
bool door1stat = true;
int doortelegram = 0;
bool doorLockMonitor = false;
bool NightMode = false;
int home1 = 0;
int home2 = 0;
int home3 = 0;
int home4 = 0;
int home5 = 0;
int firewall = 0;
int flag = 1;
int PIRStat = 0;
bool sound = HIGH;
String PIR_Telegram;
String DOOR_Telegram;
String SMOKE_Telegram;
String Fire_Telegram;
String Siren_Telegram;
String Online_Telegram;
//*************************************************************เปลี่ยนขา***********************************************************************


int smokeA0 = 34;//อนาล็อคgas ต่อเข้าด้วยA0
int FlameA3 = 15;// วัดเปลวไฟ
int Lamp = 22; // ช่อง 1
int Door = 21; // ช่อง 2
int Fan = 19; // ช่อง 3
int water = 4; // ช่อง 4
const byte door1 = 23;
int buzzer = 18;

void handleNewMessages(int numNewMessages);
void setup(){
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  pinMode(Lamp, OUTPUT);
  pinMode(Door, OUTPUT);
  pinMode(Fan, OUTPUT);
  pinMode(water, OUTPUT);
  pinMode(smokeA0, INPUT);
  pinMode(FlameA3, INPUT);
  pinMode(door1, INPUT);
  pinMode(buzzer, OUTPUT);
  
  digitalWrite(Lamp, HIGH);
  digitalWrite(Door, HIGH);
  digitalWrite(Fan, HIGH); 
  digitalWrite(water, HIGH); 
  digitalWrite(buzzer,HIGH);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  bot.sendMessage(chatId, "กด /start เพื่อเริ่มต้นใช้งาน", "");
  timeClient.begin();
  timeClient.setTimeOffset(25200);
  currentHour = 0;
  currentMinute =0; 
  currentSecond = 0;
    if (Firebase.getString(firebaseData, "/Village/LineGroup")) { //กลุ่มหมู่บ้าน
      if (firebaseData.dataType() == "string") {
        LINE_TOKEN = firebaseData.stringData();    
      }
    }   
  LINE.setToken(LINE_TOKEN); 
    if (Firebase.getString(firebaseData, "/home1/Location/Latitude")) {
      if (firebaseData.dataType() == "string") {
       la = firebaseData.stringData();
      }
    }
    if (Firebase.getString(firebaseData, "/home1/Location/Longitude")) {
      if (firebaseData.dataType() == "string") {
       lon = firebaseData.stringData();
      }
    }
    
  if (Firebase.getInt(firebaseData, "/home1/Token/Status/Door")) {
    if (firebaseData.dataType() == "int") {
      doortelegram = firebaseData.intData();
     }
  }
  if (Firebase.getInt(firebaseData, "/home1/Token/Status/flag")) {
    if (firebaseData.dataType() == "int") {
      flag = firebaseData.intData();
     }
  }
  if (Firebase.getInt(firebaseData, "/home1/Token/Status/PIRStat")) {
    if (firebaseData.dataType() == "int") {
      PIRStat = firebaseData.intData();
     }
  }
  if (Firebase.getInt(firebaseData, "/home1/Token/Status/Line")) {
    if (firebaseData.dataType() == "int") {
      Line_stage = firebaseData.intData();
     }
  }  
  if (Firebase.getInt(firebaseData, "/home1/Token/Status/fire")) {
    if (firebaseData.dataType() == "int") {
      firewall = firebaseData.intData();
     }
  }
    if (Firebase.getString(firebaseData, "/home1/Token/Status/Door1")) {
      if (firebaseData.dataType() == "string") {
       DOOR_Telegram = firebaseData.stringData();
      }
    }
    if (Firebase.getString(firebaseData, "/home1/Token/Status/smoke")) {
      if (firebaseData.dataType() == "string") {
       SMOKE_Telegram = firebaseData.stringData();
      }
    }  
    if (Firebase.getString(firebaseData, "/home1/Token/Status/fire1")) {
      if (firebaseData.dataType() == "string") {
       Fire_Telegram = firebaseData.stringData();
      }
    }
    if (Firebase.getString(firebaseData, "/home1/Token/Status/Line1")) {
      if (firebaseData.dataType() == "string") {
       Online_Telegram = firebaseData.stringData();
      }
    }      
    if (Firebase.getString(firebaseData, "/home1/Token/Status/PIRTelegram")) {
      if (firebaseData.dataType() == "string") {
       PIR_Telegram = firebaseData.stringData();
      }
    }       
}

void GetLine() { // Line กลุ่ม
  LINE.notify(message); //ส่งไลน์หมู่บ้าน
  if (home1 == 1){ //บูม Home1
      if (Firebase.getString(firebaseData, "/home1/Token/Line")) { 
      if (firebaseData.dataType() == "string") {
        LINE_TOKEN = firebaseData.stringData();    
      }
    }  
    LINE.setToken(LINE_TOKEN);
    LINE.notify(message);
    }
  if (home2 == 1){ //บาส Home2
      if (Firebase.getString(firebaseData, "/home2/Token/Line")) { 
      if (firebaseData.dataType() == "string") {
        LINE_TOKEN = firebaseData.stringData();    
      }
    }  
    LINE.setToken(LINE_TOKEN);
    LINE.notify(message);
    }    
  if (home3 == 1){ //Home3
      if (Firebase.getString(firebaseData, "/home3/Token/Line")) { 
      if (firebaseData.dataType() == "string") {
        LINE_TOKEN = firebaseData.stringData();    
      }
    }  
    LINE.setToken(LINE_TOKEN);
    LINE.notify(message);
    }
  if (home4 == 1){ //Home4
      if (Firebase.getString(firebaseData, "/home4/Token/Line")) { 
      if (firebaseData.dataType() == "string") {
        LINE_TOKEN = firebaseData.stringData();    
      }
    }  
    LINE.setToken(LINE_TOKEN);
    LINE.notify(message);
    }
  if (home5 == 1){ //Home5
      if (Firebase.getString(firebaseData, "/home5/Token/Line")) { 
      if (firebaseData.dataType() == "string") {
        LINE_TOKEN = firebaseData.stringData();    
      }
    }  
    LINE.setToken(LINE_TOKEN);
    LINE.notify(message);
    }      
    if (Firebase.getString(firebaseData, "/Village/LineGroup")) { //หมู่บ้าน
      if (firebaseData.dataType() == "string") {
        LINE_TOKEN = firebaseData.stringData();    
      }
    }
  LINE.setToken(LINE_TOKEN);    
}

void GetTime() {
  timeClient.update();  
  currentHour = timeClient.getHours();
  currentMinute = timeClient.getMinutes(); 
  currentSecond = timeClient.getSeconds(); 
  Serial.print("Seconds: ");
  Serial.println(currentSecond);
}
void loop(){
  if(FullAlarm){
    GetTime();
    if(currentSecond == 32 ){
      Random = random(2);
      Serial.println(Random);      
      digitalWrite(Lamp, Random);                       
     }     
   } 
  if(PIRStat == 1){
    if (Firebase.getInt(firebaseData, "/home1/Camera/PIRStat")) {
      if (firebaseData.dataType() == "int") {
        PIR_firebase = firebaseData.intData();
      }
    }
    if (PIR_firebase == 1 ) {
      if (NightMode) {  //โหมดกลางคืน
        digitalWrite(Lamp, LOW);
        LampCamera = 1;
        }      
        bot.sendMessage(chatId, "ตรวจพบการเคลื่อนไหวในบริเวณบ้าน !!?", "");
      if (Line_stage == 1){
        message = "บ้านเลขที่ 1 ตรวจพบการเคลื่อนไหว ในขณะที่เจ้าของบ้านไม่อยู่ www.google.com/maps/place/" + la + "," + lon ;
        GetLine();
        }               
    digitalWrite(buzzer,sound);
    if (sound == LOW){
    delay(500);
    }
    digitalWrite(buzzer,HIGH);    
    }
    else {
      if (NightMode) {
        if (LampCamera == 1){
      digitalWrite(Lamp, HIGH);
      LampCamera = 0;
      }
      }
      }     
  }
  if(doortelegram == 1)   { 
    door1stat = digitalRead(door1); // เซนเซอร์ แม่เหล็ก 0 1   ประตูเปิดเป็น 0 
    if(door1stat == 1 && doorLockMonitor == 1) {                              //door1stat = 1   และ doorLockMonitor = 1
      bot.sendMessage(chatId, "ประตูปิดแล้ว", "");     
      Firebase.setInt(firebaseData, "/home1/DOORSTAT",door1stat);
      if (NightMode) {  //โหมดกลางคืน
      digitalWrite(Lamp, HIGH);
        }      
      doorLockMonitor =  false;                                         //door1stat = 1   และ doorLockMonitor = 0
     }
    if(door1stat == 0 && doorLockMonitor == 0) {                              //door1stat = 0   และ doorLockMonitor = 0
      bot.sendMessage(chatId, "ตรวจพบการเปิดประตู !!", "");
      Firebase.setInt(firebaseData, "/home1/DOORSTAT",door1stat);
      digitalWrite(buzzer,sound);
      //Serial.println(sound);
      if (sound == LOW){
      delay(500);
      }
      digitalWrite(buzzer,HIGH);
      if (NightMode) {  //โหมดกลางคืน
      digitalWrite(Lamp, LOW);
        }
      if (Line_stage == 1){
        message = "บ้านเลขที่ 1 ตรวจพบการงัดแงะประตู หน้าต่างที่ www.google.com/maps/place/" + la + "," + lon ;
        GetLine();
        }        
      doorLockMonitor = true;                                         //door1stat = 0   และ doorLockMonitor = 1
        }
  }
  if(firewall == 1){  
  int smoke = analogRead(smokeA0);
  smoke = smoke/3;
  Firebase.setInt(firebaseData, "/home1/SMOKE",smoke);
  delay(100);
  int flame = digitalRead(FlameA3);
  Firebase.setInt(firebaseData, "/home1/FLAME",flame);
  delay(100);

    if (smoke > 749 || flame == 0 ) { 
    Serial.println("GAS OR FLAME DECTION!");
    bot.sendMessage(chatId, "ตรวจพบแก๊ส และเปลวไฟภายในบริเวณบ้าน", "");    
    digitalWrite(buzzer,sound);
    if (sound == LOW){
       delay(500);                      }
    digitalWrite(buzzer,HIGH);
    if (flame == 0 && flamewater == 0) {
      digitalWrite(water, LOW);  
      flamewater = 1;   
      }
    if (smoke > 1600 && Fansmoke == 0 ) {
      digitalWrite(Fan, LOW);
      Fansmoke = 1; 
          
      }      
      if (Line_stage == 1){
        message = "บ้านเลขที่ 1 ตรวจพบแก๊ส และเปลวไฟภายในบริเวณ www.google.com/maps/place/" + la + "," + lon ;
        GetLine();
        }        
    delay(2000);
    }
    if (Fansmoke == 1 && smoke < 1600){ //สั่งปิดพัดลม
    digitalWrite(Fan, HIGH);
    Fansmoke = 0;
      }
    if (flamewater == 1 && flame == 1) { //สั่งปิดน้ำ
      digitalWrite(water, HIGH);  
      flamewater = 0;   
      }        
  }
     
  if (millis() > lastTimeBotRan + botRequestDelay){
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages){
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
 
}

void handleNewMessages(int numNewMessages){
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);
  for (int i = 0; i < numNewMessages; i++){
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != chatId){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    String fromName = bot.messages[i].from_name;
    
    if (text == "/HELP") { 
        message = "บ้านเลขที่ 1 พบเหตุผิดปกติ ขอความช่วยเหลือด่วน www.google.com/maps/place/" + la + "," + lon ;
        bot.sendMessage(chatId, "ทำการส่งข้อความขอความช่วยเหลือแล้วค่ะ", "");      
        GetLine();
    }        
    if (text == "/Home") {
      String welcome = "เลือกบ้านที่ต้องการแจ้งเตือน\n";      
      welcome += "/Home1 /Home1Off: home 1\n";
      welcome += "/Home2 /Home2Off: home 2\n";
      welcome += "/Home3 /Home3Off: home 3\n";
      welcome += "/Home4 /Home4Off: home 4\n";
      welcome += "/Home5 /Home5Off: home 5\n";
      welcome += "/All : เปิดการแจ้งเตือนบ้านทุกหลัง\n";
      welcome += "/Mute : ปิดการแจ้งเตือน\n"; 
      bot.sendMessage(chatId, welcome, "Markdown");
    }
    if (text == "/All") {
      home1 = 1;
      home2 = 1;
      home3 = 1;
      home4 = 1;
      home5 = 1;
      Firebase.setInt(firebaseData, "home1/Social/home1",home1);
      Firebase.setInt(firebaseData, "home1/Social/home2",home2);
      Firebase.setInt(firebaseData, "home1/Social/home3",home3);
      Firebase.setInt(firebaseData, "home1/Social/home4",home4);
      Firebase.setInt(firebaseData, "home1/Social/home5",home5);    
      bot.sendMessage(chat_id, "เปิดการแจ้งเตือนบ้านทุกหลัง", "");
    }
    if (text == "/Mute") {
      home1 = 0;
      home2 = 0;
      home3 = 0;
      home4 = 0;
      home5 = 0;
      Firebase.setInt(firebaseData, "home1/Social/home1",home1);
      Firebase.setInt(firebaseData, "home1/Social/home2",home2);
      Firebase.setInt(firebaseData, "home1/Social/home3",home3);
      Firebase.setInt(firebaseData, "home1/Social/home4",home4);
      Firebase.setInt(firebaseData, "home1/Social/home5",home5);    
      bot.sendMessage(chat_id, "ปิดการแจ้งเตือนบ้านทุกหลัง", "");
    }    
    if (text == "/Home1") {
      home1 = 1;
      Firebase.setInt(firebaseData, "home1/Social/home1",home1);
      bot.sendMessage(chat_id, "เปิดการแจ้งเตือน Home 1", "");
    }
    if (text == "/Home2") {
      home2 = 1;
      Firebase.setInt(firebaseData, "home1/Social/home2",home2);
      bot.sendMessage(chat_id, "เปิดการแจ้งเตือน Home 2", "");
    }
    if (text == "/Home3") {
      home3 = 1;
      Firebase.setInt(firebaseData, "home1/Social/home3",home3);
      bot.sendMessage(chat_id, "เปิดการแจ้งเตือน Home 3", "");
    }
    if (text == "/Home4") {
      home4 = 1;
      Firebase.setInt(firebaseData, "home1/Social/home4",home4);
      bot.sendMessage(chat_id, "เปิดการแจ้งเตือน Home 4", "");
    }        
    if (text == "/Home5") {
      home5 = 1;
      Firebase.setInt(firebaseData, "home1/Social/home5",home5);
      bot.sendMessage(chat_id, "เปิดการแจ้งเตือน Home 5", "");
    }     
    if (text == "/Home1Off") {
      home1 = 0;
      Firebase.setInt(firebaseData, "home1/Social/home1",home1);
      bot.sendMessage(chat_id, "ปิดการแจ้งเตือน Home 1", "");
    }
    if (text == "/Home2Off") {
      home2 = 0;
      Firebase.setInt(firebaseData, "home1/Social/home2",home2);
      bot.sendMessage(chat_id, "ปิดการแจ้งเตือน Home 2", "");
    }
    if (text == "/Home3Off") {
      home3 = 0;
      Firebase.setInt(firebaseData, "home1/Social/home3",home3);
      bot.sendMessage(chat_id, "ปิดการแจ้งเตือน Home 3", "");
    }
    if (text == "/Home4Off") {
      home4 = 0;
      Firebase.setInt(firebaseData, "home1/Social/home4",home4);
      bot.sendMessage(chat_id, "ปิดการแจ้งเตือน Home 4", "");
    }
    if (text == "/Home5Off") {
      home5 = 0;
      Firebase.setInt(firebaseData, "home1/Social/home5",home5);
      bot.sendMessage(chat_id, "ปิดการแจ้งเตือน Home 5", "");
    }    
    if (text == "/OFFLine") {
      bot.sendMessage(chatId, "ทำการปิดการแจ้งเตือน", "");
      Line_stage = 0;
      Online_Telegram = "OFF";
      Firebase.setInt(firebaseData, "/home1/Token/Status/Line",Line_stage);
      Firebase.setString(firebaseData, "/home1/Token/Status/Line1",Online_Telegram);      
    }      
    if (text == "/ONLine") {
      String welcome = "เปิดการแจ้งเตือน\n";      
      welcome += "/Home เลือกบ้านที่ต้องการส่งการแจ้งเตือน\n"; 
      bot.sendMessage(chatId, welcome, "Markdown");
      Line_stage = 1;
      Online_Telegram = "ON";
      Firebase.setInt(firebaseData, "/home1/Token/Status/Line",Line_stage);
      Firebase.setString(firebaseData, "/home1/Token/Status/Line1",Online_Telegram);
      
    }         
    if (text == "/TurnON") {
      bot.sendMessage(chatId, "เปิดไฟเรียบร้อย", "");
      digitalWrite(Lamp, LOW);
    }
    if (text == "/TurnOFF") {
      bot.sendMessage(chatId, "ปิดไฟเรียบร้อย", "");
      digitalWrite(Lamp, HIGH);
    }
    if (text == "/DoorAlarm") {
       doortelegram = 1;
       DOOR_Telegram = "ON";      
       bot.sendMessage(chat_id, "เปิดระบบตรวจจับประตูเรียบร้อยค่ะ", "");
       Firebase.setInt(firebaseData, "/home1/Token/Status/Door",doortelegram);
       Firebase.setString(firebaseData, "/home1/Token/Status/Door1",DOOR_Telegram);
    }    
    if (text == "/DisDoorAlarm") {
       doortelegram = 0;
       DOOR_Telegram = "OFF";
       bot.sendMessage(chat_id, "ปิดระบบตรวจจับประตูเรียบร้อยค่ะ", "");
       Firebase.setInt(firebaseData, "/home1/Token/Status/Door",doortelegram);
       Firebase.setString(firebaseData, "/home1/Token/Status/Door1",DOOR_Telegram);       
    }                
    if (text == "/FireWallON") {
      firewall = 1;
      SMOKE_Telegram = "ON";
      Fire_Telegram = "ON";
      bot.sendMessage(chat_id, "เปิด FireWall", "");
      Serial.println("เปิด FireWall");
      Firebase.setInt(firebaseData, "/home1/Token/Status/fire",firewall);
      Firebase.setString(firebaseData, "/home1/Token/Status/smoke",SMOKE_Telegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/fire1",Fire_Telegram);
      
    }    
    if (text == "/FireWallOFF") {
      firewall = 0;
      SMOKE_Telegram = "OFF";
      Fire_Telegram = "OFF";      
      bot.sendMessage(chat_id, "ปิด FireWall", "");
      Serial.println("ปิด FireWall");
      Firebase.setInt(firebaseData, "/home1/Token/Status/fire",firewall);
      Firebase.setString(firebaseData, "/home1/Token/Status/smoke",SMOKE_Telegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/fire1",Fire_Telegram);
      
    }
    if (text == "/PirON") {
       flag = 1;
       PIRStat = 1; //เปิดวงเล็บ
       PIR_Telegram = "ON";
       Firebase.setInt(firebaseData, "/home1/Camera/PIR",flag);
       bot.sendMessage(chat_id, "เปิดโหมดตรวจจับ", "");
       Firebase.setInt(firebaseData, "/home1/Token/Status/flag",flag);
       Firebase.setInt(firebaseData, "/home1/Token/Status/PIRStat",PIRStat);
       Firebase.setString(firebaseData, "/home1/Token/Status/PIRTelegram",PIR_Telegram);
       
    }
    if (text == "/PirOFF") {
       flag = 0;
       PIRStat = 0;
       PIR_Telegram = "OFF";
       Firebase.setInt(firebaseData, "/home1/Camera/PIR",flag);
       bot.sendMessage(chat_id, "ปิดโหมดตรวจจับ", "");
       Firebase.setInt(firebaseData, "/home1/Token/Status/flag",flag);
       Firebase.setInt(firebaseData, "/home1/Token/Status/PIRStat",PIRStat);
       Firebase.setString(firebaseData, "/home1/Token/Status/PIRTelegram",PIR_Telegram);       
    }
    if (text == "/Photo") {
       sendPhoto = 1;
       Firebase.setInt(firebaseData, "/home1/Camera/Photo",sendPhoto);
       bot.sendMessage(chat_id, "รับทราบค่ะ นายท่าน ^_^", "");
    }    
    if (text == "/IOT"){
      String keyboardJson = "[[{ \"text\" : \"TurnON\", \"callback_data\" : \"/TurnON\" },{ \"text\" : \"TurnOFF\", \"callback_data\" : \"/TurnOFF\" }],[{ \"text\" : \"Door\", \"callback_data\" : \"/Door\" },{ \"text\" : \"Fan\", \"callback_data\" : \"/Fan\" }]]";
      bot.sendMessageWithInlineKeyboard(chat_id, "ควบคุมเครื่องใช้ไฟฟ้าภายในบ้าน", "", keyboardJson);      
    }
    if (text == "/Mode"){
      String welcome = "โหมดการทำงาน\n";     
      welcome += "/FullAlarm : เมื่อไม่มีใครอยู่บ้าน\n";
      welcome += "/NightMode : ขณะเข้านอน\n";
      welcome += "/DayMode : ขณะอยู่ที่บ้าน\n";
      welcome += "/CloseAll : ปิดระบบรักษาความปลอดภัย\n";             
      bot.sendMessage(chatId, welcome, "Markdown");
    }       
    if (text == "/Door") {
      Doorstat = !Doorstat;
      digitalWrite(Door, Doorstat);
    } 
    if (text == "/Fan") {
      Fanstat = !Fanstat;
      digitalWrite(Fan, Fanstat);
    } 
    if (text == "/SirenOn") {
      bot.sendMessage(chatId, "เปิดเสียงให้แล้วนะคะ >< ", "");
      Siren_Telegram = "ON";
      sound = LOW;    
    }
    if (text == "/SirenOFF") {
      bot.sendMessage(chatId, "ปิดเสียงให้แล้วนะคะ 0.0", ""); 
      Siren_Telegram = "OFF";     
      sound = HIGH;    
    }
    if (text == "/CloseAll") {
      bot.sendMessage(chatId, "ทำการปิดระบบรักษาความปลอดภัย", "");
      sound = HIGH;
      doortelegram = 0;
      DOOR_Telegram = "OFF"; 
      flag = 0;
      PIRStat = 0;
      PIR_Telegram = "OFF";
      Line_stage = 0;
      Online_Telegram = "OFF";
      firewall = 0;
      SMOKE_Telegram = "OFF";
      Fire_Telegram = "OFF";
      FullAlarm = false;
      NightMode = false;
      DayMode = false;
      home1 = 0;
      home2 = 0;
      home3 = 0;
      home4 = 0;
      home5 = 0;
      Firebase.setInt(firebaseData, "home1/Social/home1",home1);
      Firebase.setInt(firebaseData, "home1/Social/home2",home2);
      Firebase.setInt(firebaseData, "home1/Social/home3",home3);
      Firebase.setInt(firebaseData, "home1/Social/home4",home4);
      Firebase.setInt(firebaseData, "home1/Social/home5",home5);   
      Firebase.setInt(firebaseData, "home1/Social/home1",home1);
      Firebase.setInt(firebaseData, "/home1/Camera/PIR",flag);  
      Firebase.setInt(firebaseData, "/home1/Mode/DayMode",DayMode);
      Firebase.setInt(firebaseData, "/home1/Mode/FullAlarm",FullAlarm);
      Firebase.setInt(firebaseData, "home1/Mode/NightMode",NightMode); 
      Firebase.setInt(firebaseData, "/home1/Token/Status/Door",doortelegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/Door1",DOOR_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/fire",firewall);
      Firebase.setString(firebaseData, "/home1/Token/Status/smoke",SMOKE_Telegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/fire1",Fire_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/Line",Line_stage);
      Firebase.setString(firebaseData, "/home1/Token/Status/Line1",Online_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/flag",flag);
      Firebase.setInt(firebaseData, "/home1/Token/Status/PIRStat",PIRStat);
      Firebase.setString(firebaseData, "/home1/Token/Status/PIRTelegram",PIR_Telegram);                         
    }            
    if (text == "/DayMode") {
      String welcome = "โหมด ขณะอยู่ในบ้าน\n";     
      welcome += "เซนเซอร์ และการแจ้งเตือนทุกอย่างจะถูกปิด จะเหลือแค่เพียงตัวตรวจจับอัคคีภัย\n";
      welcome += "/start : เมนูเพิ่มเติม\n";      
      bot.sendMessage(chatId, welcome, "Markdown");
      sound = HIGH;
      doortelegram = 0;
      DOOR_Telegram = "OFF"; 
      flag = 0;
      PIRStat = 0;
      PIR_Telegram = "OFF";
      Line_stage = 0;
      Online_Telegram = "OFF";
      firewall = 1;
      SMOKE_Telegram = "ON";
      Fire_Telegram = "ON";      
      DayMode = true;
      Firebase.setInt(firebaseData, "/home1/Camera/PIR",flag);
      Firebase.setInt(firebaseData, "/home1/Mode/DayMode",DayMode);
      Firebase.setInt(firebaseData, "/home1/Token/Status/Door",doortelegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/Door1",DOOR_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/fire",firewall);
      Firebase.setString(firebaseData, "/home1/Token/Status/smoke",SMOKE_Telegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/fire1",Fire_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/Line",Line_stage);
      Firebase.setString(firebaseData, "/home1/Token/Status/Line1",Online_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/flag",flag);
      Firebase.setInt(firebaseData, "/home1/Token/Status/PIRStat",PIRStat);
      Firebase.setString(firebaseData, "/home1/Token/Status/PIRTelegram",PIR_Telegram);                            
    }
    if (text == "/NightMode") {
      String welcome = "โหมด ขณะนอนหลับ\n";     
      welcome += "มีการเปิดใช้งานเซนเซอร์ป้องกันขโมย ตรวจจับอัคคีภัย และประตูหน้าต่าง พร้อมเสียงไซเรนเตือน หากเซนเซอร์จับการเคลื่อนไหวได้ไฟหน้าบ้านจะติด\n";
      welcome += "/start : เมนูเพิ่มเติม\n";            
      bot.sendMessage(chatId, welcome, "Markdown");
      NightMode = !NightMode;
      sound = LOW;
      doortelegram = 1;
      DOOR_Telegram = "ON"; 
      flag = 1;
      PIRStat = 1;
      PIR_Telegram = "ON";
      Line_stage = 0;
      Online_Telegram = "OFF";
      firewall = 1;
      SMOKE_Telegram = "ON";
      Fire_Telegram = "ON";      
      Firebase.setInt(firebaseData, "/home1/Camera/PIR",flag);
      Firebase.setInt(firebaseData, "home1/Mode/NightMode",NightMode);
      Firebase.setInt(firebaseData, "/home1/Token/Status/Door",doortelegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/Door1",DOOR_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/fire",firewall);
      Firebase.setString(firebaseData, "/home1/Token/Status/smoke",SMOKE_Telegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/fire1",Fire_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/Line",Line_stage);
      Firebase.setString(firebaseData, "/home1/Token/Status/Line1",Online_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/flag",flag);
      Firebase.setInt(firebaseData, "/home1/Token/Status/PIRStat",PIRStat);
      Firebase.setString(firebaseData, "/home1/Token/Status/PIRTelegram",PIR_Telegram);                         
         
    }
    if (text == "/FullAlarm") {
      String welcome = "เปิดโหมด ไม่มีใครอยู่บ้าน\n";     
      welcome += "เซนเซอร์ทุกอย่างจะถูกเปิด และมีการแรนด้อมเปิด-ปิดไฟให้เหมือนกับว่ามีคนอาศัยอยู่\n";
      welcome += "/start : เมนูเพิ่มเติม\n";                 
      bot.sendMessage(chatId, welcome, "Markdown");      
      sound = LOW;
      doortelegram = 1;
      DOOR_Telegram = "ON"; 
      flag = 1;
      PIRStat = 1;
      PIR_Telegram = "ON";
      FullAlarm = true;
      Line_stage = 1;
      Online_Telegram = "ON";
      firewall = 1;
      SMOKE_Telegram = "ON";
      Fire_Telegram = "ON";            
      Firebase.setInt(firebaseData, "/home1/Camera/PIR",flag);  
      Firebase.setInt(firebaseData, "/home1/Mode/FullAlarm",FullAlarm); 
      Firebase.setInt(firebaseData, "/home1/Token/Status/Door",doortelegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/Door1",DOOR_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/fire",firewall);
      Firebase.setString(firebaseData, "/home1/Token/Status/smoke",SMOKE_Telegram);
      Firebase.setString(firebaseData, "/home1/Token/Status/fire1",Fire_Telegram); 
      Firebase.setInt(firebaseData, "/home1/Token/Status/Line",Line_stage);
      Firebase.setString(firebaseData, "/home1/Token/Status/Line1",Online_Telegram);
      Firebase.setInt(firebaseData, "/home1/Token/Status/flag",flag);
      Firebase.setInt(firebaseData, "/home1/Token/Status/PIRStat",PIRStat);
      Firebase.setString(firebaseData, "/home1/Token/Status/PIRTelegram",PIR_Telegram);                        
    }
    if (text == "/Status") {
      String welcome = "สถานะต่าง ๆ ของเซนเซอร์\n";
      welcome += "ระบบตรวจจับผู้บุกรุก: " + String(PIR_Telegram) + "\n";
      welcome += "ระบบตรวจจับประตู: " + String(DOOR_Telegram) + "\n";
      welcome += "ระบบตรวจจับควัน: " + String(SMOKE_Telegram) + "\n";
      welcome += "ระบบตรวจจับเปลวไฟ: " + String(Fire_Telegram) + "\n";
      welcome += "เสียงไซเรน: " + String(Siren_Telegram) + "\n";
      welcome += "การแจ้งเตือนเพื่อนบ้าน: " + String(Online_Telegram) + "\n";            
      bot.sendMessage(chat_id, welcome, "Markdown");
    
      String keyboardJson = "[[{ \"text\" : \"Firebase\", \"url\" : \"https://console.firebase.google.com/u/0/project/esp32-e7cd8/database/esp32-e7cd8-default-rtdb/data\" }]]";
      bot.sendMessageWithInlineKeyboard(chat_id, "ดูสถานะได้ที่ Firebase", "", keyboardJson);
    }
    if (text == "/"){
      String keyboardJson = "[[{ \"text\" : \"Start\", \"callback_data\" : \"/start\" }]]";
      bot.sendMessageWithInlineKeyboard(chat_id, "กด /start เพื่อเริ่มต้นใช้งาน", "", keyboardJson);
    }     
    if (bot.messages[i].longitude != 0 || bot.messages[i].latitude != 0) {     
      Firebase.setString(firebaseData, "/home1/Location/Longitude",String(bot.messages[i].longitude, 6));
      Firebase.setString(firebaseData, "/home1/Location/Latitude",String(bot.messages[i].latitude, 6));  
      if (Firebase.getString(firebaseData, "/home1/Location/Latitude")) {
        if (firebaseData.dataType() == "string") {
         la = firebaseData.stringData();
       }
     }
      if (Firebase.getString(firebaseData, "/home1/Location/Longitude")) {
        if (firebaseData.dataType() == "string") {
         lon = firebaseData.stringData();
        }
     }           
      String welcome = "ทำการเซ็ตค่าตำแหน่งปัจจุบันเรียบร้อยค่ะ\n";
      welcome += "Long: " + String(bot.messages[i].longitude, 6) + "\n";
      welcome += "Lat: " + String(bot.messages[i].latitude, 6) + "\n";
      bot.sendMessage(chat_id, welcome, "Markdown");

    }             
                        
    if (text == "/start"){
      String welcome = "Home Security กดคำสั่งเพื่อสั่งการ\n";      
      welcome += "/HELP : ขอความช่วยเหลือ\n";
      welcome += "/ONLine  /OFFLine : แจ้งเตือนไปยัง Line เพื่อนบ้าน หรือยาม\n";
      welcome += "/DoorAlarm : เปิดระบบตรวจจับประตู\n";
      welcome += "/DisDoorAlarm : ปิดระบบตรวจจับประตู\n";
      welcome += "/FireWallON : เปิด FireWall\n";
      welcome += "/FireWallOFF : ปิด FireWall\n";
      welcome += "/PirON  /PirOFF : ระบบตรวจจับผู้บุกรุก\n";
      welcome += "/IOT : ควบคุมเครื่องใช้ไฟฟ้า\n";
      welcome += "/Mode : โหมดการทำงาน\n";
      welcome += "/Photo : ถ่ายรูป\n";      
      welcome += "/SirenOn /SirenOFF : เสียงไซเรน\n";
      welcome += "/Status : ดูสถานะต่าง ๆ\n";
      welcome += "/Home /Mute : สถานะบ้านข้าง ๆ\n";
      welcome += "/CloseAll : ปิดระบบรักษาความปลอดภัย\n"; 
      bot.sendMessage(chatId, welcome, "Markdown");
    }
  }
}
