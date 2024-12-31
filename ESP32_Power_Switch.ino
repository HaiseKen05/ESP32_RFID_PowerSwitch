#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions for ESP32
#define RST_PIN         14
#define SS_PIN          5
#define RGB_RED_PIN     27
#define RGB_GREEN_PIN   26
#define RGB_BLUE_PIN    33
#define LOCK_PIN        25
#define BUZZER_PIN      32

// Constants for delays
const int LED_BLINK_DELAY = 250;
const int LOCK_ACTIVE_DELAY = 500;

// RFID and System Variables
MFRC522 mfrc522(SS_PIN, RST_PIN);  // MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 chars x 4 lines

String read_rfid;
const String valid_rfid[] = {"e45bd4", "263e6245"};  // Array of valid UIDs, edit this with your own RFID Card and Keyfob for it to work

void setup() {
    Serial.begin(115200);
    SPI.begin(18, 19, 23, 5);  // SCK, MISO, MOSI, SS
    mfrc522.PCD_Init();

    pinMode(RGB_RED_PIN, OUTPUT);
    pinMode(RGB_GREEN_PIN, OUTPUT);
    pinMode(RGB_BLUE_PIN, OUTPUT);
    pinMode(LOCK_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Initialize LCD
    lcd.init();
    lcd.backlight();

    // Set initial states
    setRGBColor(255, 255, 0); // Yellow LED for standby mode
    digitalWrite(LOCK_PIN, LOW);

    // Startup: Blue LED blinks 3 times, then stays on
    blueBlinkBuzzThreeTimes();
    setRGBColor(255, 255, 0); // Keep the blue LED on after the blink sequence
    lcd.setCursor(0, 0);
    lcd.print("System Ready...");
}

void dumpByteArray(byte *buffer, byte bufferSize) {
    read_rfid = "";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid += String(buffer[i], HEX);
    }
}

void blinkRGBBuzzer(int red, int green, int blue, int times, int duration = LED_BLINK_DELAY) {
    for (int i = 0; i < times; i++) {
        setRGBColor(red, green, blue);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(duration);
        setRGBColor(0, 0, 0);
        digitalWrite(BUZZER_PIN, LOW);
        delay(duration);
    }
}

void blueBlinkBuzzThreeTimes() {
    blinkRGBBuzzer(200, 200, 0, 3, 150); // Blue blink
}

void greenBlinkBuzzTwice() {
    blinkRGBBuzzer(0, 255, 0, 2, 100); // Green blink
}

void redBlinkBuzzOnce() {
    blinkRGBBuzzer(255, 0, 0, 1, 750); // Red blink
}

void openLock() {
    setRGBColor(0, 255, 0); // Green for success
    greenBlinkBuzzTwice();  // Green LED blink

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome Admin");
    delay(1000);

    digitalWrite(LOCK_PIN, HIGH); // Activate lock
    delay(LOCK_ACTIVE_DELAY);
    digitalWrite(LOCK_PIN, LOW);  // Deactivate lock

    setRGBColor(200, 200, 0); // Blue for standby
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready...");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
}

void accessDenied() {
    setRGBColor(255, 0, 0); // Red for access denied
    redBlinkBuzzOnce();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Try Again");
    delay(500);

    setRGBColor(200, 200, 0); // Blue for standby
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready...");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
}

bool isValidRFID() {
    for (String uid : valid_rfid) {
        if (read_rfid == uid) {
            return true;
        }
    }
    return false;
}

void setRGBColor(int red, int green, int blue) {
    analogWrite(RGB_RED_PIN, red);
    analogWrite(RGB_GREEN_PIN, green);
    analogWrite(RGB_BLUE_PIN, blue);
}

void loop() {
    if (!mfrc522.PICC_IsNewCardPresent()) return;
    if (!mfrc522.PICC_ReadCardSerial()) return;

    dumpByteArray(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println(read_rfid);

    if (isValidRFID()) {
        openLock();
    } else {
        accessDenied();
    }

    delay(700);
}
