#pragma once

namespace debt_collector::telegram
{
    typedef telegramMessage TelegramMessage;
    typedef void (*MessageConsumer)(const TelegramMessage &);

    class ITelegramService
    {
    public:
        virtual void checkForNewMessages(MessageConsumer messageConsumer) = 0;
        virtual void sendMessage(const String &chat_id, const String &message, const String &parseMode) = 0;
        virtual void sendMessageWithInlineKeyboard(const String &chat_id,
                                                   const String &text,
                                                   const String &parse_mode,
                                                   const String &keyboard) = 0;
        virtual void answerInlineQuery(const String &query_id, const String &results) = 0;
        virtual void editMessageText(const String &inline_message_id, const String &text, const String &reply_markup) = 0;
    };
}