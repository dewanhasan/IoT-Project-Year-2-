#include <WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_SI1145.h>

#define DHTTYPE DHT11 //Declaring DHT type
#define DHTPIN 5

//const char* ssid = "eir33567666";
//const char* password ="bAxuve8pFG";

const char* ssid = "DewansiPhone";
const char* password ="12345678";

WiFiServer server(80);

String header;

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
Adafruit_SI1145 sensor = Adafruit_SI1145();
    
void setup() {

  Serial.begin(115200);
  //Sensors connecting to the server
  dht.begin();  //DHT11
  bmp.begin();  //BMP180
  sensor.begin(); //Si1145

 // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,

    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() ) {  // loop while the client's connected

      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then

        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            delay(3000);
            
            //read Temperature function
            float temperature = readTemp(); 
            //read Humidity function
            float humidity = readHumi();
            //read Pressure function
            float pressure = readPressureBmp();
            //read altitude function
            float altitude = readAltBmp();
            //read uv function
            float uv = readUVSi1145();
            //read ir function
            float ir = readIRSi1145();
            //read visibility function
            float vis = readVISSi1145();
            
            //HTML and CSS code
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"utf-8\">");
            client.println("<meta http-equiv='refresh' content='5'>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.3.0/css/all.min.css\">");
            client.println("<title>Smart Weather Hub</title>");
            //CSS styling
            client.println("<style>body{ background-color: rgb(210,221,221);}");
            client.println(".flex-container{display: flex;flex-direction: column;background-color: rgb(210,221,221);align-items: center;}");
            client.println("h1{color:rgb(163,165,42); text-align: center;font-family: 'Times New Roman', Times, serif; font-size: 60px; text-decoration: underline; margin: 75px;border: 10px inset rgb(138, 228, 240);}");
            client.println(".p1{color: cadetblue; text-align: center; font-size: 30px; font-style: bold;}");
            client.println(".fa-temperature-low{color: cadetblue; font-size: 30px;}");
            client.println(".p2{color: aqua; text-align: center;font-size: 30px;font-size: bold;}");
            client.println(".fa-droplet{color:aqua; font-size: 30px;}");
            
            client.println(".p3{color: black; text-align: center; font-size: 30px; font-style: bold;}");
            client.println(".fa-solid-fa-wind{color: black; font-size: 30px;}");
            
            client.println(".p4{color: slategray;text-align: center;font-size: 30px;font-size: bold;}");
            client.println(".fa-solid-fa-chart-line{color: slategray; font-size: 30px;}");
            
            client.println(".p5{color: yellow; text-align: center; font-size: 30px; font-style: bold;}");
            client.println(".fa-sharp-fa-solid-fa-sun{color: yellow; font-size: 30px;}");
            
            client.println(".p6{color: orangered; text-align: center;font-size: 30px;font-size: bold;}");
            client.println(".fa-solid-fa-wifi{color: orangered; font-size: 30px;}");
            
            client.println(".p7{color: green; text-align: center;font-size: 30px;font-size: bold;}");
            client.println(".fa-solid-fa-eye-slash{color: green; font-size: 30px;}</style>");

            //Displaying Webpage with live values
            client.println("<body>");
            client.println("<div class=\"flex-Container\">");
            client.println("<h1>SMART WEATHER HUB</h1>");
            //DHT11
            client.println("<p class=\"p1\" ><i class=\"fa fa-temperature-low\"></i>Temperature:"); 
            client.println(temperature);
            client.println("°C </p>");
            client.println("<p class=\"p2\"><i class=\"fa fa-droplet\"></i> Humidity: ");
            client.println(humidity);
            client.println("% </p>");
            //BMP180
            client.println("<p class=\"p3\" ><i class=\"fa-solid fa-wind\"></i>Pressure:"); 
            client.println(pressure);
            client.println("hPa</p>");
            client.println("<p class=\"p4\"><i class=\"fa-solid fa-chart-line\"></i> Altitude: ");
            client.println(altitude);
            client.println("m</p>");
            //Si1145
            client.println("<p class=\"p5\" ><i class=\"fa-sharp fa-solid fa-sun\"></i>UV:"); 
            client.println(uv);
            client.println("</p>");
            client.println("<p class=\"p6\"><i class=\"fa-solid fa-wifi\"></i> IR: ");
            client.println(ir);
            client.println("</p>");
            client.println("<p class=\"p7\" ><i class=\"fa-solid fa-eye-slash\"></i>Visibility:"); 
            client.println(vis);
            client.println("km</p>");

            client.println("</div></body></head></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          } 
        }else if(c != '\r') {
            currentLine += c;
          }
        }
      }
        // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
}
