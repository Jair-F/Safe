

## Library's
- Adafruit Keypad       - `tested version: xx.xx`
- Adafruit Fingerprint  - `tested version: xx.xx`
- MFRC522               - `tested version: xx.xx`
- ArduinoJson           - `tested version: xx.xx`
- RtcDS1302             - `tested version: xx.xx`


## Installing required Software
- To upload the code to the Arduino install [Visual Studio Code](https://code.visualstudio.com/download). Inside Visual Studio Code install the Extention [PlatformIO](https://docs.platformio.org/en/stable/integration/ide/vscode.html#installation)
- Open the project through the [PlatformIO Homescreen](https://docs.platformio.org/en/stable/integration/ide/vscode.html#quick-start). PlatformIO will download all required librarys to upload the code which are defined in the [platformio.ini file](https://docs.platformio.org/en/latest/projectconf/index.html)
- Upload the code by clicking the upload/arrow button in the [Toolbar](https://docs.platformio.org/en/stable/integration/ide/vscode.html#platformio-toolbar)



## Wiring for Arduino Mega with this Code
<!-- Wiring not working for Arduino Mega: https://arduino.stackexchange.com/questions/60525/did-not-find-fingerprint-sensor-arduino-mega-2560-adafruit-fingerprint-sensor -->
- RFID:
    - SDA:  53 (SS - Slave-Select - needs only LOW and HIGH)
    - SCK:  52
    - MOSI: 51
    - MISO: 50
    - RST:  49
    - VC:   3,3V
    - GND:  GND
- Fingerprint:
    - RX:  18(TX1)
    - TX:  19(RX1)
    - VC:  A0(Controled power pin for fingerprint)
    - GND: GND
- RTC-Module(DS1302):
    - VCC:  5V
    - GND:  GND
    - CLK:  A1
    - DATA: A2
    - RST:  A3
- Keypad:
    - R1: A8
    - R2: A9
    - R3: A10
    - R4: A11
    - C1: A12
    - C2: A13
    - C3: A14

