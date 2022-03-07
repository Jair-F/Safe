#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>

#include "include/Helper.hpp"
#include "include/Fingerprint.hpp"

#define MFRC522_RST_PIN 49  // RST_PIN
#define MFRC522_SS_PIN  53  // SDA_PIN

//MFRC522 rfid(MFRC522_SS_PIN, MFRC522_RST_PIN);

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
    // For UNO and others without hardware serial, we must use software serial...
    // pin #2 is IN from sensor (GREEN wire)
    // pin #3 is OUT from arduino  (WHITE wire)
    // Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);
#else
    // On Leonardo/M0/etc, others with hardware serial, use hardware serial!
    // #0 is green wire, #1 is white
#define mySerial Serial1
#endif


//                      Serial-connection   default-password for database
Fingerprint::Fingerprint finger(&mySerial);

/*
RFID:
    explanation video: https://www.youtube.com/watch?v=gTQmLkwnUxk
*/

String uid;

void setup() {
    Serial.begin(9600);
    //SPI.begin();
    //rfid.PCD_Init();    // init rfid
    //rfid.PCD_DumpVersionToSerial();
    while (!Serial);
    finger.begin();
    finger.emptyDatabase();

    finger.getParameters();
    finger.getTemplateCount();
    Serial.print("Templates: ");
    Serial.println(finger.templateCount);   // how many fingerprints are stored in the sensor database

    Serial.println("Add finger: ");
    while (!Serial.available());
    int id = Serial.parseInt();
    Serial.println(id);
    finger.add_finger(id);
}

void loop() {
    Serial.println("ID_Check: ");
    while (!Serial.available());
    int id = Serial.parseInt();
    Serial.read();
    Serial.println(id);
    if (finger.check_id_used(id)) {
        Serial.println("used model id");
    }
    else {
        Serial.println("not used model id");
    }


    return;

    //if (rfid.PICC_IsNewCardPresent()) {     // card is on the reader
    //    uid = String();
    //    Serial.println("new card...");
    //    for (int i = 0; i < rfid.uid.size; ++i) {
    //        //Serial.println(rfid.uid.uidByte[i], BIN);
    //        // saving uid as bin in string
    //        uid += to_bin(rfid.uid.uidByte[i]) + ' ';
    //    }
    //    uid.remove(uid.length() - 1);
    //}
    //rfid.PICC_HaltA();
}