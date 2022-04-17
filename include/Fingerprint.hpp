#pragma once

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "Unlock_Object.hpp"
#include "Lock.hpp"

/*
Fingerprint:
    Wiring: https://learn.adafruit.com/adafruit-optical-fingerprint-sensor/enrolling-new-users-with-windows
        tutorial: https://learn.adafruit.com/adafruit-optical-fingerprint-sensor
        wiring not working on arduino mega: https://arduino.stackexchange.com/questions/60525/did-not-find-fingerprint-sensor-arduino-mega-2560-adafruit-fingerprint-sensor
    Library class reference: https://adafruit.github.io/Adafruit-Fingerprint-Sensor-Library/html/index.html
*/

namespace Fingerprint
{
    String error_code_message(uint8_t error_code);
    class Fingerprint;
    // the default baudrate for our communication between the arduino and the fingerprint-sensor
    constexpr uint32_t default_baudrate = 57600;

    class Fingerprint : public Adafruit_Fingerprint, public Unlock_Object
    {
    public:
        Fingerprint(SoftwareSerial *my_serial, Lock::unlock_token *utoken) : Adafruit_Fingerprint(my_serial), Unlock_Object(utoken) {}
        Fingerprint(HardwareSerial *my_serial, Lock::unlock_token *utoken) : Adafruit_Fingerprint(my_serial), Unlock_Object(utoken) {}
        Fingerprint(Stream *my_serial, Lock::unlock_token *utoken) : Adafruit_Fingerprint(my_serial), Unlock_Object(utoken) {}
        virtual ~Fingerprint() {}

        void begin();

        virtual void loop() override;

        /*
            @param id The id to store the fingerprint in the database(1-127)
            @return error_code from this-> - message can be shown with Fingerprint::error_code_message
        */
        uint8_t add_finger(uint16_t id);

        /*
            Checks if a fingerprint is stored on this id in the Database
            @param id id in the database(1-127)
            @return true if a fingerprint is saved on this id otherwise false
        */
        bool check_id_used(uint16_t id);

    private:
    };

    String error_code_message(uint8_t error_code);

}

// ------------- Implementation -------------

void Fingerprint::Fingerprint::begin()
{
    Adafruit_Fingerprint::begin(default_baudrate);
    if (this->verifyPassword())
    {
        Serial.println("Found fingerprint");
    }
    else
    {
        Serial.println("Didnt found fingerprint");
        while (true)
            delay(1000);
        // exit(-1);
    }
}

void Fingerprint::Fingerprint::loop()
{
    if (this->is_enabled())
    {
        uint8_t err_code = this->getImage();
        if (err_code != FINGERPRINT_OK) // if a error occourd on scanning
        {
            return;
        }

        /*
        slot in the fingerprint_class for storing the image and for
        comparision afterwards with the fingerprint database
        */
        constexpr unsigned short finger_template_slot = 1;

        // convert the image to a feature template to compare it afterwards with fingers in the database
        err_code = this->image2Tz(finger_template_slot);
        if (err_code) // error_handling - maybe output on display...
        {
            Serial.println("Error");
            return;
        }

        err_code = this->fingerSearch(finger_template_slot);
        if (err_code == FINGERPRINT_OK) // found a match
        {
            this->utoken->request_unlock();
        }
        else if (err_code == FINGERPRINT_NOMATCH)
        {
            // maybe set a timer for locking the lock?
        }
        else
        {
            // Serial.println("prints didnt matched");
            // Serial.println(err_code, HEX);
            this->utoken->report_unauthorized_unlock_try();
            // print the err_message on the display
        }
    }
}

bool Fingerprint::Fingerprint::check_id_used(uint16_t id)
{
    // source: https://forum.arduino.cc/t/adafruit-fingerprint-sensor-library-detecting-empty-id/535181
    uint8_t err_code = this->loadModel(id);
    Serial.println(error_code_message(err_code));
    if (err_code != FINGERPRINT_OK)
        return false;
    else
        return true;
}

uint8_t Fingerprint::Fingerprint::add_finger(uint16_t id)
{
    uint8_t err_code = FINGERPRINT_NOFINGER;
    Serial.println("place finger");
    while (err_code != FINGERPRINT_OK)
    {
        err_code = this->getImage();
    }
    Serial.println(error_code_message(err_code));
    err_code = this->image2Tz(1); // param slot for the image(exist 1 and 2) second is for verification...
    Serial.println(error_code_message(err_code));

    // waiting until user removes the finger
    while (err_code != FINGERPRINT_NOFINGER)
    {
        err_code = this->getImage();
    }

    Serial.println("place same finger again");
    do
    {
        err_code = this->getImage();
    } while (err_code != FINGERPRINT_OK);
    Serial.println(error_code_message(err_code));
    err_code = this->image2Tz(2); // second slot for verification
    Serial.println(error_code_message(err_code));

    Serial.println("Creating model of finger...");
    err_code = this->createModel(); // creating model of the two feature templates which are created from the images
    Serial.println(error_code_message(err_code));

    Serial.println("Storing model in Database...");
    err_code = this->storeModel(id);
    Serial.println(error_code_message(err_code));

    return err_code;
}

String Fingerprint::error_code_message(uint8_t error_code)
{
    /*
        https://adafruit.github.io/Adafruit-Fingerprint-Sensor-Library/html/_adafruit___fingerprint_8h.html
    */
    String ret;
    switch (error_code)
    {
    case FINGERPRINT_OK:
        // do nothing
        break;
    case FINGERPRINT_PACKETRECIEVEERR:
        ret = "Error receiving data package";
        break;
    case FINGERPRINT_NOFINGER:
        ret = "No finger on the sensor";
        break;
    case FINGERPRINT_IMAGEFAIL:
        ret = "Failed to enroll the finger";
        break;
    case FINGERPRINT_IMAGEMESS:
        ret = "Image to messy";
        break;
    case FINGERPRINT_FEATUREFAIL:
        ret = "Image to small";
        break;
    case FINGERPRINT_NOMATCH:
        ret = "Finger doesn't match";
        break;
    case FINGERPRINT_NOTFOUND:
        ret = "Failed to find matching finger";
        break;
    case FINGERPRINT_ENROLLMISMATCH:
        ret = "failed to combine character/finger files";
        break;
    case FINGERPRINT_BADLOCATION:
        ret = "Addressed PageID is beyond the finger library";
        break;
    case FINGERPRINT_DBRANGEFAIL:
        ret = "Error reading template from library or invalid template";
        break;
    case FINGERPRINT_UPLOADFEATUREFAIL:
        ret = "error uploading template";
        break;
    case FINGERPRINT_PACKETRESPONSEFAIL:
        ret = "Module failed to receive the following data packages";
        break;
    case FINGERPRINT_UPLOADFAIL:
        ret = "error uploading the image";
        break;
    case FINGERPRINT_DELETEFAIL:
        ret = "failed to delete template";
        break;
    case FINGERPRINT_DBCLEARFAIL:
        ret = "failed to clear finger library";
        break;
    case FINGERPRINT_PASSFAIL:
        ret = "find whether the fingerprint passed or failed";
        break;
    case FINGERPRINT_INVALIDIMAGE:
        ret = "failed to generate image because lac of valid primary image";
        break;
    case FINGERPRINT_FLASHERR:
        ret = "error writing flash";
        break;
    case FINGERPRINT_INVALIDREG:
        ret = "invalid register number";
        break;
    case FINGERPRINT_ADDRCODE:
        ret = "Address code";
        break;
    case FINGERPRINT_PASSVERIFY:
        ret = "verify the fingerprint passed";
        break;
    case FINGERPRINT_STARTCODE:
        ret = "Fixed falue of EF01H; High byte transferred first";
        break;
    case FINGERPRINT_TIMEOUT:
        ret = "Timeout was reached";
        break;
    case FINGERPRINT_BADPACKET:
        ret = "bad packet was sent";
        break;
    default:
        ret = "Unknow error: " + error_code;
        break;
    }
    return ret;
}
