#pragma once

#include <map>
#include <optional>
#include <Arduino.h>

namespace debt_collector::util
{
    class FileDict
    {
    private:
        std::map<uint64_t, uint32_t> index;
        String filename;
        bool initialized = false;

    public:
        FileDict(const String &filename) noexcept;
        FileDict(FileDict &&other) noexcept;
        ~FileDict() noexcept;

        bool init();
        bool isInitialized() const noexcept;
        int32_t get(uint64_t key);
        bool put(uint64_t key, int32_t value);
    };
}