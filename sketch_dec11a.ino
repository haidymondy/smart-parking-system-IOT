#include <WiFi.h>
#include <ESP32Servo.h>
// #include <LiquidCrystal_I2c.h>
//#include<Esp32WiFi.h>
const char* ssid = "WE2BDDA1";          // Enter SSID here
const char* password = "K9121012@";  //Enter Password here
//ESP32WebServer server(80);.
WiFiServer server(80);
// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

Servo myservo;
bool tmp1=false;
bool tmp2=false;
float dis1 , duration1 , dis2 , duration2;
int ir = 34;
bool t ;
int trigPin1 = 22 , echoPin1 = 23 , trigPin2 = 25 , echoPin2 = 26 ;
int led1 = 4 , led2 = 2;
void setup() {
  pinMode(ir,INPUT);
  myservo.attach(32);
  myservo.write(0);
  pinMode(trigPin1,OUTPUT);//trig
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);//trig
  pinMode(echoPin2,INPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);

//web server code
  //////////////////////////////////////////////////////
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  delay(100);

  Serial.begin(115200);
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

int v = digitalRead(ir);
Serial.print("ir    :");
Serial.println(v);
if(v==0 && (output27State == "off" || output26State == "off"))
{
  myservo.write(90);
  delay(5000);
  myservo.write(0);
}
else if (v==0 && output27State == "on" && output26State == "on")
{
  myservo.write(0);
}
digitalWrite(trigPin1 , LOW);
delayMicroseconds(5); 
digitalWrite(trigPin1 , HIGH);
delayMicroseconds(5); 
digitalWrite(trigPin1 , LOW);
duration1 = pulseIn(echoPin1, HIGH);
dis1 = duration1/29/2;
Serial.print("dis1    :");
Serial.println(dis1);
if(dis1 < 10)
{
  digitalWrite(led1 , LOW);
  tmp1=true;
  Serial.println("park 1 is full");
  output26State = "on";

}
else if (dis1 > 10 && tmp1 == true)
{
  digitalWrite(led1 , HIGH);
  myservo.write(90);
  delay(3000);
  myservo.write(0);
  tmp1 = false;
  Serial.println("park 1 is empty");
  output26State = "off";
}


  delay(1000);
digitalWrite(trigPin2 , LOW);
delayMicroseconds(5); 
digitalWrite(trigPin2 , HIGH);
delayMicroseconds(5); 
digitalWrite(trigPin2 , LOW);
duration2 = pulseIn(echoPin2, HIGH);
dis2 = duration2/29/2;
  Serial.print("dis2    :");

Serial.println(dis2);
if(dis2 < 10 )
{
  digitalWrite(led2 , LOW);
  tmp2=true;
  Serial.println("park 2 is full");
  output27State = "on";
}
else if (dis2 > 10 && tmp2==true)
{
  digitalWrite(led2 , HIGH);
  myservo.write(90);
  delay(3000);
  myservo.write(0);
  tmp2 = false;
  Serial.println("park 2 is empty");
  output27State = "off";
}
delay(1000);


 WiFiClient client = server.available();  // Listen for incoming clients
  if (client) {                            // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");                                             // print a message out in the serial port
    String currentLine = "";                                                   // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {  // if there's bytes to read from the client,
        char c = client.read();  // read a byte, then
        Serial.write(c);         // print it out the serial monitor
        header += c;
        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();



            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // Web Page Heading
            client.println("<body><h1>Parking System Web Server</h1>");

            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO 26 - State " + output26State + "</p>");

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO 27 - State " + output27State + "</p>");

            client.println("</body></html>");
            client.println();
            header = "";
            // Close the connection
            client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
            client.println();
            // Break out of the while loop
            break;
          } else {  // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
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


  
//  digitalWrite(D0,HIGH );
//  digitalWrite(D4, LOW );
//  delay(1000);
//  
//  digitalWrite(D0,LOW );
//  digitalWrite(D4,HIGH );
//  delay(1000);


