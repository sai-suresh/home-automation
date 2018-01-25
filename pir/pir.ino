int data;
int data1;
void setup()
{
  Serial.begin(9600);
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  Serial.print("calibrating sensor");
  for(int i=0;i<30;i++)
  {
    Serial.print(".");
    delay(1000);
    
  }

Serial.println("done");
Serial.println("sensor active");
digitalWrite(13,HIGH);
digitalWrite(12,HIGH);
delay(2000);
digitalWrite(12,LOW);
delay(50);
}

void loop()
{
  data=digitalRead(10);
  data1=digitalRead(11);
  boolean lock=true;
  if(data==HIGH&&lock==true)
  {
    lock=false;
    digitalWrite(12,HIGH);
    delay(500);
    digitalWrite(12,LOW);
    delay(4000);
    lock=true;
  }
   if(data1==HIGH&&lock==true)
  {
    lock=false;
    digitalWrite(12,HIGH);
    delay(500);
    digitalWrite(12,LOW);
    delay(4000);
    lock=true;
  }
}


