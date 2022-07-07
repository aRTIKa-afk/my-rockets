//bar bmp180
//axel gy-521
//standart cd
#include <SFE_BMP180.h>
#include <Wire.h>
#include "I2Cdev.h"                  
#include "MPU6050.h" 
#include <SPI.h>
#include <SD.h>

MPU6050 axel;                       // Создаем объект, символизирующий модуль датчика
int16_t ax, ay, az;                  // Переменные для хранения значений акселерометра
int16_t gx, gy, gz;                  // Переменные для хранения значений гироскоп


SFE_BMP180 pressure;

double baseline; 

File myFile;

void setup() {
  Wire.begin();   
  axel.initialize();
  Serial.begin(9600);
  
  SD.begin(4);
  //SD.remove("datadump.txt");
  myFile = SD.open("datadump.txt", FILE_WRITE);
  myFile.close();
  myFile = SD.open("datadump.txt", FILE_WRITE);
  
  Serial.print("Starting... \tstartTime:");Serial.println(millis());
  myFile.print("Starting... \tstartTime:");myFile.println(millis());
    // close the file:
  
  pressure.begin();

  baseline = getPressure();
  // put your setup code here, to run once: 
  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");

  myFile.print("baseline pressure: ");
  myFile.print(baseline);
  myFile.println(" mb");
  
  Serial.println("N\tH\tax\tay\taz\tgx\tgy\tgz\ttime");
  myFile.println("N\tH\tax\tay\taz\tgx\tgy\tgz\ttime");
  myFile.close();  
}

unsigned long i=0;

void loop() {
  
  double a,P;
  i++;
  String writeD;
  myFile = SD.open("datadump.txt", FILE_WRITE);
  
  P = getPressure();
  a = pressure.altitude(P, baseline);
  axel.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  writeD=String(i)+"\t"+String(a)+"\t"+String(ax)+"\t"+String(ay)+"\t"+String(az)+"\t"+String(gx)+"\t"+String(gy)+"\t"+String(gz)+"\t"+String(millis());
  
  Serial.print(i);Serial.print("\t");
  
  Serial.print(a, 1);Serial.print("\t");
  
  Serial.print(ax); Serial.print("\t");
  
  Serial.print(ay); Serial.print("\t");

  Serial.print(az); Serial.print("\t");
  
  Serial.print(gx); Serial.print("\t");
  
  Serial.print(gy); Serial.print("\t");
 
  Serial.print(gz); Serial.print("\t");
 
  Serial.println(millis());
  myFile.println(writeD);
  
  myFile.close();
  delay(100);


  
}

double getPressure(){
  char status;
  double T,P,p0,a;

  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}
