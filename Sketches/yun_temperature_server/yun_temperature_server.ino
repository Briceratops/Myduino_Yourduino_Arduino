#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

YunServer server;
String startString;
long hits = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  
  server.listenOnLocalhost();
  server.begin();
  
  Process startTime;
  startTime.runShellCommand("date");
  while(startTime.available()) {
    char c = startTime.read();
    startString += c;
  }  
}

void loop() {
  YunClient client = server.accept();
  
  if (client) {
    String command = client.readString();
    command.trim();
    Serial.println(command);
    
    if (command == "temperature") {
      Process time;
      time.runShellCommand("date");
      String timeString = "";
      while(time.available()) {
        char c = time.read();
        timeString += c;
      }
      Serial.println(timeString);
      int sensorValue = analogRead(A1);
      
      float voltage = sensorValue * (5000/1024);
      float temperature = (voltage - 500)/10;
      temperature = (temperature * (9/5)) + 32;
      
      client.print("Current time on the YUN: ");
      client.println(timeString);
      client.print("<br>Current temperature: ");
      client.print(temperature);
      client.println(" degrees F");
      client.print("<br>Sketch has been running since: ");
      client.println(startString);
      client.println("<br>Hits so far: ");
      client.print(hits);
      
      client.stop();
      hits++;
      
      delay(50);
    }
  }
}
