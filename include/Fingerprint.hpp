#pragma once

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "GlobalConstants.hpp"
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

    class Fingerprint : public Unlock_Object
    {
    public:
        /*Fingerprint(SoftwareSerial *my_serial, Lock *_lock) : Adafruit_Fingerprint(my_serial), Unlock_Object(_lock) { pinMode(FINGERPRINT_POWER_PIN, OUTPUT); }*/
        Fingerprint(HardwareSerial *my_serial, Lock *_lock);

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

        /**
         * @brief checks if a finger is placed on the sensor
         * @return true if a finger is on the sensor
         */
        bool finger_on_sensor();

        /**
         * @brief tries to read a finger from the sensor and adds it to the database on the id
         * @note the status variable error_code will be set to get more detailed information about an error.
         * @param id The id to store the fingerprint in the database(1-127)
         * @return true if a finger was added(a finger was present or a different error)
         */
        bool add_finger(uint16_t id);

        /**
         * @brief deletes a fingerprint model from the database
         * @return true if the delete was successful
         * @note the status variable error_code will be set to get more detailed information about an error.
         */
        bool delete_finger(uint16_t id);

        /**
         * Checks if a fingerprint is stored on this id in the Database
         * @param id id in the database(1-127)
         * @return true if a fingerprint is saved on this id otherwise false
         */
        bool check_id_used(uint16_t id);

        /**
         * will be called in loop by the lock. it reads the fingerprint for fingers and returns matching
         * status codes, which triggers the lock to unlock/lock.
         *
         * @note will be also used for authentication without unlocking the physical lock
         *
         * @return Unlock_Object::unlock_authentication_reports
         */
        unlock_authentication_reports read() override;

        /**
         * @brief error code for the client which is set by serval operations to get more
         * information with the func error_code_message(String) about the error
         */
        uint8_t error_code;

    protected:
    private:
        Adafruit_Fingerprint fingerprint;
    };

    String error_code_message(uint8_t error_code);

}
