# arduino-telegram-debt-collector

## Description

A lightweight Telegram bot running on Arduino that helps you track personal debts in a private chat.
The bot uses an inline keyboard interface for smooth interaction and stores who owes what, based on each user’s chat_id.

## Features

* Track debts with a simple command syntax.
* Works in private Telegram chats only.
* Inline keyboard for quick interaction.
* Debts are tied to a unique user ID (no manual debtor selection).
* Supports positive and negative values:
    * Positive ➝ Add new debt.
    * Negative ➝ Mark debt as paid.

## Requirements

* [PlatformIO](https://platformio.org/) installed.
* Arduino board with Telegram Bot support (e.g., ESP32/ESP8266).
* Wi-Fi connection.
* Bot token from [BotFather](https://core.telegram.org/bots#botfather).

## Setup & Installation

1. Create a file include/secrets.h (this file is already in .gitignore):
```cpp
#pragma once

#define WIFI_SSID       "your_wifi_name"
#define WIFI_PASSWORD   "your_wifi_password"
#define TELEGRAM_BOT_TOKEN "your_telegram_bot_token"
```

2. Build and upload firmware:
```bash
pio run --target upload
```

## Usage

Add the bot to a private chat and use the commands:
```
@DebtCollector {money_amount} // Registers debt for the sender’s chat_id.
@DebtCollector debt           // Shows your current outstanding debt.
```

* @DebtCollector 50 → You owe 50.
* @DebtCollector -50 → Your 50 debt is cleared.