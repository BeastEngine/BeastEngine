#pragma once
#include "Integration/FilesystemTestCase.h"
#include "Integration/Beast/Graphics/TestImageData.h"

#include <Beast/Graphics/Images.h>
#include <Beast/Graphics/Types.h>

namespace be::tests::integration
{
    class ResourcesTestCase : public FilesystemTestCase
    {
    public:
        static constexpr const char* TEST_TEXTURE_NAME = "TestTexture.png";
        static constexpr graphics::TextureId TEST_TEXTURE_ID = graphics::TextureId(TEST_TEXTURE_NAME);

    protected:
        const fs::Path& GetTexturesDirectory() const
        {
            return m_texturesDir;
        }

        fs::Path GetTextureImagePath(const fs::Path& filename) const
        {
            return m_texturesDir / filename;
        }

        void CreateTextureFile(const fs::Path& filePath, const graphics::Image& image) const;

        graphics::Image TestTextureToImage() const
        {
            return ConvertTextureToImage(TEST_IMAGE_WIDTH, TEST_IMAGE_HEIGHT, TEST_IMAGE_CHANNELS, TEST_IMAGE_COLOR_DATA);
        }

        graphics::Image ConvertTextureToImage(uint8 width, uint8 height, uint8 numberOfChannels, const std::vector<std::vector<unsigned char>>& textureData) const
        {
            graphics::Image image;
            image.width = width;
            image.height = height;
            image.numberOfChannels = numberOfChannels;

            auto& imageData = image.data;
            imageData.resize(image.width * image.height * image.numberOfChannels);

            for (std::size_t pixel = 0; pixel < textureData.size(); ++pixel)
            {
                for (uint8 channel = 0; channel < image.numberOfChannels; ++channel)
                {
                    imageData[pixel * image.numberOfChannels + channel] = static_cast<unsigned char>(textureData[pixel][channel]);
                }
            }

            return image;
        }

    private:
        void SetUp() override
        {
            FilesystemTestCase::SetUp();

            const auto& rootDirPath = GetBaseDirectoryPath();
            m_texturesDir = rootDirPath / "Textures";

            const graphics::Image textureImage = TestTextureToImage();
            const fs::Path testTexturePath = CreateDirectory(m_texturesDir) / TEST_TEXTURE_NAME;
            CreateTextureFile(testTexturePath, textureImage);
        }

    private:
        fs::Path m_texturesDir;
    };
} // namespace be::tests::integration