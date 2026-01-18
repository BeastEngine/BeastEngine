#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "Integration/Beast/Common/Filesystem/ResourcesTestCase.h"

namespace be::tests::integration
{
    void ResourcesTestCase::CreateTextureFile(const fs::Path& filePath, const graphics::Image& image) const
    {
        const int strideInBytes = sizeof(unsigned char) * image.numberOfChannels * image.width;

        const int result = stbi_write_png(filePath.string().c_str(), image.width, image.height, static_cast<int>(image.numberOfChannels), image.data.data(), strideInBytes);
        if (result == 0)
        {
            throw std::runtime_error("Failed to save test image data");
        }
    }
} // namespace be::tests::integration