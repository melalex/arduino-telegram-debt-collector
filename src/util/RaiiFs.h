#pragma once

#include <FS.h>

namespace debt_collector::util
{
    class RaiiFs
    {
    private:
        File file;

    public:
        RaiiFs(const char *path, const char *mode);
        ~RaiiFs();

        static bool begin(bool formatOnFail = false);
        static bool exists(const char *path);

        bool isOpen() const;
        size_t read(uint8_t *buffer, size_t size);
        size_t write(uint8_t *buffer, size_t size);
        bool seek(size_t pos);
        size_t position() const;
        size_t available();
        size_t size() const;
    };
}