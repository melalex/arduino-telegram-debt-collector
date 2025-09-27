#pragma once

#include <Arduino.h>

namespace debt_collector::debt
{
    class IDebtService
    {
    public:
        virtual int updateDebt(const String &id, int amount) = 0;
        virtual int getDebt(const String &id) = 0;
    };
}