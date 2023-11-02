#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo1;
Servo myservo2;
int pos = 0;
int var1 = 90;
int var2 = 90;



void setup()

{

Serial.begin(9600);
while (!Serial);
myservo1.attach(9);
Serial.println("calibrating servo X...");
for(pos = 0; pos <= 180; pos += 1)
myservo1.write(50);
delay(1000);
myservo1.write(130);
delay(1000);
myservo1.write(var1);
delay(1000);
myservo2.attach(10);
Serial.println("calibrating servo Y...");
for(pos = 0; pos <= 180; pos += 1)
myservo2.write(50);
delay(1000);
myservo2.write(130);
delay(1000);
myservo2.write(var2);
delay(1000);
Serial.println("servo calibrated");
}

void loop() {
  
for(pos = 0; pos <= 180; pos += 1)
if (Serial.available())

{
  int state = Serial.read();
  Serial.println(state);
if (state == 49)
{
  if (var1 > 130)
  {
    Serial.println("objek diluar jangkauan");
  }
  else
  {
  var1 = (var1 + 10);
  myservo1.write(var1);
  }
}

if (state == 50)
{
  if (var1 < 50)
  {
    Serial.println("objek diluar jangkauan");
  }
  else
  {
  var1 = (var1 - 10);
  myservo1.write(var1);
  }
}
if (state == 51)
{
  if (var2 > 130)
  {
    Serial.println("objek diluar jangkauan");
  }
  else
  {
  var2 = (var2 + 10);
  myservo2.write(var2);
  }
}
if (state == 52)
{
  if (var2 < 50)
  {
    Serial.println("objek diluar jangkauan");
  }
  else
  {
  var2 = (var2 - 10);
  myservo2.write(var2);
  }
}
}
}

