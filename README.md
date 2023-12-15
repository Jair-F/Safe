
## Used Hardware:
- Arduino Due
- TFT LCD Touch Screen Display 320X240 ILI9341+3.2 inch with shield for mounting
- R503 UART fingerprint sensor
- Estardyn 3*4 Matrix Keyboard
- MFRC522 SPI RFID Module
- DS3231 i2c clock module
- single channel 3.3V-5V relay module

## Library's
- Keypad                - `version: 3.1`
- Adafruit Fingerprint  - `version: 2.0.7`
- MFRC522               - `version: 1.4.10`
- ArduinoJson           - `version: 6.19.4`
- RTClib                - `version: 2.1.1`
- uEEPROMLib            - `version: 1.2.1`
- UTFT                  - `version: 2.83`
- URTouch               - `version: 2.02`


## Installing required Software
- To upload the code to the Arduino install [Visual Studio Code](https://code.visualstudio.com/download). Inside Visual Studio Code install the Extention [PlatformIO](https://docs.platformio.org/en/stable/integration/ide/vscode.html#installation)
- Open the project through the [PlatformIO Homescreen](https://docs.platformio.org/en/stable/integration/ide/vscode.html#quick-start). PlatformIO will download all required librarys to upload the code which are defined in the [platformio.ini file](https://docs.platformio.org/en/latest/projectconf/index.html) automatically at the first upload
- Upload the code by clicking the upload/arrow button in the [Toolbar](https://docs.platformio.org/en/stable/integration/ide/vscode.html#platformio-toolbar)
- Planning of the UI Layout is made with [Figma](https://www.figma.com/)



## Wiring for Arduino Mega with this Code
<!-- Wiring not working for Arduino Mega: https://arduino.stackexchange.com/questions/60525/did-not-find-fingerprint-sensor-arduino-mega-2560-adafruit-fingerprint-sensor -->
- RFID:
    - SDA:  A4
    - SCK:  SPI header SCK
    - MOSI: SPI header MOSI
    - MISO: SPI header MISO
    - IRQ:  -
    - RST:  SPI header RST / A5
    - VC:   3,3V
    - GND:  GND
- Fingerprint:
    - RX:   (TX3)
    - TX:   (RX3)
    - VC:   A0(Controled power pin for fingerprint)
    - GND:  GND
- RTC-Module(DS3231):
    - VCC:  3.3V
    - GND:  GND
    - SDA:  Due SDA pin
    - SCL:  Due SCL pin
- Keypad:
    - R1:   A8
    - R2:   A9
    - R3:   A10
    - R4:   A11
    - C1:   A12
    - C2:   A13
    - C3:   A14
