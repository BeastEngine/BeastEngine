#pragma once
#include <Beast/Common/Types.h>
#include <Beast/Common/Helpers.h>

namespace be
{
    /**
     * @brief Interface providing generation of the universally unique identifiers (UUIDs) in the form of an integer.
     */
    class IUuIdGenerator
    {
    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IUuIdGenerator);
        IUuIdGenerator() = default;
        virtual ~IUuIdGenerator() = default;

        /**
         * @brief Generates universally unique identifier.
         * 
         * @return New, unique Id
         */
        [[nodiscard]] virtual Id Generate() const = 0;
    };
} // namespace be
