#pragma once

#include <algorithm>

namespace Utils {

template<class Callable>
class function_output_iterator
{
public:
    typedef std::output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    explicit function_output_iterator() {}

    explicit function_output_iterator(const Callable &callable)
        : m_callable(&callable)
    {}

    function_output_iterator &operator=(const function_output_iterator &iterator)
    {
        m_callable = iterator.m_callable;

        return *this;
    }

    struct helper
    {
        helper(const Callable *callable)
            : m_callable(callable)
        {}
        template<class T>
        helper &operator=(T &&value)
        {
            (*m_callable)(std::forward<T>(value));
            return *this;
        }
        const Callable *m_callable;
    };

    helper operator*() { return helper(m_callable); }
    function_output_iterator &operator++() { return *this; }
    function_output_iterator &operator++(int) { return *this; }

private:
    const Callable *m_callable;
};

template<typename Callable>
function_output_iterator<Callable> make_iterator(const Callable &callable)
{
    return function_output_iterator<Callable>(callable);
}

template<class InputIt1, class InputIt2, class Callable, class Compare>
bool set_intersection_compare(
    InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Callable call, Compare comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            ++first1;
        } else {
            if (!comp(*first2, *first1)) {
                if (call(*first1++, *first2))
                    return true;
            }
            ++first2;
        }
    }

    return false;
}

template<typename InputIt1, typename InputIt2, typename BinaryPredicate, typename Callable, typename Value>
Value mismatch_collect(InputIt1 first1,
                       InputIt1 last1,
                       InputIt2 first2,
                       InputIt2 last2,
                       Value value,
                       BinaryPredicate predicate,
                       Callable callable)
{
    while (first1 != last1 && first2 != last2) {
        if (predicate(*first1, *first2))
            value = callable(*first1, *first2, value);
        ++first1, ++first2;
    }

    return value;
}

} // namespace Utils
