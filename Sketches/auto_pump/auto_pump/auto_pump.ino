

byte relays[] = {2, 3, 4, 5};
int relayStates[] = {0, 0, 0, 0};
const String STATES[] = { "CLOSED", "OPEN"}; 
void setup ()
{
  Serial.begin(9600);
  
  //Set pins to output, close all relays
  for (int i = 0; i < 4; i++)
  {
    pinMode(relays[i], OUTPUT); 
    digitalWrite(relays[i], 0);
  } 
}

void loop ()
{
  if (Serial.available() > 0)
  {
    if (Serial.available() == 2)
    {
      int bytes[] = {Serial.read(), Serial.read()};

      int relayId = bytes[0] - 48;
      int relayState = -1;
      if (bytes[1] == 70 || bytes[1] == 102)
        relayState = 0;
      else if (bytes[1] == 79 || bytes[1] == 111)
        relayState = 1;
      Serial.print("Setting Relay #");
      Serial.print(relayId);
      Serial.print("'s Status to: ");
      Serial.println(STATES[relayState]);
      setRelay(relayId, relayState);
      showRelayStates();
    }
  }
  delay(500);
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
