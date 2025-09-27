#include <LittleFS.h>

#include "FileDict.h"
#include "RaiiLittleFs.h"

namespace debt_collector::util
{
    FileDict::FileDict(const String &filename) noexcept : filename(filename)
    {
    }

    FileDict::FileDict(FileDict &&other) noexcept
        : index(std::move(other.index)),
          filename(std::move(other.filename))
    {
    }

    FileDict::~FileDict() noexcept
    {
    }

    bool FileDict::init()
    {
        if (!LittleFS.begin())
        {
            return false;
        }

        auto file = RaiiLittleFs(filename.c_str(), FILE_APPEND);

        if (!file.isOpen())
        {
            return false;
        }

        index.clear();

        uint64_t key;
        int32_t value;
        size_t position = sizeof(key);

        while (file.available() >= sizeof(key) + sizeof(value))
        {
            if (!file.seek(position))
            {
                return false;
            }

            if (file.read(reinterpret_cast<uint8_t *>(&key), sizeof(key)) != sizeof(key))
            {
                return false;
            }

            if (file.read(reinterpret_cast<uint8_t *>(&value), sizeof(value)) != sizeof(value))
            {
                return false;
            }

            index[key] = position;

            position += (sizeof(key) + sizeof(value));
        }

        return true;
    }

    int32_t FileDict::get(uint64_t key)
    {
        if (!index.count(key))
        {
            return 0;
        }

        auto position = index[key];

        auto file = RaiiLittleFs(filename.c_str(), FILE_READ);

        if (!file.isOpen())
        {
            return 0;
        }

        if (!file.seek(position))
        {
            return 0;
        }

        int32_t value;

        if (file.read(reinterpret_cast<uint8_t *>(&value), sizeof(value)) != sizeof(value))
        {
            return 0;
        }

        return value;
    }

    bool FileDict::put(uint64_t key, int32_t value)
    {
        auto file = RaiiLittleFs(filename.c_str(), FILE_APPEND);

        if (!file.isOpen())
        {
            return false;
        }

        if (index.count(key))
        {
            auto position = index[key];

            file.seek(position);
            file.write((uint8_t *)&value, sizeof(value));
        }
        else
        {
            auto offset = file.position() + sizeof(key);

            index[key] = offset;

            file.write((uint8_t *)&key, sizeof(key));
            file.write((uint8_t *)&value, sizeof(value));
        }

        return true;
    }
}