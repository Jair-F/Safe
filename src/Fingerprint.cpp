#include "Fingerprint.hpp"
#include "logging/Log.hpp"

extern Log::Log logger;

Fingerprint::Fingerprint::Fingerprint(HardwareSerial *my_serial, Lock *_lock) : Unlock_Object(_lock), fingerprint(my_serial)
{
}

void Fingerprint::Fingerprint::begin()
{
    this->fingerprint.begin(default_baudrate);
    if (this->fingerprint.verifyPassword())
    {
        // logger.log(F("FINGERPRINT: found fingerprint"), Log::log_level::L_INFO);
        Serial.println(F("FINGERPRINT: found fingerprint"));
    }
    else
    {
        // logger.log(F("FINGERPRINT: didnt found fingerprint-sensor"), Log::log_level::L_ERROR);
        Serial.println(F("FINGERPRINT: didnt found fingerprint-sensor"));
        //  exit(-1);
    }

    this->enable();
}

void Fingerprint::Fingerprint::led_control(led_modes _led_mode)
{
    switch (_led_mode)
    {
    case led_modes::LED_OFF:
        this->fingerprint.LEDcontrol(false);
        break;
    case led_modes::LED_WAITING_FOR_FINGER:
        this->fingerprint.LEDcontrol(FINGERPRINT_LED_BREATHING, 255, FINGERPRINT_LED_PURPLE);
        break;
    case led_modes::LED_READING_FINGER:
        this->fingerprint.LEDcontrol(FINGERPRINT_LED_FLASHING, 10, FINGERPRINT_LED_PURPLE);
        break;
    case led_modes::LED_AUTHORIZED_FINGER:
        this->fingerprint.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
        break;
    case led_modes::LED_UNAUTHORIZED_FINGER:
        this->fingerprint.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED);
        break;

    default:
        break;
    }
}

const Fingerprint::Fingerprint::led_modes Fingerprint::Fingerprint::get_led_mode() const
{
    return this->led_mode;
}

void Fingerprint::Fingerprint::enable()
{
    Unlock_Object::enable();
    this->led_control(led_modes::LED_WAITING_FOR_FINGER);
}
void Fingerprint::Fingerprint::disable()
{
    Unlock_Object::disable();
    this->led_control(led_modes::LED_OFF);
}

bool Fingerprint::Fingerprint::finger_on_sensor()
{
    uint8_t err_code = this->fingerprint.getImage();
    return !(err_code == FINGERPRINT_NOFINGER);
}

bool Fingerprint::Fingerprint::delete_finger(uint16_t id)
{
    if (id > 120)
    {
        Serial.println("id is out of range");
        return false;
    }
    this->error_code = this->fingerprint.deleteModel(id);
    return this->error_code == FINGERPRINT_OK;
}

Unlock_Object::unlock_authentication_reports Fingerprint::Fingerprint::read()
{
    if (!this->is_enabled())
    {
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_DISABLED;
    }

    uint8_t err_code = this->fingerprint.getImage();
    if (err_code == FINGERPRINT_OK) // finger is on the sensor - give feedback we read the finger with flashing LED
    {
        this->led_control(led_modes::LED_READING_FINGER);
    }
    else // if a error occourd on scanning
    {
        if (err_code == FINGERPRINT_NOFINGER)
        {
            return Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT;
        }
        else
        {
            DEBUG_PRINTLN("Error reading the finger image")
            return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR;
        }
    }

    /*
    slot in the fingerprint_class for storing the image and for
    comparision afterwards with the fingerprint database
    */
    constexpr unsigned short finger_template_slot = 1;

    // convert the image to a feature template to compare it afterwards with fingers in the database
    err_code = this->fingerprint.image2Tz(finger_template_slot);
    if (err_code == FINGERPRINT_OK)
    {
        err_code = this->fingerprint.fingerSearch(finger_template_slot);
        if (err_code == FINGERPRINT_OK) // found a match
        {
            this->led_control(led_modes::LED_AUTHORIZED_FINGER);
            delay(700);
            this->led_control(led_modes::LED_WAITING_FOR_FINGER);
            return Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT;
        }
        else
        {
            this->led_control(led_modes::LED_UNAUTHORIZED_FINGER);
            delay(700);
            this->led_control(led_modes::LED_WAITING_FOR_FINGER);
            return Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT;
        }
    }
    else if (err_code == FINGERPRINT_PACKETRECIEVEERR) // error_handling - maybe output on display...
    {
        this->fingerprint.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED); // on error red fingerprint LED
        // logger.log(F("FINGERPRINT: error converting fingerprint-image to feature template"), Log::log_level::L_DEBUG);
        Serial.println("package recieve error");
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR;
    }
    else
    {
        this->fingerprint.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED); // on error red fingerprint LED
        // on every other error just return unauthorized
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR;
    }
}

bool Fingerprint::Fingerprint::check_id_used(uint16_t id)
{
    // source: https://forum.arduino.cc/t/adafruit-fingerprint-sensor-library-detecting-empty-id/535181
    uint8_t err_code = this->fingerprint.loadModel(id);
    Serial.println(error_code_message(err_code));
    if (err_code != FINGERPRINT_OK)
        return false;
    else
        return true;
}

bool Fingerprint::Fingerprint::add_finger(uint16_t id)
{
    this->error_code = FINGERPRINT_NOFINGER;
    Serial.println(F("place finger"));

    this->error_code = this->fingerprint.getImage();
    if (this->error_code != FINGERPRINT_OK)
    {
        Serial.println(error_code_message(this->error_code));
        return false;
    }

    this->error_code = this->fingerprint.image2Tz(1); // param slot for the image(exist 1 and 2) second is for verification...
    if (this->error_code != FINGERPRINT_OK)
    {
        Serial.println(error_code_message(this->error_code));
        return false;
    }

    Serial.println(F("place same finger again"));

    this->error_code = this->fingerprint.getImage();
    if (this->error_code != FINGERPRINT_OK)
    {
        Serial.println(error_code_message(this->error_code));
        return false;
    }

    this->error_code = this->fingerprint.image2Tz(2); // second slot for verification
    if (this->error_code != FINGERPRINT_OK)
    {
        Serial.println(error_code_message(this->error_code));
        return false;
    }

    Serial.println(F("Creating model of finger..."));
    this->error_code = this->fingerprint.createModel(); // creating model of the two feature templates which are created from the images
    if (this->error_code != FINGERPRINT_OK)
    {
        if (this->error_code == FINGERPRINT_ENROLLMISMATCH)
        {
            Serial.println(F("scanned fingerprints dont match"));
        }
        String err_msg(F("FINGERPRINT: error creating fingerprint-model. ERROR_CODE: "));
        err_msg += this->error_code;
        return false;
    }

    Serial.println(F("Storing model in Database..."));
    this->error_code = this->fingerprint.storeModel(id);
    if (this->error_code != FINGERPRINT_OK)
    {
        if (this->error_code == FINGERPRINT_ENROLLMISMATCH)
        {
            Serial.println(F("scanned fingerprints dont match"));
        }
        String err_msg(F("FINGERPRINT: error storing fingerprint-model. ERROR_CODE: "));
        err_msg += this->error_code;
        return false;
    }

    return true;
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
