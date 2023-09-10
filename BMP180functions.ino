float readPressureBmp(){

float p = bmp.readPressure() / 100;
//reads pressure
return p;
}

float readAltBmp(){

float a = bmp.readAltitude();
//reads altitude
return a;
}
