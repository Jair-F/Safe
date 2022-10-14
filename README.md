

## Library's
- Adafruit Keypad       - `tested version: xx.xx`
- Adafruit Fingerprint  - `tested version: xx.xx`
- MFRC522               - `tested version: xx.xx`
- ArduinoJson           - `tested version: xx.xx`
- RtcDS1302             - `tested version: xx.xx`


## Installing required Software
- To upload the code to the Arduino install [Visual Studio Code](https://code.visualstudio.com/download). Inside Visual Studio Code install the Extention [PlatformIO](https://docs.platformio.org/en/stable/integration/ide/vscode.html#installation)
- Open the project through the [PlatformIO Homescreen](https://docs.platformio.org/en/stable/integration/ide/vscode.html#quick-start). PlatformIO will download all required librarys to upload the code which are defined in the [platformio.ini file](https://docs.platformio.org/en/latest/projectconf/index.html) automatically at the first upload
- Upload the code by clicking the upload/arrow button in the [Toolbar](https://docs.platformio.org/en/stable/integration/ide/vscode.html#platformio-toolbar)



## Wiring for Arduino Mega with this Code
<!-- Wiring not working for Arduino Mega: https://arduino.stackexchange.com/questions/60525/did-not-find-fingerprint-sensor-arduino-mega-2560-adafruit-fingerprint-sensor -->
- RFID:
    - SDA:  (SS)
    - SCK: SPI header SCK
    - MOSI: SPI header MOSI
    - MISO: SPI header MISO
    - IRQ:
    - RST: SPI header RST
    - VC:   3,3V
    - GND:  GND
- Fingerprint:
    - RX:  (TX3)
    - TX:  (RX3)
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
- SD-Card Reader:
    - VCC: 3.3V
    - GND: GND
    - MOSI: DUE SPI header MOSI
    - MISO: DUE SPI header MISO
    - SCK: DUE SPI header SCK

## Debugging
The Arduino Mega has an onboard debugging-section. The communication between the Computer and the Arduino Board is via the UART(Serial) interface. We can specify the Hardware-UART which the sketch should use in the `avr8-stub.h` header with the Makro `AVR8_UART_NUMBER` - 0-3. For communication with Serial-Monitor and debugging we need an [UART-USB dongle](https://www.aliexpress.com/item/32962455401.html?_randl_currency=ILS&_randl_shipto=IL)