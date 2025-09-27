#include "DebtService.h"

namespace debt_collector::debt
{

    DebtService::DebtService(debt_collector::util::FileDict &&dict) : dict(std::move(dict))
    {
    }

    DebtService::~DebtService()
    {
    }

    int DebtService::updateDebt(const String &id, int amount)
    {
        auto intId = id.toInt();
        auto oldValue = dict.get(intId);
        auto newValue = oldValue + amount;

        dict.put(intId, newValue);

        return newValue;
    }

    int DebtService::getDebt(const String &id)
    {
        return dict.get(id.toInt());
    }
}