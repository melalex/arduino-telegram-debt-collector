#pragma once

#include <UniversalTelegramBot.h>
#include "ITelegramService.h"

namespace debt_collector::telegram
{
    class TelegramService : public ITelegramService
    {
    private:
        UniversalTelegramBot bot;

    public:
        TelegramService(UniversalTelegramBot &&bot);
        ~TelegramService();
        void subscribeToMessages(const function<void(String &, String &)> &func) override;
        void sendMessage(const String &chat_id, const String &message) override;
    };
}