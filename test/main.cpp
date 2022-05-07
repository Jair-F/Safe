#include <Arduino.h>
#include <unity.h>

#include "test_linked_list.hpp"

void setup()
{
    randomSeed(analogRead(A0)); // initializing random seed with random read value from analogPin A0
    UNITY_BEGIN();
    RUN_TEST(linked_list_empty);
    RUN_TEST(linked_list_standard_initializer);
    RUN_TEST(linked_list_push_back);
    RUN_TEST(linked_list_push_front);
    RUN_TEST(linked_list_insert);
    RUN_TEST(linked_list_clear);
    RUN_TEST(linked_list_pop_back);
    RUN_TEST(linked_list_pop_front);
    RUN_TEST(linked_list_erase);
    RUN_TEST(linked_list_length);
    RUN_TEST(linked_list_change_values);
    UNITY_END();
}

void loop()
{
    delay(1000);
}