#include "TelegramService.h"

namespace debt_collector::telegram
{
    TelegramService::TelegramService(UniversalTelegramBot &&bot) : bot(move(bot))
    {
    }

    TelegramService::~TelegramService()
    {
    }

    void TelegramService::subscribeToMessages(const function<void(String &, String &)> &func)
    {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while (numNewMessages)
        {
            for (int i = 0; i < numNewMessages; i++)
            {
                telegramMessage message = bot.messages[i];

                func(message.chat_id, message.text);
            }

            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
    }

    void TelegramService::sendMessage(const String &chat_id, const String &message)
    {
        bot.sendMessage(chat_id.c_str(), message.c_str(), "");
    }
}