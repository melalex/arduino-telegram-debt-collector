#include "FileDict.h"
#include "RaiiFs.h"

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
        if (!RaiiFs::begin(true))
        {
            Serial.println("FS mount failed");
            return false;
        }

        if (!ensureFileExists(filename.c_str()))
        {
            return false;
        }

        auto file = RaiiFs(filename.c_str(), FILE_READ);

        if (!file.isOpen())
        {
            Serial.print("Cannot open file: ");
            Serial.println(filename);
            return false;
        }

        index.clear();

        uint64_t key;
        int32_t value;
        size_t position = 0;

        do
        {
            if (!file.seek(position))
            {
                Serial.print("Cannot seek position: ");
                Serial.println(position);

                return false;
            }

            auto readSize = file.read(reinterpret_cast<uint8_t *>(&key), sizeof(key));

            if (readSize != sizeof(key))
            {
                Serial.print("Cannot read key at position: ");
                Serial.print(position);
                Serial.print(". Read size: ");
                Serial.println(readSize);

                return false;
            }

            readSize = file.read(reinterpret_cast<uint8_t *>(&value), sizeof(value));

            if (file.read(reinterpret_cast<uint8_t *>(&value), sizeof(value)) != sizeof(value))
            {
                Serial.print("Cannot read value at position: ");
                Serial.print(position);
                Serial.print(". Read size: ");
                Serial.println(readSize);

                return false;
            }

            index[key] = position + sizeof(key);

            Serial.print("Loaded index: ");
            Serial.print(key);
            Serial.print(" -> ");
            Serial.println(position);

            position += (sizeof(key) + sizeof(value));
        } while (file.available() >= sizeof(key) + sizeof(value));

        initialized = true;

        return true;
    }

    bool FileDict::isInitialized() const noexcept
    {
        return initialized;
    }

    int32_t FileDict::get(uint64_t key)
    {
        if (!index.count(key))
        {
            Serial.print("Cannot find key in the index: ");
            Serial.println(key);

            return 0;
        }

        auto position = index[key];

        auto file = RaiiFs(filename.c_str(), FILE_READ);

        if (!file.isOpen())
        {
            Serial.print("Cannot open file for read: ");
            Serial.println(filename);

            return 0;
        }

        if (!file.seek(position))
        {
            Serial.print("Cannot change position: ");
            Serial.println(position);

            return 0;
        }

        int32_t value;

        if (file.read(reinterpret_cast<uint8_t *>(&value), sizeof(value)) != sizeof(value))
        {
            Serial.print("Cannot read value: ");
            Serial.println(position);

            return 0;
        }

        return value;
    }

    bool FileDict::put(uint64_t key, int32_t value)
    {
        auto file = RaiiFs(filename.c_str(), FILE_APPEND);

        if (!file.isOpen())
        {
            Serial.print("Cannot open file for write: ");
            Serial.println(filename);
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

    bool FileDict::ensureFileExists(const char *path)
    {
        if (!RaiiFs::exists(path))
        {
            auto f = RaiiFs(path, FILE_WRITE);
            if (!f.isOpen())
            {
                Serial.println("Failed to create file: ");
                Serial.println(path);
                return false;
            }
            Serial.print("File created: ");
            Serial.println(path);
        }
        else
        {
            Serial.println("File already exists: ");
            Serial.println(path);
        }
        return true;
    }
}