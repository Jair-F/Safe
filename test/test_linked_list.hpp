#pragma once
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
    for (unsigned short list_size = 0; list_size < max_list_size; ++list_size)
    {
        for (unsigned short i = 0; i < list_size; ++i)
        {
            list.push_back(list_default_testing_value);
        }
        TEST_ASSERT_EQUAL(list_size, list.size());
        list.clear();
        TEST_ASSERT_EQUAL(0, list.size());
        TEST_ASSERT_TRUE(list.begin() == list.end());
    }
}

void linked_list_pop_back()
{
    SinglyLinkedList<TESTING_TYPE> list;
    for (unsigned short list_size = 0; list_size < max_list_size; ++list_size)
    {
        for (unsigned short i = 0; i < list_size; ++i)
        {
            if (random(0, 100) % 2) // just for pushing_back random data
            {
                list.push_back(list_default_testing_value);
            }
            else
            {
                list.push_back(list_2default_testing_value);
            }
        }
        for (unsigned short i = 0; i < list_size; ++i)
        {
            auto removed_value = list[list.size() - 1];
            unsigned short prev_list_size = list.size();
            auto tmp = list.pop_back();
            TEST_ASSERT_TRUE(tmp == removed_value);
            TEST_ASSERT_EQUAL(prev_list_size - 1, list.size());
        }
    }
}

void linked_list_pop_front()
{
    SinglyLinkedList<TESTING_TYPE> list;
    for (unsigned short list_size = 0; list_size < max_list_size; ++list_size)
    {
        for (unsigned short i = 0; i < list_size; ++i) // initializing the list with data
        {
            if (random(0, 100) % 2) // just for pushing_back random data
            {
                list.push_back(list_default_testing_value);
            }
            else
            {
                list.push_back(list_2default_testing_value);
            }
        }
        for (unsigned short i = 0; i < list_size; ++i)
        {
            auto removed_value = list[0];
            unsigned short prev_list_size = list.size();
            auto tmp = list.pop_front();
            TEST_ASSERT_TRUE(tmp == removed_value);
            TEST_ASSERT_EQUAL(prev_list_size - 1, list.size());
        }
    }
}

void linked_list_erase()
{
    SinglyLinkedList<TESTING_TYPE> list;
    for (unsigned short list_size = 0; list_size < max_list_size; ++list_size)
    {
        for (unsigned short i = 0; i < list_size; ++i) // initializing the list with data
        {
            if (random(0, 100) % 2) // just for pushing_back random data
            {
                list.push_back(list_default_testing_value);
            }
            else
            {
                list.push_back(list_2default_testing_value);
            }
        }
        for (unsigned short i = 0; i < list_size; ++i)
        {
            unsigned short removing_pos = random(0, list.size() - 1);
            auto removed_value = list[removing_pos];
            unsigned short prev_list_size = list.size();

            auto tmp = list.erase(removing_pos);

            TEST_ASSERT_TRUE(tmp == removed_value);
            TEST_ASSERT_EQUAL(prev_list_size - 1, list.size());
        }
        TEST_ASSERT_TRUE(list.begin() == list.end());
    }
}

void linked_list_length()
{
    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        SinglyLinkedList<TESTING_TYPE> list(i, list_default_testing_value);
        TEST_ASSERT_EQUAL(i, list.size());
    }
    SinglyLinkedList<TESTING_TYPE> list;
    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        list.push_back(list_default_testing_value);
        TEST_ASSERT_EQUAL(i + 1, list.length());
    }
    list.clear();
    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        list.push_front(list_default_testing_value);
        TEST_ASSERT_EQUAL(i + 1, list.length());
    }
    list.clear();
    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        list.insert(random(0, i), list_default_testing_value);
        TEST_ASSERT_EQUAL(i + 1, list.length());
    }
}

void linked_list_change_values()
{
    SinglyLinkedList<TESTING_TYPE> list(max_list_size, list_default_testing_value);
    for (unsigned short i = 0; i < max_list_size; ++i)
    {
        TESTING_TYPE tmp = random(list_default_testing_value, list_2default_testing_value);
        list[i] = tmp;
        TEST_ASSERT_TRUE(list[i] == tmp);
    }
}
