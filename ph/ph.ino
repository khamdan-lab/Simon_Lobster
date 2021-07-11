//Arduino code
#include <SoftwareSerial.h>
SoftwareSerial s(0,1);
#define SensorPin A0

// Ph Sensor
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

void setup() {
  Serial.begin(9600);
  s.begin(9600);
}
 
void loop() {
    for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
      { 
        buf[i]=analogRead(SensorPin);
        delay(10);
      }
      for(int i=0;i<9;i++)        //sort the analog from small to large
      {
        for(int j=i+1;j<10;j++)
        {
          if(buf[i]>buf[j])
          {
            temp=buf[i];  
            buf[i]=buf[j];
            buf[j]=temp;
          }
        }
  }
      avgValue=0;
      for(int i=2;i<8;i++)                      //take the average value of 6 center sample
      avgValue+=buf[i];
      float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
      float phValue1=(3.5*phValue);
      Serial.println(phValue1, 2);
    
    if(s.available()>0)
      {
       char c= s.read();
       if(c == 's'){
       s.write(phValue1);
       delay(2000);
       }
    }
}
