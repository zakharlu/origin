#include "transport.h"
#include <cmath>
#include <algorithm>

// ===================== Базовые классы =====================
class GroundTransport : public Transport {
protected:
    double speed;
    double drivingTime;
    virtual double getRestDuration(int restCount) const = 0;
public:
    TransportType getType() const override {
        return TransportType::GROUND;
    }

    double calcTimeRide(int distance) const override {
        double totalTime = 0.0;
        int restCount = 0;

        while (distance > 0) {
            double segment = speed * drivingTime;
            if (distance <= segment) {
                totalTime += distance / speed;
                break;
            }
            totalTime += drivingTime + getRestDuration(restCount++);
            distance -= segment;
        }

        return totalTime;
    }
};

class AirTransport : public Transport {
protected:
    double speed;
    virtual double getDistanceReduction(int distance) const = 0;
public:
    TransportType getType() const override {
        return TransportType::AIR;
    }

    double calcTimeRide(int distance) const override {
        double reducedDistance = distance * (1.0 - getDistanceReduction(distance));
        return reducedDistance / speed;
    }
};

// ===================== Конкретные транспорты =====================

// --- Наземные ---
class Camel : public GroundTransport {
public:
    Camel() { speed = 10.0; drivingTime = 30.0; }
    const char* getName() const override { return "Верблюд"; }
    double getRestDuration(int restCount) const override {
        return (restCount == 0) ? 5.0 : 8.0;
    }
};

class FastCamel : public GroundTransport {
public:
    FastCamel() { speed = 40.0; drivingTime = 10.0; }
    const char* getName() const override { return "Верблюд-быстроход"; }
    double getRestDuration(int restCount) const override {
        if (restCount == 0) return 5.0;
        if (restCount == 1) return 6.5;
        return 8.0;
    }
};

class Centaur : public GroundTransport {
public:
    Centaur() { speed = 15.0; drivingTime = 8.0; }
    const char* getName() const override { return "Кентавр"; }
    double getRestDuration(int) const override {
        return 2.0;
    }
};

class AllTerrainBoots : public GroundTransport {
public:
    AllTerrainBoots() { speed = 6.0; drivingTime = 60.0; }
    const char* getName() const override { return "Ботинки-вездеходы"; }
    double getRestDuration(int restCount) const override {
        return (restCount == 0) ? 10.0 : 5.0;
    }
};

// --- Воздушные ---
class MagicCarpet : public AirTransport {
public:
    MagicCarpet() { speed = 10.0; }
    const char* getName() const override { return "Ковёр-самолёт"; }
    double getDistanceReduction(int distance) const override {
        if (distance < 1000) return 0.0;
        if (distance < 5000) return 0.03;
        if (distance < 10000) return 0.1;
        return 0.05;
    }
};

class Eagle : public AirTransport {
public:
    Eagle() { speed = 8.0; }
    const char* getName() const override { return "Орёл"; }
    double getDistanceReduction(int) const override {
        return 0.06;
    }
};

class Broom : public AirTransport {
public:
    Broom() { speed = 20.0; }
    const char* getName() const override { return "Метла"; }
    double getDistanceReduction(int distance) const override {
        double reduction = 0.01 * (distance / 1000);
        return std::min(reduction, 0.99);
    }
};

// ===================== Фабричная функция =====================
extern "C" Transport** makeTransports(size_t& totalTransports) {
    const size_t count = 7;
    Transport** transports = new Transport * [count];

    transports[0] = new Camel();
    transports[1] = new FastCamel();
    transports[2] = new Centaur();
    transports[3] = new AllTerrainBoots();
    transports[4] = new MagicCarpet();
    transports[5] = new Eagle();
    transports[6] = new Broom();

    totalTransports = count;
    return transports;
}