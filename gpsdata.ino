#include <string.h>
#include <SoftwareSerial.h>
#include<stdio.h>
SoftwareSerial mySerial(3, 2); // RX, TX
void(* resetFunc) (void) = 0;//declare reset function at address 0 and Call this reset the MCU!!!

//Global variables
int ch; // Buffer int
char a ; // Buffer Char
char arr[300]; // String for GSM
int i = 0; //Refresing the Index of the Char Array
char * allac; //String Parsing
int y = 0;
String lat; //Latitude Variable
String longe; //longeitude Variable


void pwrGPRSgps(){ //This function is for Powering up GPRS and GPS
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\""); //Specifying the connection type GPRS
  //delay(50); 
//  Serial.print(mySerial.readString()); // for debug purposes
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"www\""); //Setting Up APN for Vodafone its WWW
  //delay(50);
//  Serial.print(mySerial.readString());
  mySerial.println("AT+SAPBR =1,1"); //Enable GPRS
  mySerial.println("AT+SAPBR=2,1");
//  Serial.print(mySerial.readString());
  mySerial.println("AT+CGNSPWR=1"); // Powering On the GPS & GLONASS
//  Serial.print(mySerial.readString());
  mySerial.println("AT+HTTPINIT"); //Intilization of the HTTP Protocol
  //delay(50);
//  Serial.print(mySerial.readString());
  mySerial.println("AT+HTTPPARA=\"CID\",1"); // Setting Up the HTTP bearer Profile identifier
  //delay(50);
//  Serial.print(mySerial.readString());
}

void http(String a, String b){
  String urlMain = "\"http://ksathvik95.000webhostapp.com/?"; // web address
  String inpGps = String("lat=" + a + "&longe=" + b); // Importing data to the String
  urlMain = String(urlMain + inpGps); // Going Co-ordinates
  urlMain = String("AT+HTTPPARA=\"URL\"," + urlMain+"\""); // joining the String to form the GET Method Url
//  Serial.println(urlMain);
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  mySerial.flush(); //for ignoring the incoming data while sending data
  mySerial.println(urlMain); //sending it to the SIM Module
//  Serial.print(mySerial.readString());
  mySerial.flush();
  mySerial.println("AT+HTTPACTION=0"); // Sending to the Web
//  Serial.print(mySerial.readString());
}



void setup() {
  // Open serial communications and wait for port to open:
//  Serial.begin(9600);
//  Serial.println("Goodnight moon!");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(15000); //Delay the GSM Activation for a while to setup while it restarts
  pwrGPRSgps();
}

void loop() {
 while (mySerial.available()) { //checking for Connection of SIM Com Module
    ch = mySerial.read(); //reading the ascii values
    a = (char)ch; //coverting them to char to read as digits
    //Serial.print(a);
    arr[i] = a; //Storing them to Array
    i++; //incrementing the index of Array
  }
  allac = strtok (arr, ","); // A Function for Sperating the String with ","
  y = 0;
  while (allac != NULL){ //whenn it is not empty
    if (y == 3) { //postion of the Latitude of in the String
      //Serial.println(allac);
      lat = String(allac); //Storing in the Variable for further usage
    }
     if (y == 4) {//postion of the longeitude of in the String
      //Serial.println(allac);
      longe = String(allac);
    }
    y++;
    allac = strtok (NULL, ",");
  }
  http(lat,longe); //Sending Co-ordinates to pass the values to Module
  i = 0;
  delay(500);
  pwrGPRSgps();
  if (!(mySerial.available())) { //When Serial is not Available
    delay(500);
    mySerial.println("AT+CGNSINF"); // Ask for Co-ordinate Values
  }
  delay(10000); //Wait for 5 seconds
}
