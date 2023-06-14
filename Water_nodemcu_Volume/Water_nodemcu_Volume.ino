int sensorPin = D2;
volatile long pulse;
unsigned long lastTime;

float volume;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(sensorPin), increase, RISING);
}

void loop() {
  volume = 2.663 * pulse;
  Serial.print(pulse);
  Serial.println(" mL");
  delay(200);
}

ICACHE_RAM_ATTR void increase() {
  pulse++;
}