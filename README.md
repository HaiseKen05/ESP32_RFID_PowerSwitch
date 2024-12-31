# ESP32_RFID_PowerSwitch
an RFID Power Switch made with the help of ChatGPT, If you want to add special spice to your PC, this is one of them.

# Components used: 
- ESP32-WROOM-32
- RFID-RC522
- SRD-05VDC-SL-C Relay Module
- RGB Module
- Buzzer
- LCD I2C 20x4

# Wiring Diagram

- ESP32:
  - 3V3 → Breadboard (+)
  - GND → Breadboard (-)

- RFID-RC522 Module:
  - 3V3 → Breadboard (+)
  - GND → Breadboard (-)
  - RST → GPIO 14 (ESP32)
  - SDA → GPIO 5 (ESP32)
  - SCK → GPIO 18 (ESP32)
  - MOSI → GPIO 23 (ESP32)
  - MISO → GPIO 19 (ESP32)

- Relay Module (SRD-05VDC-SL-C):
  - S → GPIO 25 (ESP32)
  - (+) → Breadboard (+)
  - (-) → Breadboard (-)
  
- RGB Module:
  - Red → GPIO 27 (ESP32)
  - Green → GPIO 26 (ESP32)
  - Blue → GPIO 33 (ESP32)
  - GND → Breadboard (-)

- Buzzer:
  - (+) → GPIO 32 (ESP32)
  - (-) → Breadboard (-)

- I2C LCD (20x4):
  -  SDA → GPIO 21 (ESP32)
  -  SCL → GPIO 22 (ESP32)
  -  VCC → Breadboard (+)
  -  GND → Breadboard (-)
 
# Potential Problems
There are some problems will arise that will cause an error during the upload stage, heres one i encountered:
WARNING: library LiquidCrystal I2C claims to run on avr architecture(s) and may be incompatible with your current board which runs on esp32 architecture(s). // You can leave this out or switch out to a different library such as the HD44780 

c:\Users\Admin\Documents\Arduino\libraries\MFRC522\src\MFRC522Extended.cpp: In member function 'MFRC522::StatusCode MFRC522Extended::TCL_Transceive(TagInfo*, byte*, byte, byte*, byte*)':
c:\Users\Admin\Documents\Arduino\libraries\MFRC522\src\MFRC522Extended.cpp:824:34: error: ordered comparison of pointer with integer zero ('byte*' {aka 'unsigned char*'} and 'int')
  824 |         if (backData && (backLen > 0)) {
      |                          ~~~~~~~~^~~
c:\Users\Admin\Documents\Arduino\libraries\MFRC522\src\MFRC522Extended.cpp:847:42: error: ordered comparison of pointer with integer zero ('byte*' {aka 'unsigned char*'} and 'int')
  847 |                 if (backData && (backLen > 0)) {
      |                                  ~~~~~~~~^~~

exit status 1

Compilation error: exit status 1

- To solve this issue go to the your Arduino Libraries "Arduino\libraries\MFRC522\src\MFRC522Extended.cpp" and open up your code editor (I use VSCode) locate this line of code

" if (backData && (backLen > 0)) { "

Replace it with this:

" if (backData && backLen && (*backLen > 0)) { "

Save the file and verify it first, and if there are no problems then proceed to Upload the code. and enjoy.


