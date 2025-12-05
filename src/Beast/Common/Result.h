#pragma once
#include "Beast/Debug.h"

#include <variant>
#include <type_traits>
#include <string>

namespace be
{
    struct ResultError
    {
        const std::string message = "";
    };

    template<typename T>
    concept NotResultError = (!std::is_same_v<T, ResultError>) && (!std::is_convertible_v<T, std::string>) && (!std::is_convertible_v<T, std::string_view>);

    /**
     * @brief Utility class serving a similar purpose to the one of the https://en.cppreference.com/w/cpp/utility/expected.
     * It gives a possibility of capturing the expected result of an operation or an error.
     * 
     * @tparam T Type of the expected successful value. Cannot be the same as \c ResultError.
     */
    template<NotResultError T>
    class [[nodiscard]] Result
    {
    public:
        Result(T&& value)
            : m_data(std::move(value))
        {
        }

        Result(const T& value)
            : m_data(value)
        {
        }

        Result(ResultError&& error)
            : m_data(std::move(error))
        {
        }

        Result(std::string error)
            : m_data(ResultError{.message = std::move(error)})
        {
        }

        /**
         * @brief Returns a reference to the value held by the result.
         * Throws if value is not present (result was created with an error).
         */
        [[nodiscard]] const T& Value() const
        {
            return std::get<0>(m_data);
        }

        /**
         * @brief Moves the value held by the result, leaving the value in a moved-from state.
         * Accessing the value after this call is prohibited.
         * Throws if value is not present (result was created with an error).
         */
        [[nodiscard]] T&& TakeValue()
        {
            return std::move(std::get<0>(m_data));
        }

        /**
         * @brief Returns a reference to the error held by the result.
         * Throws if error is not present (result was created with a value).
         */
        [[nodiscard]] const ResultError& Error() const
        {
            return std::get<1>(m_data);
        }

        [[nodiscard]] operator bool() const
        {
            return std::get_if<0>(&m_data) != nullptr;
        }

        [[nodiscard]] constexpr const T* operator->() const
        {
            return &std::get<0>(m_data);
        }

    private:
        std::variant<T, ResultError> m_data;
    };
} // namespace be