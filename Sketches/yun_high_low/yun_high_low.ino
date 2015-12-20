#include <Bridge.h>
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

const int ledPin = 13; 
int incomingByte = 0;

void setup() {
  Bridge.begin();
  Console.begin();
  
  while(!Console) {;}
  Console.println("Console connected!");
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (Console.available() > 0) {
    incomingByte = Console.read();
    if (incomingByte == 'H') {
      digitalWrite(ledPin, HIGH);
      Console.println("H");
    }
    if (incomingByte == 'L') {
      digitalWrite(ledPin, LOW);
      Console.println("L");
    }
    delay(100);
  }
}
