/*
 To turn it on:
 http://your-IP-address/$1
 
 To turn it off:
 http://your-IP-address/$2
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 */

#include <SPI.h>
#include <Ethernet.h>

boolean incoming = 0;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ip(192,168,0,13);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);


// Store the relay pins in an array for indexing
byte relays[] = {7, 6, 5, 4};
int relayStates[] = {0, 0, 0, 0};
const String STATES[] = { "CLOSED", "OPEN"}; 

void setup()
{
  pinMode(2, OUTPUT);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  
  //Set pins to output, close all relays
  for (int i = 0; i < 4; i++)
  {
    pinMode(relays[i], OUTPUT); 
    digitalWrite(relays[i], 0);
  } 
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
        //reads URL string from $ to first blank space
        if(incoming && c == ' '){ 
          incoming = 0;
        }
        if(c == '$'){ 
          incoming = 1; 
        }
        
        //Checks for the URL string $1 or $2
        if(incoming == 1){
          Serial.println(c);
          
          if(c == '1'){
            Serial.println("ON");
            setRelay(0, 1);
            server.println("<cke:html><cke:body bgcolor=#FFFFFF>On</cke:body></cke:html>");
            client.stop();
          }
          else if(c == '2'){
            Serial.println("OFF");
            setRelay(0, 0);
            server.println("<cke:html><cke:body bgcolor=#000000 text=#FFFFFF>Off</cke:body></cke:html>");
            client.stop();
          }
          else {
            Serial.println(c);
            client.print("<cke:html><cke:body bgcolor=#000000 text=#FFFFFF>Received ");
            server.print(c);
            client.println("</cke:body></cke:html> ");
          }
        
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
  }
}


/* relayId    identifies relay 0, 1, 2, or 3 *
 * relayState identifies Closed/Open  0/1         */
void setRelay(int relayId, int relayState)
{
  if (relayState != 0 && relayState != 1)
  {
    Serial.print("Error: Invalid relayState: ");
    Serial.println(relayState);
    Serial.println();
    return;
  }
  if (relayId != 0 && relayId != 1 && relayId != 2 && relayId != 3)
  {
    Serial.print("Error: Invalid relayId: ");
    Serial.println(relayId);
    Serial.println();
    return;
  }
  if (relayState == 0)
  {
    digitalWrite(relays[relayId], LOW);
    relayStates[relayId] = 0;
  }
  else 
  {
    digitalWrite(relays[relayId], HIGH);
    relayStates[relayId] = 1;
  }
}

void showRelayStates()
{
  for (int i = 0; i < 4; i++)
  {
     Serial.print("Relay #");
     Serial.print(i);
     Serial.print(", State: ");
     Serial.println(STATES[relayStates[i]]);
  } 
  Serial.println();
}
