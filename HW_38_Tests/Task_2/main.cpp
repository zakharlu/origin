﻿//подключаем макросы catch2
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <iostream>

struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};


class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};

//юнит-тесты
TEST_CASE("PushBack добавляет элементы в кнец списка", "[List]") {
    List list;

    list.PushBack(1);
    REQUIRE(list.Size() == 1);

    list.PushBack(2);
    REQUIRE(list.Size() == 2);

    list.PushBack(3);
    REQUIRE(list.Size() == 3);
}

TEST_CASE("PopBack удаляет элементы с конца списка", "[List]") {
    List list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    REQUIRE(list.PopBack() == 3);
    REQUIRE(list.Size() == 2);

    REQUIRE(list.PopBack() == 2);
    REQUIRE(list.Size() == 1);

    REQUIRE(list.PopBack() == 1);
    REQUIRE(list.Size() == 0);
    REQUIRE(list.Empty());
}

TEST_CASE("PopFront удаляет первый элемент", "[List]") {
    List list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    REQUIRE(list.PopFront() == 1);
    REQUIRE(list.Size() == 2);

    REQUIRE(list.PopFront() == 2);
    REQUIRE(list.Size() == 1);

    REQUIRE(list.PopFront() == 3);
    REQUIRE(list.Size() == 0);
    REQUIRE(list.Empty());
}

TEST_CASE("PopBack на пустом списке выбрасывает исключение", "[List]") {
    List list;
    REQUIRE_THROWS_AS(list.PopBack(), std::runtime_error);
}

TEST_CASE("PopFront на пустом списке выбрасывает исключение", "[List]") {
    List list;
    REQUIRE_THROWS_AS(list.PopFront(), std::runtime_error);
}

TEST_CASE("Несколько сценариев", "[List]") {
    List list;

    list.PushFront(2);
    list.PushBack(3);
    list.PushFront(1);
    REQUIRE(list.Size() == 3);

    REQUIRE(list.PopFront() == 1);
    REQUIRE(list.PopFront() == 2);
    REQUIRE(list.PopFront() == 3);
    REQUIRE(list.Empty());

    list.PushBack(10);
    list.PushFront(5);
    list.PushBack(15);
    REQUIRE(list.Size() == 3);

    REQUIRE(list.PopBack() == 15);
    REQUIRE(list.PopFront() == 5);
    REQUIRE(list.PopBack() == 10);
    REQUIRE(list.Empty());

    REQUIRE_NOTHROW(list.PushBack(100));
    REQUIRE_NOTHROW(list.PushFront(50));
    REQUIRE(list.Size() == 2);

    REQUIRE(list.PopBack() == 100);
    REQUIRE(list.PopFront() == 50);
    REQUIRE(list.Empty());

    REQUIRE_THROWS_AS(list.PopBack(), std::runtime_error);
    REQUIRE_THROWS_AS(list.PopFront(), std::runtime_error);
}