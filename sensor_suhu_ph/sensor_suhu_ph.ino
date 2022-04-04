#include <OneWire.h>
#include <DallasTemperature.h>
#define DS18B20PIN 13

/* Create an instance of OneWire */
OneWire oneWire(DS18B20PIN);

DallasTemperature sensors(&oneWire);


#define SensorPin 35            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;
void setup(void)
{
  Serial.begin(9600);
  Serial.println("pH meter experiment!");    //Test the serial monitor
}
void loop(void)
{

  //Sensor Suhu___________________________________________________________________________
  
  sensors.requestTemperatures(); 
  float tempinC = sensors.getTempCByIndex(0);
  Serial.print("Temperature = ");
  Serial.print(tempinC);
  Serial.println("ºC");
  delay(1000);

  //Sensor pH_____________________________________________________________________________
  
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,pHValues,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*3.3/1024;
      pHValue = 3.5*voltage+Offset;
      pHValues = pHValue-9;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(pHValues ,2);
        printTime=millis();
  }
     Serial.println("____________________________________________________________________");
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}