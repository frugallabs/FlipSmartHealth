/*------------------------------------------------------------
This is the .cpp file for FLIP SMART HEALTH Library
This is the property of Frugal Labs Tech Solutions Pvt. Ltd.
Please refer lisence.txt for complete details.
-------------------------------------------------------------*/
#include "FlipSmartHealth.h"

FlipSmartHealth::FlipSmartHealth()
{   
  pinMode(pulsepin, INPUT);
  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT);
  pinMode(zpin, INPUT);	
}

void FlipSmartHealth::Init()
{
	calibrate();
}

/**************************************************************/
int FlipSmartHealth::getBpm()
{
   int addbpm;
   int avgbpm;
   int bpm;
   unsigned long frequency;
   unsigned long time;   
   int period = 2000;
   unsigned long death = 0;
   int starttime = 2000;
   int input = 0;
   int lastinput = 0;
   time = millis();
  input = digitalRead(pulsepin);

  if ((input != lastinput)&&(input == HIGH))
  {
    // If the pin state has just changed from low to high (edge detector)
     period = time - starttime; // Compute the time between the previous beat and the one that has just been detected
     starttime = time; // Define the new time reference for the next period computing
     death = time;
  }
  lastinput = input; // Save the current pin state for comparison at the next loop iteration

  if (period < 0)
  {
    frequency = 0;
  }
  else
  {
    frequency = 60000/period; // Compute the heart rate in beats per minute (bpm) with the period in milliseconds
  }
  
  /*if (frequency > 85 && frequency < 120)
  {
    for (int j=0; j<6; j++)
    {
      addbpm += frequency;
     //delay(60);
    }
      avgbpm = addbpm / 6;
      bpm = avgbpm - 5;
      //Serial.print("BPM: ");
      //Serial.println(bpm);
      delay(6);
      addbpm = 0;
      avgbpm = 0;
      return(bpm); 
  }
  
  if (frequency > 20 && frequency < 61)
  {
    bpm = map(frequency, 21,60,71,110);
    return(bpm);
  }
  
  else
  {*/
    for (int j=0; j<6; j++)
    {
      addbpm += frequency;
      //delay(5);
    }
      avgbpm = addbpm / 6;
      bpm = avgbpm;
      //int bpmx = map (bpm, 0, 200, 0, 120);
      //Serial.print("BPM: ");
      //Serial.println(bpm);
      delay(6); 
      addbpm = 0;
      avgbpm = 0;
      return(bpm); 
 // }
 // return(bpm);
}
/**************************************************************/

int FlipSmartHealth::readTemp()
{
  int temp1 = (analogRead(temppin)/9+5);
  int temp2 = (temp1*9/5+62);//32 not 62
  return(temp2);
}

/*************************************************************/
int FlipSmartHealth::getPedo()
{
int xaccl[5]={0};
int yaccl[5]={0};
int zaccl[5]={0};
  int totvect[5]={0};
  int totave[5]={0};


  for (int i=0;i<5;i++)
  {
    xaccl[i]=float(analogRead(xpin));
    delay(1);
    yaccl[i]=float(analogRead(ypin));
    delay(1);
    zaccl[i]=float(analogRead(zpin));
    delay(1);
    totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
    totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
    delay(60);

    //cal steps 
    if (totave[i]>threshhold && flag==0)
    {
       steps=steps+1;
       flag=1;
    }
    else if (totave[i] > threshhold && flag==1)
    {
        //do nothing 
    }
    if (totave[i] <threshhold  && flag==1)
    {
      flag=0;
    }
   // Serial.print("steps=");
   // Serial.println(steps);
   return(steps);
  }
  delay(60);
}
/**************************************************************/

void FlipSmartHealth::calibrate()
{
  Serial.begin(19200);
  digitalWrite(13,HIGH);
  Serial.println("Calibrating......");
  float sum=0;
  float sum1=0;
  float sum2=0;
  for (int i=0;i<5;i++)
  {
    xval[i]=float(analogRead(xpin));
    sum=xval[i]+sum;
  }
  delay(60);
  xavg=sum/5.0;
  Serial.println(xavg);
  for (int j=0;j<5;j++)
  {
    xval[j]=float(analogRead(xpin));
    sum1=xval[j]+sum1;
  }
  yavg=sum1/5.0;
  Serial.println(yavg);
  delay(60);
  for (int i=0;i<5;i++)
  {
    zval[i]=float(analogRead(zpin));
    sum2=zval[i]+sum2;
  }
  zavg=sum2/5.0;
  delay(60);
  Serial.println(zavg);
  digitalWrite(13,LOW);
  Serial.println("Calibration Successful!");
  //delay(900);
}
/**************************************************************/
