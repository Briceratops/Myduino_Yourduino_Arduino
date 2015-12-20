float temp;
int tempPin = 1;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();
    if (incomingByte == 70 || incomingByte == 67) {
      Serial.println(readTemp(incomingByte));
    }
    delay(1000);
  }
}

float readTemp(int inByte)
{
  temp = analogRead(tempPin);
  temp = (temp * 0.48828125);
  if (inByte == 70) {
    temp = (temp * 1.8) + 32;
  }
  return temp;
}
