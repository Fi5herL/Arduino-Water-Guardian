const int analogInPin = A0;
const int relayPin = 7;
const int ledPin = 13;
const int testPin = 2;

const int windowSize = 50;
const int threshold = 80;
const int triggerCount = 40;
const unsigned long debounceTime = 1000;  // 需持續 3 秒才切換

int zeroCount = 0;
bool relayState = false;
bool lastDetected = false;
unsigned long stateStartTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(testPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  digitalWrite(testPin, HIGH);
}

void loop() {
  zeroCount = 0;

  for (int i = 0; i < windowSize; i++) {
    int val = analogRead(analogInPin);
    if (val < threshold) {
      zeroCount++;
    }
    delay(2);
  }

  bool waterDetected = (zeroCount >= triggerCount);

  // 狀態改變時，重新計時
  if (waterDetected != lastDetected) {
    stateStartTime = millis();
    lastDetected = waterDetected;
  }

  // 持續同樣狀態超過 3 秒才切換
  if (millis() - stateStartTime >= debounceTime) {
    relayState = waterDetected;
  }

  digitalWrite(relayPin, relayState ? HIGH : LOW);
  digitalWrite(ledPin, relayState ? HIGH : LOW);

  Serial.print("0V次數 = ");
  Serial.print(zeroCount);
  Serial.print("\t detected = ");
  Serial.print(waterDetected ? "YES" : "---");
  Serial.print("\t relay = ");
  Serial.println(relayState ? "ON" : "OFF");
}