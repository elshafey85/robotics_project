#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

int led = 13;
int buzzer = 4;
int sensor_pin = A0;      // Soil moisture sensor
int Analogpin = A1;       // Water level sensor
int relayPin = 6;

#define DHTPIN 5
#define DHTTYPE DHT11
DHT tempSensor = DHT(DHTPIN, DHTTYPE);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

float temp = 0.0;
float humidity = 0.0;
int output_value = 0;
int sensor_value = 0;

unsigned long previousDHT = 0;
unsigned long lastDisplay = 0;
unsigned long lastBuzzerTime = 0;
unsigned long buzzerStartTime = 0;
unsigned long lastSerialTime = 0;

bool isBuzzerBeeping = false;

// ====================== CONSTANTS ======================
const unsigned long DHT_INTERVAL = 10000;     
const unsigned long DISPLAY_INTERVAL = 500;  
const unsigned long BUZZER_INTERVAL = 5000;   
const unsigned long BUZZER_DURATION = 700;    
const unsigned long SERIAL_INTERVAL = 2000;   

void setup() {
  Serial.begin(9600);
  tempSensor.begin();
  lcd.begin(16, 2);
  
  pinMode(sensor_pin, INPUT);
  pinMode(Analogpin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  
  // Initial state
  digitalWrite(relayPin, HIGH);  // Motor OFF
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
  
  lcd.clear();
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1500);  // Only once at startup
}

void loop() {
  unsigned long currentMillis = millis();  // ONE millis() call for everything

  if (currentMillis - previousDHT >= DHT_INTERVAL) {
    previousDHT = currentMillis;
    temp = tempSensor.readTemperature(true) - 8;   // Your original calibration
    humidity = tempSensor.readHumidity();
  }

  // ==================== 2. READ OTHER SENSORS EVERY LOOP (FAST) ====================
  output_value = analogRead(sensor_pin);
  output_value = map(output_value, 550, 10, 0, 100); 
  // Safety: prevent negative or >100 values
  if (output_value < 0) output_value = 0;
  if (output_value > 100) output_value = 100;

  sensor_value = analogRead(Analogpin);
  if (sensor_value < 0 ) sensor_value = 0;
  // ==================== 3. CONTROL MOTOR & LED (NON-BLOCKING) ====================
  bool motorOn = (output_value < 30);
  
  if (motorOn) {
    digitalWrite(relayPin, LOW);   // Motor ON
    digitalWrite(led, HIGH);       // LED stays ON while motor is running (clear status)
  } else {
    digitalWrite(relayPin, HIGH);  // Motor OFF
    digitalWrite(led, LOW);
  }

  if (sensor_value > 1015) {
    if (!isBuzzerBeeping && (currentMillis - lastBuzzerTime >= BUZZER_INTERVAL)) {
      digitalWrite(buzzer, HIGH);
      isBuzzerBeeping = true;
      buzzerStartTime = currentMillis;
      lastBuzzerTime = currentMillis;
    }
  } else {
    // Water level OK → force buzzer OFF
    if (isBuzzerBeeping) {
      digitalWrite(buzzer, LOW);
      isBuzzerBeeping = false;
    }
  }
  
  // Turn buzzer OFF after duration (non-blocking)
  if (isBuzzerBeeping && (currentMillis - buzzerStartTime >= BUZZER_DURATION)) {
    digitalWrite(buzzer, LOW);
    isBuzzerBeeping = false;
  }

  // ==================== 5. UPDATE LCD EVERY 500ms (NON-BLOCKING) ====================
  // Temp & humidity stay the same for full 10 seconds (as you wanted)
  // Moisture & motor status update live without any delay
  if (currentMillis - lastDisplay >= DISPLAY_INTERVAL) {
    lastDisplay = currentMillis;
    
    // Line 0: Temp + Humidity (held for 10 seconds)
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp, 1);           // 1 decimal place
    lcd.print((char)223);
    lcd.print("C H:");
    lcd.print(humidity, 0);
    lcd.print("% ");
    
    // Line 1: Moisture + Pump status (updates live)
    lcd.setCursor(0, 1);
    lcd.print("M:");
    lcd.print(output_value);
    lcd.print("% Pump:");
    lcd.print(motorOn ? "ON " : "OFF");
    lcd.print("   ");   // extra spaces to clear any old characters
  }

  // ==================== 6. SERIAL OUTPUT (clean, not spamming) ====================
  if (currentMillis - lastSerialTime >= SERIAL_INTERVAL) {
    lastSerialTime = currentMillis;
    Serial.println("_____________________");
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" C");
    Serial.print("Moisture: ");
    Serial.print(output_value);
    Serial.println("%");
    Serial.print("Water sensor: ");
    Serial.println(sensor_value);
    Serial.print("Motor: ");
    Serial.println(motorOn ? "ON" : "OFF");
  }
}