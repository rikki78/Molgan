#define MY_RADIO_NRF24
#define MY_BAUD_RATE  9600 // Sets the serial baud rate for console and serial gateway
#define SV "2.1"

#include "sensor_defines.h"
#include <EEPROM.h>
#include <readVcc.h>
#include <MySensors.h>
#include <DHT.h>  

#define MAX_RESEND    5

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_MOT 2   // Id of the sensor child
#define CHILD_ID_LIGHT 3
#define CHILD_ID_LIGHT2 4
#define CHILD_ID_SWITCH 5

#define HUMIDITY_SENSOR_DIGITAL_PIN 4

#define LIGHT_SENSOR_ANALOG_PIN A0
#define LIGHT_SENSOR_SWITCH_PIN 5

#define DIGITAL_INPUT_SENSOR 3   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
#define INTERRUPT 1 // Usually the interrupt = pin -2 (on uno/nano anyway)

#define BATT_VOLT_MEAS_PIN A1

#ifdef BATT_3V
  #define MIN_V   2400
  #define MAX_V   3200
#endif

#ifdef BATT_4_5V
  #define MIN_V   2500
  #define MAX_V   4500
#endif

#define FAST_LIGHT_CHANGE_DIFF 10 


unsigned long sleepTime = SHORT_SLEEP_TIME; // Sleep time between reads (in milliseconds)

byte useLedIndicator = 0;
byte motionCounter = 0;
byte lightCounter = 0, lightCounterTempHum = 0;


DHT dht;
float lastTemp;
float lastHum;
int   oldBatteryPcnt, batteryPcnt;
int   lastLightLevel;
int   intervalDHT, tempHumReadFail;
boolean metric = true, motion, lastMotion, wakeByMotion; 
long batteryVoltage;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgMot(CHILD_ID_MOT, V_TRIPPED);
MyMessage msgLight(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
MyMessage msgSwitch(CHILD_ID_SWITCH, V_LIGHT);

void before()  
{ 
    // This will execute before MySensors starts up
}

void setup()  
{ 
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Arduino init");
  #endif
  // Use internal 1.1V Reference (for Battery Measurement)
  // analogReference(INTERNAL);
  
  #ifdef USE_TEMP_HUM
    dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 
  #endif
  
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input
  pinMode(LIGHT_SENSOR_ANALOG_PIN, INPUT); 
  pinMode(LIGHT_SENSOR_SWITCH_PIN, OUTPUT);
  
  pinMode(BATT_VOLT_MEAS_PIN, INPUT); 
  
  metric = getConfig().isMetric;

}


void presentation()  
{ 
  // Send the Sketch Version Information to the Gateway
  sendSketchInfo(SN, SV);
  // Register all sensors to node (they will be created as child devices)
  #ifdef USE_TEMP_HUM
    present(CHILD_ID_HUM, S_HUM);
    present(CHILD_ID_TEMP, S_TEMP);
  #endif
  present(CHILD_ID_MOT, S_MOTION);
  present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
  present(CHILD_ID_SWITCH, S_LIGHT);
}

void receive(const MyMessage &message) 
{
}
  
void resend(MyMessage &msg, int repeats) // Resend messages if not received by 
{
  int repeat = 0;
  int repeatDelay = 0;
  boolean ack = false;

  while ((ack == false) and (repeat < repeats)) 
  {
    if (send(msg)) 
      ack = true;
    else 
    {
      ack = false;
      repeatDelay += 100;
    } 
    repeat++;
    wait(repeatDelay);
  }/* 
  if (ack)
  {
    Serial.print("Send ok ");
    Serial.println(repeat);
  }
  else
    Serial.println("Send fail"); */
}

void sendMotion()
{
  // Read digital motion value
  boolean motion = digitalRead(DIGITAL_INPUT_SENSOR) == HIGH; 
  if (lastMotion != motion) 
  {
    lastMotion = motion;     
    #ifdef DEBUG
      Serial.print("motion ");
      Serial.println(motion ? "1" : "0" );
    #endif  
    msgMot.set(motion ? "1" : "0" );
    resend(msgMot, 5);
  }
}

void sendTempHum()
{
  if (tempHumReadFail > 100)
  {
    #ifdef DEBUG
      Serial.println("No DHT connected");
    #endif
    return;
  } 
  delay(dht.getMinimumSamplingPeriod()); 
  float temperature = dht.getTemperature();
  if (isnan(temperature)) 
  {
    #ifdef DEBUG
      Serial.println("Failed reading temperature from DHT");
    #endif  
    tempHumReadFail++;
  } 
  else 
  {
    tempHumReadFail = 0;
    if (temperature != lastTemp) 
    {
      lastTemp = temperature;
      send(msgTemp.set(temperature, 1));
      #ifdef DEBUG
        Serial.print("T: ");
        Serial.println(temperature);
      #endif  
    }
  }

  float humidity = dht.getHumidity();
  if (isnan(humidity)) 
  {
    #ifdef DEBUG
      Serial.println("Failed reading humidity from DHT");
    #endif  
    tempHumReadFail++;
  } 
  else 
  {
    tempHumReadFail = 0;
    if (humidity != lastHum) 
    {
      lastHum = humidity;
      send(msgHum.set(humidity, 1));
      #ifdef DEBUG
        Serial.print("H: ");
        Serial.println(humidity);
      #endif  
    }
  }

}

void sendLightValue()
{
  int delta;
  
  #ifdef LOW_LIGHT_AREA
    analogReference(INTERNAL);
  #else
    analogReference(DEFAULT);
  #endif
  
  digitalWrite(LIGHT_SENSOR_SWITCH_PIN, LOW);
  analogRead(LIGHT_SENSOR_ANALOG_PIN);// read once to make sure the next reading is ok
  wait(5);
  int sensorValue = analogRead(LIGHT_SENSOR_ANALOG_PIN);
  if (sensorValue > MAX_LIGHT_VAL)
    sensorValue = MAX_LIGHT_VAL;
  sensorValue /= (MAX_LIGHT_VAL / 100.0);
  digitalWrite(LIGHT_SENSOR_SWITCH_PIN, HIGH);  

  delta = sensorValue - lastLightLevel;

  if (motionCounter > 10 || !wakeByMotion || (delta >= FAST_LIGHT_CHANGE_DIFF) || (delta <= -FAST_LIGHT_CHANGE_DIFF) )
  {
    if (sensorValue != lastLightLevel) 
    {
      resend(msgLight.set(sensorValue), 3);
      lastLightLevel = sensorValue;
      #ifdef DEBUG
        Serial.print("L1: ");
        Serial.println(sensorValue);
      #endif
    }
    // sendLightValue();
    
    motionCounter = 0;
    if (lastLightLevel < LONG_SLEEP_LIGHT_LEVEL_LO || lastLightLevel > LONG_SLEEP_LIGHT_LEVEL_HI)
      sleepTime = LONG_SLEEP_TIME;
    else
      sleepTime = SHORT_SLEEP_TIME;
    /* Serial.print("Sleep ");
    Serial.println(sleepTime);
     */  
  }
  
}


void sendBattery()
{
  analogReference(INTERNAL);
  for (int i = 0; i < 52; i++)
    analogRead(BATT_VOLT_MEAS_PIN); // read x times after switching to internal

  unsigned long sensorValue2 = analogRead(BATT_VOLT_MEAS_PIN);
  #ifdef BATT_4_5V
    float batteryV  = ((float)sensorValue2 * 1.1 * ((1E+6+330E+3)/330E+3));		// read value / 1024 * maxVoltageBattery * r1+r2/r2
    batteryVoltage  = ((unsigned long)sensorValue2 * 1100L * ((1E+6+330E+3)/330E+3)) / 1023;		// read value / 1024 * maxVoltageBattery * r1+r2/r2
    batteryPcnt = min(map(batteryVoltage, MIN_V, MAX_V, 0, 100), 100); // Convert voltage to percentage
  #endif  // BATT_4_5V

  #ifdef BATT_3V
    batteryVoltage = readVcc();
    batteryPcnt = min(map(batteryVoltage, MIN_V, MAX_V, 0, 100), 100); // Convert voltage to percentage
  #endif

  #ifdef DEBUG
    Serial.print("Battery Voltage analog read: ");
    Serial.print(sensorValue2);
    Serial.print("Battery Voltage: ");
    Serial.print(batteryVoltage);
    Serial.println("mV");
    Serial.print("Battery percentage: ");
    Serial.println(batteryPcnt);
  #endif
  
  if (oldBatteryPcnt != batteryPcnt) 
  {
    #ifdef DEBUG
      Serial.print("Battery percentage: ");
      Serial.println(batteryPcnt);
    #endif
    sendBatteryLevel(batteryPcnt);
    oldBatteryPcnt = batteryPcnt;
  } 	
}

void loop()      

{  
  if (wakeByMotion)
  {
    motionCounter++;
    sendMotion();
  }	
  sendLightValue();
  if (!wakeByMotion)
  {
    #ifdef USE_TEMP_HUM
      if (lightCounterTempHum++ > 5)
      {
          sendTempHum();
          wait(50);
        lightCounterTempHum = 0;
      }
    #endif
    if (lightCounter++ > 10)
    {
      sendBattery();
      lightCounter = 0;
    }
    
  }


  // Sleep until interrupt comes in on motion sensor. Send update every x minute. 
  if (sleep(INTERRUPT,CHANGE, sleepTime) == -1)  // -1 = by timer, 1 = by interrupt
  {
    wakeByMotion = false;
  //	Serial.println("Woke by timer");
	}
  else  
    wakeByMotion = true;
}

