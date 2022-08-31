#include <Arduino.h>
#include <unity.h>

double val1 = 10;
double val2 = 5;
typedef int TESTING_TYPE;
constexpr TESTING_TYPE list_default_testing_value = 20;
constexpr TESTING_TYPE list_2default_testing_value = 30;
constexpr unsigned short max_list_size = 100;

#include "test_singly_linked_list.hpp"
#include "test_doubly_linked_list.hpp"

void setup()
{
    randomSeed(analogRead(A0)); // initializing random seed with random read value from analogPin A0
    UNITY_BEGIN();
    RUN_TEST(singly_linked_list_empty);
    RUN_TEST(singly_linked_list_standard_initializer);
    RUN_TEST(singly_linked_list_push_back);
    RUN_TEST(singly_linked_list_push_front);
    RUN_TEST(singly_linked_list_insert);
    RUN_TEST(singly_linked_list_pop_back);
    RUN_TEST(singly_linked_list_clear);
    RUN_TEST(singly_linked_list_pop_front);
    RUN_TEST(singly_linked_list_erase);
    RUN_TEST(singly_linked_list_length);
    RUN_TEST(singly_linked_list_change_values);

    TEST_MESSAGE("Testing Doubly Linked List");

    RUN_TEST(doubly_linked_list_empty);
    RUN_TEST(doubly_linked_list_push_back);
    RUN_TEST(doubly_linked_list_standard_initializer);
    RUN_TEST(doubly_linked_list_push_front);
    RUN_TEST(doubly_linked_list_insert);
    RUN_TEST(doubly_linked_list_pop_back);
    RUN_TEST(doubly_linked_list_clear);
    RUN_TEST(doubly_linked_list_pop_front);
    RUN_TEST(doubly_linked_list_erase);
    RUN_TEST(doubly_linked_list_length);
    RUN_TEST(doubly_linked_list_change_values);
    UNITY_END();
}

void loop()
{
    delay(1000);
}