#include "Beast/Ecs/Scheduler.h"

#include <stdexcept>

namespace be
{
    Schedule Scheduler::Prepare()
    {
        const auto functionsCount = m_functions.size();
        for (std::size_t i = 0; i < functionsCount; ++i)
        {
            auto& lhsFunction = m_functions[i];
            for (std::size_t j = i + 1; j < functionsCount; ++j)
            {
                CheckDependencies(lhsFunction, m_functions[j]);
            }

            if (lhsFunction.m_parents.empty())
            {
                m_starterFunctions.push_back(&lhsFunction);
            }
        }

        ResolveDependencies(m_functions);
        SetUpStarterFunctions(m_starterFunctions);
        VerifyGraphIsDAG(m_functions);

        return Schedule{m_starterFunctions};
    }

    void Scheduler::CheckFunctionUniquness(std::string_view name)
    {
        const auto it = std::find_if(m_functions.begin(), m_functions.end(), [&](const SystemFunction& fn) { return fn.m_name == name; });
        if (it != m_functions.end())
        {
            throw std::runtime_error("Function already registered!");
        }
    }

    void Scheduler::CheckDependencies(SystemFunction& lhs, SystemFunction& rhs)
    {
        using Relation = FunctionRelation;
        Relation currentRelation = Relation::NONE;

        for (const auto& component : lhs.m_components)
        {
            const auto newRelation = lhs.GetFunctionRelation(component, rhs);

            // The relation is not changed, so we can ignore it.
            if (newRelation == Relation::NONE || newRelation == currentRelation)
            {
                continue;
            }

            if (currentRelation != Relation::NONE)
            {
                // We already have a stronger relation in place,
                // so we can ignore the WEAK_DEPENDENCY.
                if (newRelation == Relation::WEAK_DEPENDENCY)
                {
                    continue;
                }

                // There's a strong dependency in both direction, which prevents us from creating a graph.
                if (currentRelation != Relation::WEAK_DEPENDENCY)
                {
                    // TODO: Make this error clear.
                    throw std::runtime_error("Functions depend on each other");
                }
            }

            // Relation changed, we should update it.
            currentRelation = newRelation;
        }

        if (currentRelation == Relation::NONE)
        {
            return;
        }

        if (currentRelation == Relation::PARENT)
        {
            lhs.AddParent(&rhs);
        }
        else if (currentRelation == Relation::CHILD)
        {
            rhs.AddParent(&lhs);
        }
        else
        {
            lhs.AddWeakDependency(&rhs);
        }
    }

    void Scheduler::ResolveDependencies(std::span<SystemFunction> functions)
    {
        for (auto& function : functions)
        {
            function.ResolveWeakDependencies();
        }

        for (auto& function : functions)
        {
            function.SimplifyDependencies();
        }
    }

    void Scheduler::SetUpStarterFunctions(std::vector<SystemFunction*>& functions)
    {
        std::size_t i = 0;
        while (i != functions.size())
        {
            auto* starterFn = functions[i];
            if (starterFn->m_parents.empty())
            {
                ++i;
                continue;
            }

            functions[i] = functions.back();
            functions.pop_back();
        }

        if (functions.empty())
        {
            throw std::runtime_error("No started functions!");
        }
    }

    void Scheduler::VerifyGraphIsDAG(std::span<const SystemFunction> functions)
    {
        // Kahn's algorithm implementation

        const auto graphSize = functions.size();

        std::vector<std::size_t> parents_per_function(graphSize, 0u);
        std::vector<const SystemFunction*> startNodes;

        for (std::size_t i = 0; i < graphSize; ++i)
        {
            const auto& function = functions[i];

            const auto parentsCount = function.m_parents.size();
            if (parentsCount == 0)
            {
                startNodes.push_back(&functions[i]);
            }
            else
            {
                parents_per_function[function.m_id.Raw()] = parentsCount;
            }
        }

        std::size_t output_functions_count = 0;
        while (!startNodes.empty())
        {
            const auto* startNode = startNodes.back();
            startNodes.pop_back();
            ++output_functions_count;

            for (const auto& function : startNode->m_children)
            {
                const auto functionIndex = function->m_id.Raw();
                --parents_per_function[functionIndex];

                if (parents_per_function[functionIndex] == 0)
                {
                    startNodes.push_back(function);
                }
            }
        }

        // We don't really care about the final graph.
        // We only care whether all functions have their parents removed, which we know by storing a count.
        // This way we can save memory and cpu time.
        if (output_functions_count != graphSize)
        {
            throw std::runtime_error("NOT A DAG");
        }
    }
} // namespace be
