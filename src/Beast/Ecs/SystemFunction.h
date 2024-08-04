#pragma once
#include "Beast/Ecs/AccessList.h"

#include "Beast/Common/Id.h"

#include <string_view>
#include <vector>
#include <typeindex>

namespace be
{
    class World;

    template<typename T>
    concept WorldView = requires {
        std::is_same<typename std::decay_t<T>::AL, BaseAccessList>::value;
    };

    enum class FunctionRelation
    {
        NONE,
        PARENT,
        CHILD,
        WEAK_DEPENDENCY,
    };

    enum class ComponentAccess
    {
        GET = 0,
        UPDATE = 1,
        ADD = 2,
        REMOVE = 3,
    };

    struct SystemFunction
    {
        struct Component
        {
            using ComponentId = std::type_index;

            ComponentId id;
            ComponentAccess access;
            std::string name;

            bool operator==(const Component& rhs) const
            {
                return id == rhs.id;
            }
        };

        using Wrapper = std::function<void(be::World&)>;

        template<WorldView... Views>
        static SystemFunction Create(Id id, std::string_view name, Wrapper&& function)
        {
            SystemFunction systemFunction{id, name, std::move(function)};
            systemFunction.Process<Views...>();

            return systemFunction;
        }

        template<WorldView... Views>
        void Process()
        {
            using Get = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Get...>>;
            using Update = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Update...>>;
            using Add = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Add...>>;
            using Remove = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Remove...>>;

            ProcessComponents(Remove{}, ComponentAccess::REMOVE);
            ProcessComponents(Add{}, ComponentAccess::ADD);
            ProcessComponents(Update{}, ComponentAccess::UPDATE);
            ProcessComponents(Get{}, ComponentAccess::GET);
        }

        template<typename... ComponentsT>
        void ProcessComponents(be::Components<ComponentsT...> types, ComponentAccess access)
        {
            if constexpr (types.size != 0)
            {
                (AddComponent({std::type_index(typeid(ComponentsT)), access, typeid(ComponentsT).name()}), ...);
            }
        }

        FunctionRelation GetFunctionRelation(const Component& component, const SystemFunction& other) const;

        void AddParent(SystemFunction* parentFunction);
        void AddWeakDependency(SystemFunction* function);
        void AddComponent(Component&& component);
        void RemoveParent(SystemFunction* parent);

        /**
         * @brief A BFS implementation that searches for a weak dependency in the graph of this function.
         * When dependency is found, it is removed from the list of weak dependencies, as this means there's already an indirect
         * connection between this function and that dependency.
         * 
         * Otherwise, there has to be a strong dependency between them, as they cannot run at the same time.
         * In that case, we simply convert the weak dependency into a strong dependency.
         */
        void ResolveWeakDependencies();
        void SimplifyDependencies();

    public:
        Id m_id;
        std::string_view m_name;
        Wrapper m_implementation;

        std::vector<SystemFunction*> m_parents;
        std::vector<SystemFunction*> m_children;
        std::vector<SystemFunction*> m_weakDependencies;

        std::vector<Component> m_components;
    };

    /**
     * The implementation I am going to go with is that each function has an internal counter that when increased, checks if it reached 0 and if so, tries to enqueue itself to the thread pool.
     * So the functions don't really actively wait for each other. They are just poked by their parents that they can be run. This also should make sure that the functions that can be run first, are.
     */
} // namespace be
