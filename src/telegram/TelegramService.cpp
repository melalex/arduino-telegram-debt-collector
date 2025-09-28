#include "TelegramService.h"

using namespace std;

namespace debt_collector::telegram
{
    TelegramService::TelegramService(UniversalTelegramBot &&bot, unsigned long messageRate) : bot(move(bot)), messageRate(messageRate)
    {
    }

    TelegramService::~TelegramService()
    {
    }

    void TelegramService::checkForNewMessages(MessageConsumer messageConsumer)
    {
        if (millis() - lastMessageReceivedAt > messageRate)
        {
            auto numNewMessages = bot.getUpdates(bot.last_message_received + 1);

            while (numNewMessages)
            {
                for (int i = 0; i < numNewMessages; i++)
                {
                    auto message = bot.messages[i];

                    messageConsumer(message);
                }

                numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            }

            lastMessageReceivedAt = millis();
        }
    }

    void TelegramService::sendMessage(const String &chat_id, const String &message, const String &parseMode)
    {
        bot.sendMessage(chat_id.c_str(), message.c_str(), parseMode.c_str());
    }

    void TelegramService::sendMessageWithInlineKeyboard(const String &chat_id,
                                                        const String &text,
                                                        const String &parse_mode,
                                                        const String &keyboard)
    {
        bot.sendMessageWithInlineKeyboard(chat_id.c_str(), text.c_str(), parse_mode.c_str(), keyboard.c_str());
    }

    void TelegramService::answerInlineQuery(const String &query_id, const String &results)
    {
        bot.answerInlineQuery(query_id.c_str(), results.c_str(), 0, true);
    }

    void TelegramService::editMessageText(const String &inline_message_id, const String &text, const String &reply_markup)
    {
        bot.editMessageText(inline_message_id.c_str(), text.c_str(), reply_markup.c_str());
    }
}