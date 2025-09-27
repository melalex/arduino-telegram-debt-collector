#include <functional>

namespace debt_collector::telegram
{
    class ITelegramService
    {
    public:
        virtual void subscribeToMessages(const std::function<void(String &, String &)> &func) = 0;
        virtual void sendMessage(const String &chat_id, const String &message) = 0;
    };
}