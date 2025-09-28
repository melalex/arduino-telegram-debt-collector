#define TELEGRAM_DEBUG 1

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "secrets.h"
#include "telegram/TelegramService.h"
#include "debt/DebtService.h"
#include <SPIFFS.h>

using namespace debt_collector::telegram;
using namespace debt_collector::debt;
using namespace std;

const unsigned long BOT_MTBS = 1000;

WiFiClientSecure securedClient;
TelegramService telegramService(UniversalTelegramBot(TELEGRAM_BOT_TOKEN, securedClient), BOT_MTBS);
DebtService debtService("debts");

void handleNewMessages(const TelegramMessage &message);
uint64_t generateUniqueId();
String createResultsJson(int amount, const String &sender_id);

void setup()
{
    Serial.begin(9600);

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    Serial.print("Connecting to Wifi SSID [ ");
    Serial.print(WIFI_SSID);
    Serial.println(" ]");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    securedClient.setCACert(TELEGRAM_CERTIFICATE_ROOT);

    auto wifiStatus = WiFi.status();
    auto iterCount = 0;

    while (wifiStatus != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);

        if (iterCount % 2 == 0)
        {
            digitalWrite(LED_RED, LOW);
        }
        else
        {
            digitalWrite(LED_RED, HIGH);
        }

        wifiStatus = WiFi.status();

        if (iterCount % 10 == 0)
        {
            Serial.println();
            Serial.print("WiFi status: ");
            Serial.println(wifiStatus);
        }

        iterCount++;
    }

    digitalWrite(LED_RED, HIGH);

    Serial.print("WiFi connected. IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Retrieving time: ");
    configTime(0, 0, "pool.ntp.org"); 
    auto now = time(nullptr);

    iterCount = 0;

    while (now < 24 * 3600)
    {
        Serial.print(".");
        delay(500);
        now = time(nullptr);

        if (iterCount % 2 == 0)
        {
            digitalWrite(LED_GREEN, LOW);
        }
        else
        {
            digitalWrite(LED_GREEN, HIGH);
        }
        iterCount++;
    }

    Serial.println(now);

    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
}

void loop()
{
    telegramService.checkForNewMessages(&handleNewMessages);
}

void handleNewMessages(const TelegramMessage &message)
{
    if (message.type == "inline_query")
    {
        Serial.print("Inline query called by [ ");
        Serial.print(message.from_id);
        Serial.print(" ]. Text: ");
        Serial.println(message.text);

        auto amount = message.text.toInt();

        auto results = createResultsJson(amount, message.from_id);

        telegramService.answerInlineQuery(message.query_id, results);
    }
    else if (message.type == "callback_query")
    {
        Serial.print("Call back button pressed: ");
        Serial.println(message.inline_message_id);

        auto input = message.text;
        auto sep = input.indexOf('|');

        if (sep != -1)
        {
            auto caller = input.substring(0, sep);
            auto amount = input.substring(sep + 1).toInt();

            Serial.print("Caller is: ");
            Serial.println(caller);
            Serial.print("Amount is: ");
            Serial.println(amount);

            auto newDebtAmount = debtService.updateDebt(caller, message.from_id, amount);
            auto responseText = "Debt change is accepted. New debt amount: " + String(newDebtAmount);

            telegramService.editMessageText(message.inline_message_id, responseText, "{inline_keyboard:[]}");
        }
    }
}

uint64_t generateUniqueId()
{
    uint64_t high = (uint64_t)micros();
    uint64_t low = (uint64_t)(esp_random());
    return (high << 32) | low;
}

inline String createResultsJson(int amount, const String &sender_id)
{
    uint64_t id1 = generateUniqueId();
    uint64_t id2 = generateUniqueId();

    return "[{\"type\":\"article\",\"id\":\"" + String(id1) + "\",\"title\":\"Lend the recipient " + String(amount) + "\",\"input_message_content\":{\"message_text\":\"Do you accept an increase in your debt by " + String(amount) + "?\"},\"reply_markup\":{\"inline_keyboard\":[[{\"text\":\"Ack\",\"callback_data\":\"" + sender_id + "|" + String(amount) + "\"}]]},\"description\":\"Recipient debt would be increased by " + String(amount) + "\"},{\"type\":\"article\",\"id\":\"" + String(id2) + "\",\"title\":\"Reduce the recipient's debt by " + String(amount) + "\",\"input_message_content\":{\"message_text\":\"Do you accept a decrease in your debt by " + String(amount) + "?\"},\"reply_markup\":{\"inline_keyboard\":[[{\"text\":\"Ack\",\"callback_data\":\"" + sender_id + "|-" + String(amount) + "\"}]]},\"description\":\"Recipient debt would be decreased by " + String(amount) + "\"}]";
}