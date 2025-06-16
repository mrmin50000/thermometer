#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);
void setup() {
Serial.begin(9600); 
dht.begin(); 
}
void loop() {

float humidity = dht.readHumidity();
float temperature = dht.readTemperature();

if (isnan(humidity) || isnan(temperature)) {
Serial.println("Не удалось считать данные с датчика DHT11");
return;
}

Serial.print("Влажность: ");
Serial.print(humidity);
Serial.print("%\t");
Serial.print("Температура: ");
Serial.print(temperature);
Serial.println("°C");

delay(2000);
}