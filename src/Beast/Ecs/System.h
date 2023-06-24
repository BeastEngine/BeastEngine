#pragma once
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Common/Helpers.h>

#include <type_traits>
#include <utility>

namespace be
{
    class ISystem
    {
    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(ISystem);
        ISystem() = default;
        virtual ~ISystem() = default;
    };

    template<typename T>
    concept ecs_system = std::is_base_of<ISystem, T>::value;
} // namespace be