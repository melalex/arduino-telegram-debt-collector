#pragma once

#include <Arduino.h>

#include "IDebtService.h"
#include "./util/FileDict.h"

namespace debt_collector::debt
{
    class DebtService : public IDebtService
    {
    private:
        debt_collector::util::FileDict dict;

    public:
        DebtService(debt_collector::util::FileDict &&dict);
        ~DebtService();

        int updateDebt(const String &id, int amount) override;
        int getDebt(const String &id) override;
    };
}