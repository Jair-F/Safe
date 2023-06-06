#pragma once

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "GlobalConstants.hpp"
#include "Lock/Unlock_Object.hpp"
#include "Lock/Lock.hpp"

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

        /**
         * @brief predefined LED-blink and light cycles/modes to put the
         * fingerprint LED in with the function led_control()
         */
        enum class led_modes
        {
            LED_OFF,
            LED_WAITING_FOR_FINGER,
            LED_READING_FINGER,
            LED_AUTHORIZED_FINGER,
            LED_UNAUTHORIZED_FINGER
        };

        /**
         * @brief sets the LED in the according LED mode.
         * @note the LED stays in the set mode until this function is called
         * again (internally by a different Fingerprint function or from outside)
         */
        void led_control(led_modes _led_mode);

        inline const led_modes get_led_mode() const;

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
         * @brief reads the finger on the sensor and stores it in a slot for storing later
         * @note valid slot id's are 1,2
         * @param _slot_id id of the slot to store the fingerprint in - 1 or 2
         * @return true if a finger was read, false if not
         */
        bool read_finger(uint8_t _slot_id);

        /**
         * @brief stores the fingerprint image of the two slots to the given id
         * @note id is between 1-127 including
         * @param id id to store the fingerprint model of the two slots in.
         * @return true on success, false on fail
         */
        bool store_finger_model(uint8_t id);

        /**
         * @brief deletes a fingerprint model from the database
         * @return true if the delete was successful
         * @note the status variable error_code will be set to get more detailed information about an error.
         */
        bool delete_finger(uint16_t id);

        /**
         * @brief clears the whole database of stored fingerpints
         * @return true on success
         */
        bool clear_database();

        /**
         * Checks if a fingerprint is stored on this id in the Database
         * @param id id in the database(1-127)
         * @return true if a fingerprint is saved on this id otherwise false
         */
        bool check_id_used(uint16_t id);

        /**
         * @brief returns the matching id to the read fingprint stored in slot 1
         * @return true if a match was found, false if not
         * @param _store_id_here id to matching fingerprint will be stored here, if no matching
         *                       fingerprint was found(@return==false) it will be not changed.
         */
        bool get_id(uint8_t &_store_id_here);

        /**
         * will be called in loop by the lock. it reads the fingerprint for fingers and returns matching
         * status codes, which triggers the lock to unlock/lock.
         *
         * @note will be also used for authentication without unlocking the physical lock
         *
         * @return Unlock_Object::unlock_authentication_reports
         */
        unlock_authentication_reports read() override;

        bool authorized_unob_database_empty() override;

        /**
         * @brief error code for the client which is set by serval operations to get more
         * information with the func error_code_message(String) about the error
         */
        uint8_t error_code;

        const uint16_t _get_fingerprint_capacity() const { return this->fingerprint.capacity; }

    protected:
    private:
        Adafruit_Fingerprint fingerprint;
        led_modes led_mode;
    };

    String error_code_message(uint8_t error_code);

}
