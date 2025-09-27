#include "RaiiLittleFs.h"

namespace debt_collector::util
{
    RaiiLittleFs::RaiiLittleFs(const char *path, const char *mode)
    {
        file = LittleFS.open(path, mode, true);
    }

    RaiiLittleFs::~RaiiLittleFs()
    {
        if (file)
        {
            file.close();
        }
    }

    bool RaiiLittleFs::isOpen() const
    {
        return file;
    }

    size_t RaiiLittleFs::read(uint8_t *buffer, size_t size)
    {
        if (!file)
        {
            return 0;
        }

        return file.read(buffer, size);
    }

    size_t RaiiLittleFs::write(uint8_t *buffer, size_t size)
    {
        if (!file)
        {
            return 0;
        }

        return file.write(buffer, size);
    }

    bool RaiiLittleFs::seek(size_t pos)
    {
        if (!file)
        {
            return false;
        }

        return file.seek(pos);
    }

    size_t RaiiLittleFs::position() const
    {
        if (!file)
        {
            return 0;
        }

        return file.position();
    }

    size_t RaiiLittleFs::available()
    {
        if (!file)
        {
            return 0;
        }

        return file.available();
    }

    size_t RaiiLittleFs::size() const
    {
        if (!file)
        {
            return 0;
        }

        return file.size();
    }
}