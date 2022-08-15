#pragma once

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "GlobalConstants.hpp"
#include "Unlock_Object.hpp"
#include "Lock.hpp"
#include "logging/Log.hpp"

extern Log::Log logger;

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
        /*Fingerprint(SoftwareSerial *my_serial, Lock *_lock) : Adafruit_Fingerprint(my_serial), Unlock_Object(_lock) { pinMode(FINGERPRINT_POWER_PIN, OUTPUT); }*/
        Fingerprint(HardwareSerial *my_serial, Lock *_lock) : Adafruit_Fingerprint(my_serial), Unlock_Object(_lock)
        { /*pinMode(FINGERPRINT_POWER_PIN, OUTPUT);*/
        }
        /*
        Fingerprint(Stream *my_serial, Lock *_lock) : Adafruit_Fingerprint(my_serial), Unlock_Object(_lock)
        { pinMode(FINGERPRINT_POWER_PIN, OUTPUT);
        }
        */
        virtual ~Fingerprint() {}

        void begin();

        /*
            enable the sensor and wake him up
        */
        virtual void enable() override;
        /*
            disable the sensor and send him to sleep
        */
        virtual void disable() override;

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

        unlock_authentication_reports read() override;

    protected:
        void _initialize_sensor();

    private:
    };

    String error_code_message(uint8_t error_code);

}

// ------------- Implementation -------------

void Fingerprint::Fingerprint::_initialize_sensor()
{
    Adafruit_Fingerprint::begin(default_baudrate);
    if (this->verifyPassword())
    {
        logger.log(F("FINGERPRINT: found fingerprint"), Log::log_level::L_INFO);
        // Serial.println(F("FINGERPRINT: found fingerprint"));
    }
    else
    {
        logger.log(F("FINGERPRINT: didnt found fingerprint-sensor"), Log::log_level::L_ERROR);
        // Serial.println(F("FINGERPRINT: didnt found fingerprint-sensor"));
        //  exit(-1);
    }
}

void Fingerprint::Fingerprint::begin()
{
    this->_initialize_sensor();
    this->enable();
}

void Fingerprint::Fingerprint::enable()
{
    Unlock_Object::enable();
    this->LEDcontrol(FINGERPRINT_LED_BREATHING, 4000, FINGERPRINT_LED_PURPLE);
}
void Fingerprint::Fingerprint::disable()
{
    Unlock_Object::disable();
}

Unlock_Object::unlock_authentication_reports Fingerprint::Fingerprint::read()
{
    if (!this->is_enabled())
    {
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_DISABLED;
    }

    uint8_t err_code = this->getImage();
    if (err_code == FINGERPRINT_OK) // finger is on the sensor - give feedback we read the finger with flashing LED
    {
        this->LEDcontrol(FINGERPRINT_LED_FLASHING, 10, FINGERPRINT_LED_PURPLE);
    }
    else // if a error occourd on scanning
    {
        if (err_code == FINGERPRINT_NOFINGER)
        {
            return Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT;
        }
        else
        {
            return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR;
        }
    }

    /*
    slot in the fingerprint_class for storing the image and for
    comparision afterwards with the fingerprint database
    */
    constexpr unsigned short finger_template_slot = 1;

    // convert the image to a feature template to compare it afterwards with fingers in the database
    err_code = this->image2Tz(finger_template_slot);
    if (err_code == FINGERPRINT_OK)
    {
        err_code = this->fingerSearch(finger_template_slot);
        if (err_code == FINGERPRINT_OK) // found a match
        {
            this->LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
            delay(1000);
            this->LEDcontrol(FINGERPRINT_LED_BREATHING, 4000, FINGERPRINT_LED_PURPLE);
            return Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT;
        }
        else
        {
            this->LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED);
            delay(1000);
            this->LEDcontrol(FINGERPRINT_LED_BREATHING, 4000, FINGERPRINT_LED_PURPLE);
            return Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT;
        }
    }
    else if (err_code == FINGERPRINT_PACKETRECIEVEERR) // error_handling - maybe output on display...
    {
        logger.log(F("FINGERPRINT: error converting fingerprint-image to feature template"), Log::log_level::L_DEBUG);
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR;
    }
    else
    {
        // on every other error just return unauthorized
        return Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT;
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
    Serial.println(F("place finger"));
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

    Serial.println(F("place same finger again"));
    do
    {
        err_code = this->getImage();
    } while (err_code != FINGERPRINT_OK);
    Serial.println(error_code_message(err_code));
    err_code = this->image2Tz(2); // second slot for verification
    Serial.println(error_code_message(err_code));

    Serial.println(F("Creating model of finger..."));
    err_code = this->createModel(); // creating model of the two feature templates which are created from the images
    if (err_code != FINGERPRINT_OK)
    {
        if (err_code == FINGERPRINT_ENROLLMISMATCH)
        {
            Serial.println(F("scanned fingerprints dont match"));
        }
        String err_msg(F("FINGERPRINT: error creating fingerprint-model. ERROR_CODE: "));
        err_msg += err_code;
        logger.log(err_msg, Log::log_level::L_DEBUG);
    }

    Serial.println(F("Storing model in Database..."));
    err_code = this->storeModel(id);
    if (err_code != FINGERPRINT_OK)
    {
        if (err_code == FINGERPRINT_ENROLLMISMATCH)
        {
            Serial.println(F("scanned fingerprints dont match"));
        }
        String err_msg(F("FINGERPRINT: error storing fingerprint-model. ERROR_CODE: "));
        err_msg += err_code;
        logger.log(err_msg, Log::log_level::L_DEBUG);
    }

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
        ret = F("Error receiving data package");
        break;
    case FINGERPRINT_NOFINGER:
        ret = F("No finger on the sensor");
        break;
    case FINGERPRINT_IMAGEFAIL:
        ret = F("Failed to enroll the finger");
        break;
    case FINGERPRINT_IMAGEMESS:
        ret = F("Image to messy");
        break;
    case FINGERPRINT_FEATUREFAIL:
        ret = F("Image to small");
        break;
    case FINGERPRINT_NOMATCH:
        ret = F("Finger doesn't match");
        break;
    case FINGERPRINT_NOTFOUND:
        ret = F("Failed to find matching finger");
        break;
    case FINGERPRINT_ENROLLMISMATCH:
        ret = F("failed to combine character/finger files");
        break;
    case FINGERPRINT_BADLOCATION:
        ret = F("Addressed PageID is beyond the finger library");
        break;
    case FINGERPRINT_DBRANGEFAIL:
        ret = F("Error reading template from library or invalid template");
        break;
    case FINGERPRINT_UPLOADFEATUREFAIL:
        ret = F("error uploading template");
        break;
    case FINGERPRINT_PACKETRESPONSEFAIL:
        ret = F("Module failed to receive the following data packages");
        break;
    case FINGERPRINT_UPLOADFAIL:
        ret = F("error uploading the image");
        break;
    case FINGERPRINT_DELETEFAIL:
        ret = F("failed to delete template");
        break;
    case FINGERPRINT_DBCLEARFAIL:
        ret = F("failed to clear finger library");
        break;
    case FINGERPRINT_PASSFAIL:
        ret = F("find whether the fingerprint passed or failed");
        break;
    case FINGERPRINT_INVALIDIMAGE:
        ret = F("failed to generate image because lac of valid primary image");
        break;
    case FINGERPRINT_FLASHERR:
        ret = F("error writing flash");
        break;
    case FINGERPRINT_INVALIDREG:
        ret = F("invalid register number");
        break;
    case FINGERPRINT_ADDRCODE:
        ret = F("Address code");
        break;
    case FINGERPRINT_PASSVERIFY:
        ret = F("verify the fingerprint passed");
        break;
    case FINGERPRINT_TIMEOUT:
        ret = F("Timeout was reached");
        break;
    case FINGERPRINT_BADPACKET:
        ret = F("bad packet was sent");
        break;
    default:
        ret = F("Unknow error: ");
        ret += error_code;
        break;
    }
    return ret;
}
