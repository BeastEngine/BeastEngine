#if 0

    #include "Beast/Common/IdGenerators/IUuIdGenerator.h"
    #include "Beast/Common/Filesystem/Types.h"
    #include <span>
    #include <shared_mutex>
    #include <mutex>
    #include "Beast/Debug.h"

namespace be
{
    namespace graphics
    {
        struct Texture;

        struct Context
        {
            Texture* CreateTexture(const be::fs::Path& filepath /*, TextureParameters*/);
            void DestroyTexture(Texture*);
        };
    } // namespace graphics

    enum class TextureId : be::Id
    {
    };

    class TexturesManager1
    {
    public:
        struct TexturePath
        {
            TextureId id;
            be::fs::Path filepath;
        };

    public:
        TexturesManager1(std::span<const TexturePath> texturePaths, graphics::Context* context)
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
            fs::Path filepath;
        };

    private:
        graphics::Context* m_context;
        std::unordered_map<TextureId, TextureResource> m_textures;
    };

    class TexturesManager2
    {
    public:
        struct TexturePath
        {
            TextureId id;
            be::fs::Path filepath;
        };

    public:
        TexturesManager2(std::span<const TexturePath> texturePaths, graphics::Context* context)
            : m_context(context)
        {
            // Initialize m_textures with nullptr for data and given texturePaths
        }

        graphics::Texture& Get(TextureId id)
        {
            auto& texture = m_textures[id];
            if (!texture.data)
            {
                Create(texture);
            }

            return *texture.data;
        }

    private:
        struct TextureResource
        {
            graphics::Texture* data;
            fs::Path filepath;
        };

        void Create(TextureResource& resource)
        {
            resource.data = m_context->CreateTexture(resource.filepath);
        }

    private:
        graphics::Context* m_context;
        std::unordered_map<TextureId, TextureResource> m_textures;
    };


    /**
     * PHYSICS.
     */
    class Physics
    {
        DynamicRB CreateDynamicRigidBody(...)
        {  
            // Assert all have the same size, so that our index points to the same element in all arrays.
            // This could be done by doing:
            // dynamicRBs.rigidBodies.resize(index);
            // etc.
            DynamicRB index = GenerateIndex();
            dynamicRBs.rigidBodies[index] = World.CreateRigidBody(...);
            dynamicRBs.velocities[index] = Velocity(0.0f, 0.0f);
            dynamicRBs.transforms[index] = ...;

           return level.physics.dynamicRBs.create(World::CreateRigidBody(...), Velocity{0.0f, 0.0f});
        }
    };


    for (std::size_t i = 0; i < )
    world.level.physics.rigidBodies
} // namespace be
#endif