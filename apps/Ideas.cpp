#if 1

    #include <Beast/Common/IdGenerators/IUuIdGenerator.h>
    #include <Beast/Common/Filesystem/Types.h>
    #include <span>
    #include <shared_mutex>
    #include <mutex>
#include <Beast/Assertions.h>

namespace be
{
    namespace graphics
    {
        struct Texture;

        struct Context
        {
            Texture* CreateTexture(const be::FilesystemPath& filepath /*, TextureParameters*/);
            void DestroyTexture(Texture*);
        };
    } // namespace graphics

    enum class TextureId : be::Id
    {
    };

    class TexturesManager
    {
    public:
        struct TexturePath
        {
            TextureId id;
            be::FilesystemPath filepath;
        };

    public:
        TexturesManager(std::span<const TexturePath> texturePaths, graphics::Context* context)
            : m_context(context)
        {
        }

        void Create(TextureId id)
        {
            auto& texture = m_textures[id];
            std::unique_lock lock{texture.mutex};
            if (!texture.count)
            {
                texture.data = m_context->CreateTexture(texture.filepath);
            }
        }

        graphics::Texture& Get(TextureId id)
        {
            auto& texture = m_textures[id];
            std::shared_lock lock{texture.mutex};

            BE_ASSERT(texture.data);
            return *texture.data;
        }

        void Destroy(TextureId id)
        {
            auto& texture = m_textures[id];
            std::unique_lock lock{texture.mutex};

            if (texture.count == 1)
            {
                m_context->DestroyTexture(texture.data);
                texture.data = nullptr;
            }

            --texture.count;
        }

    private:
        struct TextureResource
        {
            std::shared_mutex mutex; // This will mostl likely not be needed
            std::size_t count = 0;
            graphics::Texture* data;
            FilesystemPath filepath;
        };

    private:
        graphics::Context* m_context;
        std::unordered_map<TextureId, TextureResource> m_textures;
    };
} // namespace be
#endif