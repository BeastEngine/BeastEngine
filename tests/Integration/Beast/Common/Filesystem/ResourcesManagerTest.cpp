#include "Integration/Beast/Common/Filesystem/ResourcesTestCase.h"

#include <Beast/Common/Filesystem/ResourcesManager.h>
#include <Beast/Graphics/Types.h>

namespace be::tests::integration
{
    class ResourcesManagerTestCase : public ResourcesTestCase
    {
    };

    TEST_F(ResourcesManagerTestCase, GetResourcePathWillReturnAbsoluteTextureFilepath)
    {
        const fs::Path expectedPath = GetTextureImagePath(TEST_TEXTURE_NAME);
        fs::ResourcesManager sut(GetBaseDirectoryPath());

        const Result<const fs::Path*> actualPath = sut.GetResourcePath(TEST_TEXTURE_ID);
        ASSERT_TRUE(actualPath);

        ASSERT_EQ(expectedPath, *actualPath.Value());
    }

    TEST_F(ResourcesManagerTestCase, GetResourcePathWillReturnAbsoluteTextureFilepathWithSubDirectories)
    {
        constexpr const char* TEXTURE_FILE_NAME = "Wolf.png";
        constexpr const char* TEXTURE_FULL_NAME = "Characters/Monsters/Wolf.png";
        constexpr const char* TEXTURE_SUB_DIRS = "Characters/Monsters";
        constexpr auto TEXTURE_ID = graphics::TextureId(TEXTURE_FULL_NAME);

        const fs::Path texturesDirPath = GetTexturesDirectory() / TEXTURE_SUB_DIRS;
        CreateDirectory(texturesDirPath);

        const fs::Path expectedPath = texturesDirPath / TEXTURE_FILE_NAME;

        const graphics::Image testTextureImage = TestTextureToImage();
        CreateTextureFile(expectedPath, testTextureImage);

        fs::ResourcesManager sut(GetBaseDirectoryPath());
        const Result<const fs::Path*> actualPath = sut.GetResourcePath(TEXTURE_ID);
        ASSERT_TRUE(actualPath);

        ASSERT_EQ(expectedPath, *actualPath.Value());
    }

    TEST_F(ResourcesManagerTestCase, GetResourcePathWillReturnErrorResultIfTextureDoesNotExist)
    {
        constexpr auto invalidTextureId = graphics::TextureId("Invalid.png");

        fs::ResourcesManager sut(GetBaseDirectoryPath());
        const Result<const fs::Path*> actualPath = sut.GetResourcePath(invalidTextureId);
        ASSERT_FALSE(actualPath);
    }
} // namespace be::tests::integration