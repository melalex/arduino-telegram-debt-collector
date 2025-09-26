#include <functional>

using namespace std;

namespace debt_collector::telegram
{
    class ITelegramService
    {
    public:
        virtual void subscribeToMessages(const function<void(String &, String &)> &func);
        virtual void sendMessage(const String &chat_id, const String &message);
    };
}