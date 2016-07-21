
//#include <HttpClient.h>

#include <WiFi.h>

char ssid[50] = "ameba";  //Set the AP's SSID
char pass[50] = "amebaiot";     //Set the AP's password
char channel[] = "1";         //Set the AP's channel
int status = WL_IDLE_STATUS;     // the Wifi radio's status
bool isSTA = false;

WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //Init and set all Zones to LOW
  for(int i = 1; i <- 4; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  

  //Start the RTC assumption: RTC is initialized and is running
  //ds1307_init();

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  //check pin13 state and select AP or STA mode
  srvr_or_ap();

  

  
  //printWifiData();
  //printCurrentNet();
}


void loop() {

  //Check time and set the zones
  //displayTime();
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    String response = "";
    while (client.connected()) {
      
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        response = response + c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          servePage(client);
          /*
          client.println("<html>");
          
          // output the value of each analog input pin
          for (int i = 1; i <= 4; i++){
            client.print("Zone ");
            client.print(i);
            client.print(": ");
            if(digitalRead(i) == HIGH) client.print("ON");
            else client.print("OFF");
            client.println("<br />");
          }
          client.println("</html>");*/
          break;
          }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
      
    }
    Serial.println("Request: "+response);

    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");

    //if we get WiFi creds
    if(getCreds(response)){
      //save to NVM
      writeCredNVM();

      //start STA
      //1st disconnect, 2nd begin
    }
  }
}
