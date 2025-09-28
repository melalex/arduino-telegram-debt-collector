#pragma once

#include <UniversalTelegramBot.h>
#include "ITelegramService.h"

namespace debt_collector::telegram
{
    class TelegramService : public ITelegramService
    {
    private:
        UniversalTelegramBot bot;
        unsigned long lastMessageReceivedAt;
        unsigned long messageRate;

    public:
        TelegramService(UniversalTelegramBot &&bot, unsigned long messageRate);
        ~TelegramService();

        void checkForNewMessages(MessageConsumer messageConsumer) override;
        void sendMessage(const String &chat_id, const String &message, const String &parseMode) override;
        void sendMessageWithInlineKeyboard(const String &chat_id,
                                           const String &text,
                                           const String &parse_mode,
                                           const String &keyboard) override;

        void answerInlineQuery(const String &query_id, const String &results) override;
        void editMessageText(const String &inline_message_id, const String &text, const String &reply_markup) override;
    };
}