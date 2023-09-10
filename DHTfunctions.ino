 
//DHT11 Integration with the ESP32
float readTemp(){

float t = dht.readTemperature();
//reads temperature
return t;
}

float readHumi(){

float h = dht.readHumidity();
//reads humidity
return h;
}
