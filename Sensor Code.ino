#define BLYNK_TEMPLATE_ID "TMPL3Zeueh-7F"
#define BLYNK_TEMPLATE_NAME "Smart Sewage Monitoring"
#define BLYNK_AUTH_TOKEN "onkdLqTuan7rDSnssll-4ODADwYAI1uT"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <Blynk.h>

const int trigPin1 = D3;
const int echoPin1 = D2;
#define redled D4

// char auth[] = "aX3Cded2oxBxxxxx9bAVEGt1U";
char ssid[] = "Siddhesh";
char pass[] = "01234567";

int flowPin = D5; // Pin connected to flow sensor
volatile int pulseCount = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
BlynkTimer timer;

void ICACHE_RAM_ATTR pulseCounter()
{
  pulseCount++;
}

void flow()
{
  // Measure the flow rate
  pulseCount = 0;
  attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, FALLING);
  delay(1000);
  detachInterrupt(digitalPinToInterrupt(flowPin));
  flowMilliLitres = pulseCount / 7.5;
  totalMilliLitres += flowMilliLitres;
  flowRate = ((float)flowMilliLitres / 1000.0) * 60.0;
  Blynk.virtualWrite(V1, flowRate);
  // Serial.println(flowRate);
}

int duration1, distance1;
void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(A0, INPUT);
  digitalWrite(A0, OUTPUT);
  pinMode(redled, OUTPUT);
  digitalWrite(redled, LOW);
  Serial.begin(9600);
  pinMode(flowPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, FALLING);
  timer.setInterval(1000L, flow);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // WiFi.begin(ssid, pass);
  // Blynk.config(BLYNK_AUTH_TOKEN);
  // Blynk.connect();
}

void loop() {
  // int float1 = analogRead(A0);
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  // Serial.println(float1);
  Blynk.virtualWrite(V1, distance1);
  // Serial.println(distance1);
 if (flowRate > 3) {
    Blynk.virtualWrite(V2, "Blockage Alert!");
    // Serial.println("Blockage Alert");
    digitalWrite(redled, HIGH);
    
  }
  else if (distance1 <= 3) {
    Blynk.virtualWrite(V2, "Overflow Alert!");
    // Serial.println("Blockage Alert");
    digitalWrite(redled, HIGH);
  }
  else {
    digitalWrite(redled, LOW);
  }
  Blynk.run();
  timer.run();
}