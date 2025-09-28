#include "DebtService.h"
#include "../util/StdExt.h"

using namespace debt_collector::util;

namespace debt_collector::debt
{

    DebtService::DebtService(const String &filenamePrefix) : filenamePrefix(filenamePrefix)
    {
    }

    DebtService::~DebtService()
    {
    }

    FileDict &DebtService::getFileDict(const String &caller)
    {
        if (!dictStorage.count(caller))
        {
            auto filename = "/" + filenamePrefix + "-" + caller + ".dat";
            auto filedict = std::make_unique<FileDict>(filename);

            dictStorage.emplace(caller, std::move(filedict));

            if (!dictStorage.at(caller)->init())
            {
                Serial.print("Failed to initialize FileDict: ");
                Serial.println(filename);
            }
        }

        return *dictStorage.at(caller);
    }

    int DebtService::updateDebt(const String &caller, const String &id, int amount)
    {
        auto &dict = getFileDict(caller);

        if (amount == 0)
        {
            return dict.get(id.toInt());
        }

        auto intId = id.toInt();
        auto oldValue = dict.get(intId);
        auto newValue = oldValue + amount;

        dict.put(intId, newValue);

        return newValue;
    }

    int DebtService::getDebt(const String &caller, const String &id)
    {
        auto &dict = getFileDict(caller);

        return dict.get(id.toInt());
    }
}