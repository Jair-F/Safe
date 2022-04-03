

## Library's
- Adafruit Keypad
- Adafruit Fingerprint
- MFRC522
- ArduinoJson


## Debugging the Arduino Code
install arduino ide
install arduino extention

install openocd

press F5 and select "Arduino envirement"


## Wiring for Arduino Mega with this Code
<!-- Wiring not working for Arduino Mega: https://arduino.stackexchange.com/questions/60525/did-not-find-fingerprint-sensor-arduino-mega-2560-adafruit-fingerprint-sensor -->
- RFID:
    - SDA:  53
    - SCK:  52
    - MOSI: 51
    - MISO: 50
    - RST:  49
    - VC:   3,3V
    - GND:  GND
- Fingerprint:
    - RX:  18(TX1)
    - TX:  19(RX1)
    - VC:  5V
    - GND: GND
- Display: