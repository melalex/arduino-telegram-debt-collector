#include "RaiiFs.h"

#include <SPIFFS.h>

namespace debt_collector::util
{
    RaiiFs::RaiiFs(const char *path, const char *mode)
    {
        file = SPIFFS.open(path, mode, true);
    }

    RaiiFs::~RaiiFs()
    {
        if (file)
        {
            file.close();
        }
    }

    bool RaiiFs::begin(bool formatOnFail)
    {
        return SPIFFS.begin(formatOnFail);
    }

    bool RaiiFs::exists(const char *path)
    {
        return SPIFFS.exists(path);
    }

    bool RaiiFs::isOpen() const
    {
        return file;
    }

    size_t RaiiFs::read(uint8_t *buffer, size_t size)
    {
        if (!file)
        {
            return 0;
        }

        return file.read(buffer, size);
    }

    size_t RaiiFs::write(uint8_t *buffer, size_t size)
    {
        if (!file)
        {
            return 0;
        }

        return file.write(buffer, size);
    }

    bool RaiiFs::seek(size_t pos)
    {
        if (!file)
        {
            return false;
        }

        return file.seek(pos);
    }

    size_t RaiiFs::position() const
    {
        if (!file)
        {
            return 0;
        }

        return file.position();
    }

    size_t RaiiFs::available()
    {
        if (!file)
        {
            return 0;
        }

        return file.available();
    }

    size_t RaiiFs::size() const
    {
        if (!file)
        {
            return 0;
        }

        return file.size();
    }
}