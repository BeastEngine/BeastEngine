#include "Beast/Ecs/SystemFunction.h"

#include <exception>
#include <queue>

namespace be
{
    static constexpr FunctionRelation EvaluateRelation(ComponentAccess lhsAccess, ComponentAccess rhsAccess)
    {
        using Access = ComponentAccess;

        // For components with the same access (except GET), we make RHS a weak dependency of the LHS by default.
        if (lhsAccess == rhsAccess)
        {
            // Right Hand Side also GET the component, so we don't need to depend on each other.
            if (lhsAccess == Access::GET)
            {
                return FunctionRelation::NONE;
            }

            // Other access, let's make a weak dependency as we can't decide which one should run prior to the other yet.
            return FunctionRelation::WEAK_DEPENDENCY;
        }

        // At this point, we know for sure that lhsAccess and rhsAccess are NOT the same.

        // If we have it in GET, we need to depend on the RHS function,
        // because it means it has it in a higher access list, thus it modifies the component
        if (lhsAccess == Access::GET)
        {
            // Right Hand Side modifes the component, hance, we depend on it.
            return FunctionRelation::PARENT;
        }

        // If RHS has it in REMOVE or ADD, make it our parent.
        // Otherwise, make it our child.
        if (lhsAccess == Access::UPDATE)
        {
            if (rhsAccess == Access::GET)
            {
                // Right Hand Side reads the component we're modifying, it has to be our child.
                return FunctionRelation::CHILD;
            }

            // Right Hand Side adds or removes the component. We need to depend on it.
            return FunctionRelation::PARENT;
        }

        // If RHS has it in REMOVE, make it a parent.
        // Otherwise, make it a child.
        if (lhsAccess == Access::ADD)
        {
            if (rhsAccess == Access::REMOVE)
            {
                // Right Hand Side removes the component we're adding. We need to depend on it.
                return FunctionRelation::PARENT;
            }

            // Right Hand Side depends on us adding the component. It needs to depend on us.
            return FunctionRelation::CHILD;
        }

        // We have the component in REMOVE list at this point, so make RHS a child.
        return FunctionRelation::CHILD;
    }

    static constexpr bool SearchParentDFS(const SystemFunction* nodeToFind, std::span<SystemFunction* const> nodesToSearch, std::unordered_set<const SystemFunction*>& visitedNodes)
    {
        for (const auto* processedNode : nodesToSearch)
        {
            if (nodeToFind == processedNode)
            {
                return true;
            }

            if (!visitedNodes.contains(processedNode))
            {
                visitedNodes.insert(processedNode);
                if (SearchParentDFS(nodeToFind, processedNode->Parents(), visitedNodes))
                {
                    return true;
                }
            }
        }

        return false;
    }

    std::string_view SystemFunction::Name() const
    {
        return m_name;
    }

    const std::vector<SystemFunction*>& SystemFunction::Parents() const
    {
        return m_parents;
    }

    const std::vector<SystemFunction*>& SystemFunction::Children() const
    {
        return m_children;
    }

    FunctionRelation SystemFunction::GetFunctionRelation(const Component& component, const SystemFunction& other) const
    {
        const auto foundComponent = std::find(other.m_components.begin(), other.m_components.end(), component);
        if (foundComponent == other.m_components.end())
        {
            return FunctionRelation::NONE;
        }

        return EvaluateRelation(component.access, foundComponent->access);
    }

    void SystemFunction::AddParent(SystemFunction* parentFunction)
    {
        m_parents.push_back(parentFunction);
        parentFunction->m_children.push_back(this);
    }

    void SystemFunction::AddWeakDependency(SystemFunction* function)
    {
        m_weakDependencies.push_back(function);
    }

    void SystemFunction::AddComponent(Component&& component)
    {
        if (std::find(m_components.begin(), m_components.end(), component) == m_components.end())
        {
            m_components.emplace_back(std::move(component));
        }
        else
        {
            throw std::runtime_error("Components not unique");
        }
    }

    void SystemFunction::RemoveParent(SystemFunction* parent)
    {
        auto& children = parent->m_children;
        for (std::size_t i = 0; i < children.size(); ++i)
        {
            if (children[i] == this)
            {
                children[i] = children.back();
                children.pop_back();

                break;
            }
        }

        for (std::size_t i = 0; i < m_parents.size(); ++i)
        {
            if (m_parents[i] == parent)
            {
                m_parents[i] = m_parents.back();
                m_parents.pop_back();

                break;
            }
        }
    }

    void SystemFunction::ResolveWeakDependencies()
    {
        if (m_weakDependencies.empty())
        {
            return;
        }

        for (size_t i = 0; i < m_weakDependencies.size(); ++i)
        {
            const auto removeWeakDependency = [this](size_t i) {
                m_weakDependencies[i] = m_weakDependencies.back();
                m_weakDependencies.pop_back();
            };

            auto* weakDependency = m_weakDependencies[i];
            bool dependencyRemoved = false;

            std::unordered_set<SystemFunction*> visited{};
            std::queue<SystemFunction*> queue{};
            queue.push(this);
            while (!queue.empty())
            {
                auto* function = queue.front();
                queue.pop();
                if (visited.contains(function))
                {
                    continue;
                }
                visited.insert(function);

                // If one of the functions in the graph we're in has our weak dependency as an edge,
                // remove our weak dependency as there's already an indirect edge via one of our direct edges.
                if (function == weakDependency)
                {
                    removeWeakDependency(i);
                    dependencyRemoved = true;
                    break;
                }

                for (auto* parent : function->m_parents)
                {
                    queue.push(parent);
                }

                for (auto* child : function->m_children)
                {
                    queue.push(child);
                }
            }

            // Weak dependency not found in the graph. We need to decide manually.
            if (!dependencyRemoved)
            {
                AddParent(weakDependency);
                removeWeakDependency(i);
            }
        }
    }

    void SystemFunction::SimplifyDependencies()
    {
        std::vector<SystemFunction*> queue = m_parents;
        while (!queue.empty())
        {
            std::unordered_set<const SystemFunction*> visitedNodes{};

            SystemFunction* searchedParent = queue.back();
            queue.pop_back();

            RemoveParent(searchedParent);
            if (!SearchParentDFS(searchedParent, m_parents, visitedNodes))
            {
                AddParent(searchedParent);
            }
        }
    }
} // namespace be
