#define volt_in A0
#define ohm A2
#define volt_detect 2
#define current_detect 4
#define ohm_detect 7
#define current_in A1
#define power_detect 5
#define rx 9
#define tx 8
int v_on,v_on1;
int c_on,c_on1;
int o_on,o_on1;
int p_on,p_on1;
float Vacross;
float Iamp=0;
float volt;
float r;
float Vr;
float avg;
float power;
String msgBuffer = "";
String masseg = "";
#include <LiquidCrystal_I2C.h>//for lcd
#include "SoftwareSerial.h"
SoftwareSerial BTSerial(rx,tx);//rx pin 9 for reciving and tx pin 8 is for sending
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
  //put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  //using app
  BTSerial.begin(9600);
  //voltmeter
  pinMode(volt_in,INPUT);
  pinMode(volt_detect,INPUT);
  //Ammeter
  pinMode(current_in, INPUT);
  pinMode(current_detect,INPUT);
  //ohmeter
  pinMode(ohm,INPUT);
  pinMode(ohm_detect,INPUT);
  //power
  pinMode(power,INPUT);
  pinMode(power_detect,INPUT);
 }
 void usingApp(){
  while(BTSerial.available()>0){
    char data = (char) BTSerial.read();
    msgBuffer += data;
    if (data == ';')
    {
      masseg = msgBuffer;
      msgBuffer = "";
    } 
  p_on1 = (masseg=="p;"|| p_on == HIGH) && v_on != HIGH && c_on != HIGH && o_on != HIGH ?HIGH:LOW;
  v_on1 = (masseg=="v;"|| v_on == HIGH) && p_on != HIGH && c_on != HIGH && o_on != HIGH ?HIGH:LOW;
  c_on1 = (masseg=="c;"|| c_on == HIGH) && v_on != HIGH && p_on != HIGH && o_on != HIGH ?HIGH:LOW;
  o_on1 = (masseg=="o;"|| o_on == HIGH) && v_on != HIGH && c_on != HIGH && p_on != HIGH ?HIGH:LOW;
  }
 }
//  void test(){
  // BTSerial.println(p_on);
  // BTSerial.println(v_on);
  // BTSerial.println(c_on);
  // BTSerial.println(o_on);
  // BTSerial.println("++++++1");
  // BTSerial.println(p_on1);
  // BTSerial.println(v_on1);
  // BTSerial.println(c_on1);
  // BTSerial.println(o_on1);
  // BTSerial.println("++++++2");
  //}
 
void loop() {
  //put your main code here, to run repeatedly:
  fflush(stdout);
  fflush(v_on);
  fflush(c_on);
  fflush(o_on);
  fflush(p_on);
  
  p_on = digitalRead(power_detect);
  v_on = digitalRead(volt_detect);
  c_on = digitalRead(current_detect);
  o_on = digitalRead(ohm_detect);
  usingApp();
  //test();
  for(int i=0;i<100;i++)
  {
  volt = (analogRead(volt_in))*(5.0/1023);
  volt = (volt*(100/5)-0.2);
  if(i!=0)
  {
  volt = volt+avg;
  }
  avg = volt;
  }
  volt = volt/100;
  avg = NULL;
  
  Vacross = (analogRead(current_in))*((5.0/ 1023.0)); 
  Iamp = ((Vacross)/22);
  for(int i=0;i<300;i++)
  {
  Vr = (analogRead(ohm))*(5.0/1023);
  r = ((2*Vr)/(1-(Vr/5)));
  delay(3);
  if(i!=0)
  {
  r = r+avg;
  }
  avg = r;
  }
  r = r/300;
  
//Voltmeter
if((v_on == HIGH || v_on1 == HIGH)&&(c_on == LOW)&&(o_on== LOW)&&(p_on == LOW))
{
lcd.print("Voltmeter");
lcd.setCursor(0,1);
lcd.print("Volt =");
lcd.setCursor(11,1);
lcd.print("V");  
lcd.setCursor(6,1);
lcd.print(volt);
delay(1000); 
volt = NULL;
lcd.clear();
}
//Ammeter:
else if((v_on == LOW)&&(c_on == HIGH || c_on1 == HIGH)&&(o_on== LOW)&&(p_on == LOW))
{
 lcd.print("Ammeter");
 lcd.setCursor(0,1);
 lcd.print("Current= "); 
 if(floor(Iamp))
 {
 lcd.setCursor(8,1);
 lcd.print(Iamp-0.44);
 lcd.setCursor(14,1);
 lcd.print("A");
 }
 else
 {
 Iamp = Iamp*1000;
 lcd.setCursor(8,1);
 lcd.print(Iamp-0.44);
 lcd.setCursor(14,1);
 lcd.print("mA");
 }
 delay(1000);
 Iamp = NULL;
 lcd.clear();
}
//Ohmmeter
else if((v_on == LOW )&&(c_on == LOW)&&(o_on == HIGH || o_on1 == HIGH)&&(p_on == LOW))
{
lcd.print("Ohmmeter");
lcd.setCursor(0,1);
lcd.print("R =");
if(floor(r))
{
lcd.setCursor(3,1);
lcd.print(r);
lcd.setCursor(12,1);
lcd.print("Kohm");
}
else
{
r = r*1000;
lcd.setCursor(3,1);
lcd.print(r);
lcd.setCursor(12,1);
lcd.print("ohm");
}
delay(1000);
r = NULL;
lcd.clear();
}
else if((v_on == LOW)&&(c_on == LOW)&&(o_on==LOW)&&(p_on == HIGH || p_on1 == HIGH))
{
  power = volt*Iamp;
  lcd.print("Wattmeter");
  lcd.setCursor(0,1);
  lcd.print("Power= ");
  if(floor(power))
  {
  lcd.setCursor(7,1);
  lcd.print(power);
  lcd.setCursor(12,1);
  lcd.print("Watt");
  }
  else
  {
  power = power*100;
  lcd.setCursor(7,1);
  lcd.print(power);
  lcd.setCursor(12,1);
  lcd.print("mw");
  }
  delay(1000);
  lcd.clear();
  power = NULL;
}
else
{
  lcd.print("Choose mode");
  delay(1000);
  lcd.clear();
}
}
