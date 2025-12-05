#include "Integration/FilesystemTestCase.h"

#include <Beast/Common/Filesystem/ResourcesManager.h>
#include <Beast/Graphics/Types.h>

namespace be::tests::integration
{
    class ResourcesManagerTestCase : public FilesystemTestCase
    {
    public:
        static constexpr const char* TEST_TEXTURE_NAME = "TestTexture.png";
        static constexpr graphics::TextureId TEST_TEXTURE_ID = graphics::TextureId(TEST_TEXTURE_NAME);

    protected:
        const fs::Path& GetTextureFullPath() const
        {
            return m_texturesDir;
        }

    private:
        void SetUp() override
        {
            const auto& rootDirPath = GetBaseDirectoryPath();
            m_texturesDir = rootDirPath / "Textures";

#undef CreateDirectory
#undef CreateFile
            const fs::Path testTexturePath = CreateDirectory(m_texturesDir) / TEST_TEXTURE_NAME;
            CreateFile(testTexturePath, "content");
        }

    private:
        fs::Path m_texturesDir;
    };

    TEST_F(ResourcesManagerTestCase, GetTextureWillReturnAbsoluteFilepath)
    {
        const fs::Path& expectedPath = GetTextureFullPath() / TEST_TEXTURE_NAME;
        fs::ResourcesManager sut(GetBaseDirectoryPath());

        const Result<const fs::Path*> actualPath = sut.GetTexturePath(TEST_TEXTURE_ID);
        ASSERT_TRUE(actualPath);

        ASSERT_EQ(expectedPath, *actualPath.Value());
    }
} // namespace be::tests::integration