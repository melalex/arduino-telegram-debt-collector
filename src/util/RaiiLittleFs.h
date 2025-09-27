#pragma once

#include <LittleFS.h>

namespace debt_collector::util
{
    class RaiiLittleFs
    {
    private:
        File file;

    public:
        RaiiLittleFs(const char *path, const char *mode);
        ~RaiiLittleFs();

        bool isOpen() const;
        size_t read(uint8_t *buffer, size_t size);
        size_t write(uint8_t *buffer, size_t size);
        bool seek(size_t pos);
        size_t position() const;
        size_t available();
        size_t size() const;
    };
}