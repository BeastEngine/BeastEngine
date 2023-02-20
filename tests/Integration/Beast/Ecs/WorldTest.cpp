#include <Integration/Beast/Ecs/WorldTest.h>

#include <Beast/Ecs/World.h>
#include <Beast/Ecs/Types.h>

namespace be::tests::integration
{
    TEST_F(WorldTest, CreateEntityWillAddNewEntityToTheWorld)
    {
        World sut;
        const auto newEntity = sut.CreateEntity();

        ASSERT_TRUE(newEntity != NULL_ENTITY);
        ASSERT_TRUE(sut.IsValid(newEntity));
    }
}