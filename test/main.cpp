#include <Arduino.h>
#include <unity.h>
#include <LinkedList.hpp>

using TESTING_TYPE = int;
constexpr TESTING_TYPE list_default_testing_value = 100;
constexpr TESTING_TYPE list_2default_testing_value = 150;
constexpr unsigned short max_list_size = 100;

void linked_list_empty()
{
    SinglyLinkedList<TESTING_TYPE> list;
    TEST_ASSERT_TRUE(list.begin() == list.end());
    TEST_ASSERT_EQUAL(list.size(), 0);
}
void linked_list_standard_initializer()
{
    for (unsigned short num_of_elements = 0; num_of_elements < max_list_size; ++num_of_elements)
    {
        SinglyLinkedList<TESTING_TYPE> list(num_of_elements, list_default_testing_value);
        TEST_ASSERT_EQUAL(num_of_elements, list.size());
        for (unsigned short i = 0; i < num_of_elements; ++i)
        {
            TEST_ASSERT_EQUAL(list_default_testing_value, list[i]);
        }
    }
}

void linked_list_push_back()
{
    SinglyLinkedList<TESTING_TYPE> list;
    unsigned short prev_list_size = list.size();

    list.push_back(list_default_testing_value);
    TEST_ASSERT_EQUAL(list_default_testing_value, list[0]);
    TEST_ASSERT_EQUAL(prev_list_size + 1, list.size());

    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        prev_list_size = list.size();
        list.push_back(list_default_testing_value);
        TEST_ASSERT_EQUAL(prev_list_size + 1, list.size());
        TEST_ASSERT_EQUAL(list_default_testing_value, list[list.size() - 1]);
    }
}

void linked_list_push_front()
{
    SinglyLinkedList<TESTING_TYPE> list;
    unsigned short prev_list_size = list.size();

    list.push_front(list_default_testing_value);
    TEST_ASSERT_EQUAL(prev_list_size + 1, list.size());
    TEST_ASSERT_EQUAL(list_default_testing_value, list[0]);

    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        prev_list_size = list.size();
        list.push_back(list_default_testing_value);
        TEST_ASSERT_EQUAL(prev_list_size + 1, list.size());
        TEST_ASSERT_EQUAL(list_default_testing_value, list[0]);
    }
}

void linked_list_insert()
{
    SinglyLinkedList<TESTING_TYPE> list;
    unsigned short prev_list_size = list.size();

    for (unsigned short list_size = 0; list_size < max_list_size; ++list_size)
    {
        unsigned short insert_pos = random(0, list_size);
        prev_list_size = list.size();

        list.insert(insert_pos, list_default_testing_value);

        TEST_ASSERT_EQUAL(prev_list_size + 1, list.size());
        TEST_ASSERT_EQUAL(list_default_testing_value, list[insert_pos]);
    }
}

void linked_list_clear()
{
    SinglyLinkedList<TESTING_TYPE> list;

    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        list.push_back(list_default_testing_value);
    }
    TEST_ASSERT_EQUAL(max_list_size, list.size());
    list.clear();
    TEST_ASSERT_EQUAL(0, list.size());
    TEST_ASSERT_TRUE(list.begin() == list.end());
}

void linked_list_pop_back()
{
    SinglyLinkedList<TESTING_TYPE> list;
    unsigned short prev_list_size;
    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        if (random(true, false))
        {
            list.push_back(list_default_testing_value);
        }
        else
        {
            list.push_back(list_2default_testing_value);
        }
    }
    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        auto removed_value = list[list.size() - 1];
        prev_list_size = list.size();
        auto tmp = list.pop_back();
        TEST_ASSERT_TRUE(tmp == removed_value);
        TEST_ASSERT_EQUAL(prev_list_size - 1, list.size());
    }
}

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
    UNITY_END();
}

void loop()
{
    delay(1000);
}