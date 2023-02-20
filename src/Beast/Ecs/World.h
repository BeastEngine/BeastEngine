#include <Beast/Ecs/Types.h>

namespace be
{
    class World final
    {
    public:
        [[nodiscard]] Entity CreateEntity() const;
        [[nodiscard]] bool IsValid(Entity entity) const;
    };
} // namespace be