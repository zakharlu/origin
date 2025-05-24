#pragma once
#include <cstddef> // Для size_t

enum class TransportType {
    GROUND,
    AIR,
    MIXED
};

class Transport {
public:
    virtual ~Transport() = default;
    virtual const char* getName() const = 0;
    virtual TransportType getType() const = 0;
    virtual double calcTimeRide(int distance) const = 0;
};

// Фабричная функция
extern "C" Transport** makeTransports(size_t& totalTransports);
