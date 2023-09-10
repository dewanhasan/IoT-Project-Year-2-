float readUVSi1145(){

float u = sensor.readUV();
//reads UV index
return u;
}

float readIRSi1145(){

float i = sensor.readIR();
//reads IR index
return i;
}

float readVISSi1145(){

float v = sensor.readVisible()/10;
//reads visibility
return v;
}
