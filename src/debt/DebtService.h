#pragma once

#include <Arduino.h>
#include <memory>

#include "IDebtService.h"
#include "./util/FileDict.h"

namespace debt_collector::debt
{
    class DebtService : public IDebtService
    {
    private:
        std::map<String, std::unique_ptr<debt_collector::util::FileDict>> dictStorage;
        String filenamePrefix;

        debt_collector::util::FileDict &getFileDict(const String &caller);

    public:
        DebtService(const String &filenamePrefix);
        ~DebtService();

        DebtService(const DebtService &) = delete;
        DebtService &operator=(const DebtService &) = delete;

        int updateDebt(const String &caller, const String &id, int amount) override;
        int getDebt(const String &caller, const String &id) override;
    };
}