#define BLYNK_TEMPLATE_ID "TMPLf_YztT2O"
#define BLYNK_DEVICE_NAME "NodeMCU"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#include "BlynkEdgent.h"
#include "DHT.h"

BlynkTimer timer;
#define DHTPIN 5       
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

float temp=0,humidity=0,moistureData=0;

BLYNK_WRITE(V0)
{
  if(param.asInt()==1){
    digitalWrite(2, HIGH);
  }
  else{
    digitalWrite(2, LOW);
  }
}
//
//BLYNK_CONNECTED()
//{
//  Blynk.syncVirtual(V0);  
//}

void sendSensor(){
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V6, temp); 
  Serial.println(temp);
  Serial.print("Temperature : ");
  Serial.print(temp);
  Serial.print("    Humidity : ");
  Serial.println(humidity);
}

void moisture() {
  Serial.println(moistureData);
  float value = analogRead(A0);  
  moistureData = map(value, 0, 1024, 200,0);
  Serial.println("Moisture: ");
  Serial.println(moistureData);
  Blynk.virtualWrite(V4, moistureData);
  if(moistureData<30)
  {    
    Blynk.email("dhruvmojila098@gmail.com", "Alert", "soil moisture is below 30");
    Blynk.logEvent("moisture_alert","soil moisture is below 30");
    delay(3000);
  }
}

void setup()
{
  pinMode(2, OUTPUT); // Initialise digital pin 2 as an output pin
  Serial.begin(115200);
  delay(100);
  BlynkEdgent.begin();
  dht.begin();
  timer.setInterval(100L, sendSensor);
  timer.setInterval(100L, moisture);
}

void loop() {
  BlynkEdgent.run();
  timer.run();
}
