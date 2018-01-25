#include<SoftwareSerial.h>
int data;
int data1;
boolean lock=true;
int counter = 0;
int Light_status=10;
SoftwareSerial esp8266(3,4); //Software Serial RX=3,TX=4
#define SSID "xxxx" //replace xxxx with wifi network name
#define PASS "xxxx" //replace xxxx with password

String sendAT(String command,const int timeout){
  String response="";
  esp8266.print(command);
  long int time = millis();
  while((time + timeout)>millis()){
    while(esp8266.available()){
      char c=esp8266.read();
      response += c;
    }
  }
  Serial.print(response);
  return response;
}

void connectwifi(){
  sendAT("AT\r\n",1000);
  sendAT("AT+CWMODE=1\r\n",1000); //call AT function to set esp8266 to station name
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n",2000); //AT command to connect with the wifi nettwork
  while(!esp8266.find("OK")){
    }
  sendAT("AT+CIFSR\r\n",1000); //AT command to print address on Serial Monitor
  sendAT("AT+CIPMUX=0\r\n",1000); //AT command  to set esp8266 to single connection
}

void setup(){
  esp8266.begin(9600); //begin Software Serial Communication 
  Serial.begin(9600); //begin Software Serial Communication with baud rate 9600
  sendAT("AT+RST\r\n",2000); //call AT function to sent reset command
  connectwifi();
  pinMode(10,INPUT); //sensor is connected
  pinMode(11,INPUT); //sensor is connected
  pinMode(8,OUTPUT); //bulb is connected via relay
  pinMode(13,OUTPUT); //onboard led
  pinMode(12,OUTPUT); //buzzer
  Serial.print("Calibrating Sensor");
  for(int i=0;i<=30;i++){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("done");
  Serial.println("Sensor active");
  digitalWrite(12,HIGH);
  delay(2000);
  digitalWrite(12,LOW);
  delay(50);
}

void loop(){

  data=digitalRead(10);
  data1=digitalRead(11);

  if(data==HIGH&&lock==true){ //outside sensor
    lock=false;
    digitalWrite(12,HIGH);
    delay(500);
    digitalWrite(12,LOW);
    Serial.print("Sensor IN");
    counter++;
    if(counter>0){
      digitalWrite(8,HIGH);
      Light_status=100;
      passTS(counter,Light_status);
      }
    delay(4000);
    lock=true;}
    
  if(data1==HIGH&&lock==true){
    lock=false;
    digitalWrite(12,HIGH);
    delay(500);
    digitalWrite(12,LOW);
    Serial.print("SensorOUT");
    counter--;
    if(counter==0){
      digitalWrite(8,LOW);
      Light_status=10;
      }
    passTS(counter,Light_status);
    delay(4000);
    lock=true; 
    }
}

void passTS(int c,int l){
  String counter1=String(c); //convert integer to string
  Serial.print("Number of Persons:");
  Serial.println(c);

  String Light_status1=String(l);
  Serial.print("Light status:");
  Serial.println(c);
  updateTS(counter1,Light_status1);
  delay(3000);
}
  
void updateTS(String C,String L){
  Serial.print("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n",1000);
  delay(2000);
  String cmdlen;
  String cmd="GET /update?key=xxxxxx&field1="+C+"&field2="+L+"\r\n"; // update Thinkspeak channel , replace xxxxxx with your Thinkspeak channel write API key
  cmdlen=cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n",2000);
  esp8266.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n",2000);
  Serial.println("");
  delay(1000);
\
