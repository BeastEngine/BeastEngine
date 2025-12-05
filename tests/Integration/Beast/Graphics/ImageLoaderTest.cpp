#include "Integration/Beast/Config.h"
#include "Integration/Beast/Graphics/TestImageData.h"

#include <Beast/Graphics/ImageLoader.h>
#include <Beast/Graphics/Types.h>

#include <TestSetup.h>

// TODO: Rename Common to Core

namespace be::tests::integration
{
    class ImageLoaderTest : public testing::Test
    {
    protected:
        static fs::Path CreateImagePath(const fs::Path& imageFileName)
        {
            return GetConfig().dataBasePath / "textures" / imageFileName;
        }

        std::vector<std::vector<unsigned char>> ConvertImageDataToColorData(const graphics::Image& image)
        {
            const auto& imageData = image.data;

            std::vector<std::vector<unsigned char>> colorData{};
            colorData.resize(imageData.size() / image.numberOfChannels);

            std::size_t pixel = 0;
            for (std::size_t i = 0; i < imageData.size(); i += image.numberOfChannels)
            {
                colorData[pixel].resize(image.numberOfChannels);
                for (uint8 channel = 0; channel < image.numberOfChannels; ++channel)
                {
                    colorData[pixel][channel] = imageData[i + channel];
                }

                ++pixel;
            }

            return colorData;
        }
    };

    TEST_F(ImageLoaderTest, LoadImageWillReturnValidImageStruct)
    {
        const uint32 expectedWidth = 9;
        const uint32 expectedHeight = 9;
        const uint32 expectedChannels = 4;
        const std::size_t expectedDataSize = 324;

        const auto imagePath = CreateImagePath("test.png");
        const Result<graphics::Image> actualImage = graphics::LoadImageFromFile(imagePath);
        ASSERT_TRUE(actualImage);

        ASSERT_EQ(expectedWidth, actualImage->width);
        ASSERT_EQ(expectedHeight, actualImage->height);
        ASSERT_EQ(expectedChannels, actualImage->numberOfChannels);
        ASSERT_EQ(expectedDataSize, actualImage->data.size());

        const auto actualColorData = ConvertImageDataToColorData(actualImage.Value());
        ASSERT_EQ(EXPECTED_IMAGE_COLOR_DATA.size(), actualColorData.size());

        for (std::size_t pixel = 0; pixel < actualColorData.size(); ++pixel)
        {
            ASSERT_EQ(EXPECTED_IMAGE_COLOR_DATA[pixel], actualColorData[pixel]) << "PIXEL " << pixel << "\n";
        }
    }

    TEST_F(ImageLoaderTest, LoadImageWillReturnInvalidImageStructOnError)
    {
        const auto imagePath = "invalid/Image/Path.png";
        const Result<graphics::Image> actualImage = graphics::LoadImageFromFile(imagePath);
        ASSERT_FALSE(actualImage);
    }

} // namespace be::tests::integration
