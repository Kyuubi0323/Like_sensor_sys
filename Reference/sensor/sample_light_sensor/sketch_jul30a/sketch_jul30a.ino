#define LIGHT 7 // define pint 7 for sensor
#define RELAY 4 // define pin 4 as for relay

void setup()
{
  Serial.begin(9600);
  pinMode(LIGHT, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
}

void loop()
{
  int L = digitalRead(LIGHT);

  if (L == 1)
  {
    Serial.println(" light is ON");
    digitalWrite(RELAY, LOW);
  }
  else
  {
    Serial.println("  === light is OFF");
    digitalWrite(RELAY, HIGH);
  }
  delay(500);
}
