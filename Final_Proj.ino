 #include <Wire.h>
#include<EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 
RTC_DS1307 RTC;
int tmp,Inc,hor,mIn,add=11; 
int set=8;  
int cge=9;   
int mod=10; 
int off=0;
int sensor=A0;

float tempc;  //variable to store temperature in degree Celsius
float tempf;  //variable to store temperature in Fahreinheit 
float vout;  //temporary variable to hold sensor reading
int count_flag = 0;
#define buz 11 
int Hor,Min,Sec; 
///////////////////////////////////////Function to adjust the time//////////////////////////////////

void time()                             
{
    int tmp=1,mins=0,hors=0,secs=0;
    while(tmp==1)
    {
     off=0;
     if(digitalRead(cge)==0)
     {
      Hor++;
      if(Hor==24)
      {
       Hor=0;
      }
     }
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Set Alarm Time ");

    lcd.setCursor(0,1);
    if(Hor<=9)
    lcd.print("0");
    lcd.print(Hor);
    lcd.print(":");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    delay(200);
    lcd.setCursor(0,1);
    lcd.print("  ");
    lcd.print(":");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    delay(200);
    if(digitalRead(set)==0)
    {
      hor=Hor;
      EEPROM.write(add++,hor);
     tmp=2;
     while(digitalRead(set)==0);
    }
    }
    
    while(tmp==2)
    {
     if(digitalRead(cge)==0)
     {
      Min++;
      if(Min==60)
      {Min=0;}
     }
    lcd.setCursor(0,1);
    lcd.print(Hor);
    lcd.print(":");
     if(Min<=9)
    lcd.print("0");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    lcd.print("  ");
    delay(200);
     lcd.setCursor(0,1);
    lcd.print(Hor);
    lcd.print(":");
    lcd.print("  ");
    lcd.print(":");
    lcd.print(Sec);
    lcd.print("  ");
    delay(200);
      if(digitalRead(set)==0)
      {
       mIn=Min;
       EEPROM.write(add++, mIn);
       tmp=0;
       while(digitalRead(set)==0);
      }
    }
    off=1;
    delay(10);
}
///////////////////////////////////////////function to sound the buzzer//////////////////////////////////
void Buz()
{
  if(digitalRead(set)==0)
  off=0;
  if(off==1)
  {
   digitalWrite(buz,HIGH);
   delay(500);
  
   digitalWrite(buz, LOW);
   delay(500);
  }
}
//////////////////////////////////////////function to compare the alarm time with current RTC time//////////////////
void TimeCheck()
{
  int tem[17];
  for(int i=11;i<17;i++)
  {
    tem[i]=EEPROM.read(i);
  }
  
  
  if(Hor == tem[11] && Min == tem[12] && off==1) 
  {
   
   add=11;
   Buz();
   Buz();
   
   lcd.clear();
   lcd.print("Wake UP....");
   lcd.setCursor(0,1);
   lcd.print("...........alarm");
   Buz();
   Buz();
   
   }
}
////////////////////////////////////////////////////////////setup///////////////////////////
void setup()
{
 Wire.begin();
 RTC.begin();
 lcd.begin(16,2);
 pinMode(cge, INPUT);
 pinMode(set, INPUT);
 pinMode(mod, INPUT);
 pinMode(buz, OUTPUT);
 pinMode(sensor,INPUT);
 digitalWrite(set, HIGH);
 digitalWrite(mod, HIGH);
 digitalWrite(cge, HIGH);
 
 
   lcd.setCursor(0,0);
   lcd.print("Abhishek &");
   lcd.setCursor(0,1);
   lcd.print("Biswajeet");
   delay(2000);
    
 if(!RTC.isrunning())
 {
 RTC.adjust(DateTime(__DATE__,__TIME__));
 }
}
////////////////////////////////////////////////////////////loop/////////////////////////////////////
void loop()
{
   DateTime now = RTC.now();
   if(digitalRead(mod) == 0)      
   { 
    current();
    time();
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Alarm On");
    delay(2000);
   }
   
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Time:");
 lcd.setCursor(6,0);
 Hor=now.hour(),DEC; 
 if(Hor<=9)
 {
  lcd.print("0");
  lcd.print(Hor=now.hour(),DEC);
 }
 else
 lcd.print(Hor=now.hour(),DEC);
 lcd.print(":");
 Min=now.minute(),DEC;
 if(Min<=9)
 {
 lcd.print("0");
 lcd.print(Min=now.minute(),DEC);
 }
 else
 lcd.print(Min=now.minute(),DEC);
 
 lcd.print(":");
 Sec=now.second(),DEC;
 if(Sec<=9)
 {
  lcd.print("0");
  lcd.print(Sec=now.second(),DEC);
 }
 else
 lcd.print(Sec=now.second(),DEC);
 lcd.setCursor(0,1);
 lcd.print("Date: ");
 lcd.print(now.day(),DEC);
 lcd.print("/");
 lcd.print(now.month(),DEC);
 lcd.print("/");
 lcd.print(now.year(),DEC);
 TimeCheck();
 
 delay(2000);
 if(Min == EEPROM.read(12) + 1 && count_flag < 2)
   {
    count_flag++;
    lcd.clear();
    lcd.print("dds");
   int ttt= EEPROM.read(12);
   
   EEPROM.update(12,ttt+3);
   }
   if(count_flag >= 2)
   {
    count_flag = 0;
   }
 temp();
 
}

///////////////////////////////////////////////////////tempreture check//////////////////////////////////////////////
void temp()
{
//  int temp = analog.read()
//  Serial.print("Temperature:-");
//  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print("Temperature:-");
//  delay(300);

 vout=analogRead(sensor); 
 vout=(vout*500)/1023;
 tempc=vout; // Storing value in Degree Celsius
 tempf=(vout*1.8)+32; // Converting to Fahrenheit 
 lcd.setCursor(0,0);
 if(tempc > 90)
 {
    lcd.clear();
    lcd.print("Danger");
    delay(500);
    digitalWrite(buz,HIGH);
   delay(100);
  
   digitalWrite(buz, LOW);
   delay(100);
    digitalWrite(buz,HIGH);
   delay(100);
  
   digitalWrite(buz, LOW);
   delay(100);
   digitalWrite(buz,HIGH);
   delay(100);
  
   digitalWrite(buz, LOW);
   delay(100);
   digitalWrite(buz,HIGH);
   delay(100);
  
   digitalWrite(buz, LOW);
   delay(100);
 }
 if(tempc < 90){
 lcd.print("in DegreeC= ");
 lcd.print(tempc);
 lcd.setCursor(0,1);
 lcd.print("in DegreeF=");
 lcd.print(tempf);
 delay(2000); //Delay of 1 second for ease of viewing in serial monitor
 }
}


///////////////////////////////////////////////////////function to get current RTC time//////////////////////////////
void current()
{
  lcd.setCursor(0,1);
  lcd.print(Hor);
  lcd.print(":");
  lcd.print(Min);
  lcd.print(":");
  lcd.print(Sec);
}
