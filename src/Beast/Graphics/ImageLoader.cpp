#define STB_IMAGE_IMPLEMENTATION

#include "Beast/Graphics/ImageLoader.h"
#include "Beast/Debug.h"

#include <stb/stb_image.h>
#include <exception>
#include <format>

namespace be::graphics
{
    Result<Image> LoadImageFromFile(const fs::Path& imagePath)
    {
        static constexpr int DESIRED_CHANNELS_DEFAULT = 0;

        int width = 0;
        int height = 0;
        int numberOfChannels = 0;

        stbi_set_flip_vertically_on_load(1);
        stbi_uc* pixelData = stbi_load(
            imagePath.string().c_str(),
            &width,
            &height,
            &numberOfChannels,
            DESIRED_CHANNELS_DEFAULT
        );

        if (pixelData == nullptr)
        {
            stbi_image_free(pixelData);
            return std::format("An error occurred during {} image loading. Reason: {}", imagePath.string().c_str(), stbi_failure_reason());
        }

        using SizeType = std::vector<unsigned char>::size_type;
        const auto imageSize = static_cast<SizeType>(width) * static_cast<SizeType>(height) * numberOfChannels;
        return Image{
            .data = std::vector<unsigned char>(pixelData, pixelData + imageSize),
            .width = static_cast<uint32>(width),
            .height = static_cast<uint32>(height),
            .numberOfChannels = static_cast<uint8>(numberOfChannels),
        };
    }
} // namespace be::graphics
