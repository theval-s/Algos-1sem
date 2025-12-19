#pragma once
#include <cassert>
#include <memory>
#include <cmath>

//constraints: T must me move/copy assignable and move/copy constructible?
template<typename T>
class Array final
{
public: //functions required by task

    //Constructors
    Array(int capacity)
        : m_capacity(capacity)
        , m_data(nullptr)
    {
        allocate(m_capacity);
    }
    Array() : Array(DEFAULT_CAPACITY) {}

    ~Array()
    {
        annihilate();
    }

    Array(const Array& other)
        : m_data(nullptr)
        , m_size(other.m_size)
        , m_capacity(other.m_capacity)
    {
        allocate(m_capacity);
        std::uninitialized_copy(other.m_data, other.m_data + other.m_size, m_data);
    }

    Array(Array&& other) noexcept
        : m_data(std::exchange(other.m_data, nullptr))
        , m_size(std::exchange(other.m_size, 0))
        , m_capacity(std::exchange(other.m_capacity, 0))
    {
    }

    //Assignment op
    Array& operator=(const Array& other)
    {
        if (this == &other) return *this;

        Array temp(other);
        std::swap(m_data, temp.m_data);
        std::swap(m_size, temp.m_size);
        std::swap(m_capacity, temp.m_capacity);
        return *this;
        //temp destroyed
    }

    Array& operator=(Array&& other) noexcept
    {
        if (this == &other) return *this;

        annihilate();
        m_data = std::exchange(other.m_data, nullptr);
        m_size = std::exchange(other.m_size, 0);
        m_capacity = std::exchange(other.m_capacity, 0);
        return *this;
    }

    //API functions
    int insert(const T& value)
    {
        if (m_size == m_capacity) grow();

        std::construct_at(m_data+m_size, value);
        m_size++;

        return m_size-1;
    }
    int insert(int index, const T& value)
    {
        if (m_size == m_capacity) grow();

        shiftElementsRight(index);
        m_data[index] = value;

        return index;
    }
    void remove(int index)
    {
        assert(index >= 0 && index < m_size);
        shiftElementsLeft(index+1);
        std::destroy_at(&m_data[m_size-1]);
        m_size-=1;
    }
    int size() const
    {
        return m_size;
    }

    //Operators
    const T& operator[](int index) const
    {
        assert(index >= 0 && index < m_size);
        return m_data[index];
    }
    T& operator[](int index)
    {
        assert(index >= 0 && index < m_size);
        return m_data[index];
    }

public: //Iterators
    class Iterator
    {
        public:
        Iterator(Array& arr, int index = 0, bool rIter = false);
        T& get() const;
        void set(const T& value);
        void next();
        bool hasNext() const;

        //Additional functions
        Iterator& operator++();
        T& operator*() const;
        bool operator!=(const Iterator& other) const;

        private:
        Array& m_array;
        int m_index{0};
        bool m_reverse{false};
    };
    class ConstIterator
    {
        public:
        ConstIterator(const Array& arr, int index = 0, bool rIter = false);
        const T& get() const;
        void next();
        bool hasNext() const;

        //Additional functions
        ConstIterator& operator++();
        const T& operator*() const;
        bool operator!=(const ConstIterator& other) const;

        private:
        const Array& m_array;
        int m_index;
        bool m_reverse;
    };

    Iterator iterator()
    {
        return Iterator(*this);
    }
    ConstIterator iterator() const
    {
        return ConstIterator(*this);
    }
    Iterator reverseIterator()
    {
        return Iterator(*this, m_size - 1, true);
    }
    ConstIterator reverseIterator() const
    {
        return ConstIterator(*this, m_size - 1, true);
    }


private:
    void annihilate()
    {
        if (m_data)
        {
            for (int i = 0; i < m_size; ++i)
            {
                std::destroy_at(m_data + i);
            }
            free(m_data);
        }
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    void grow()
    {
        int newCapacity = static_cast<int>(std::ceil(m_capacity*GROWTH_FACTOR));
        newCapacity = (newCapacity == 0) ? DEFAULT_CAPACITY : newCapacity;
        allocate(newCapacity);
    }

    void allocate(int capacity)
    {
        assert(capacity > m_size);
        if (capacity == 0) capacity = DEFAULT_CAPACITY;

        void* newPtr = malloc(capacity*sizeof(T));
        if (!newPtr) throw std::bad_alloc();
        if (ZERO_FILL) memset(newPtr, 0, capacity*sizeof(T));
        T* newData = static_cast<T*>(newPtr);

        if (m_data)
        {
            for (int i = 0; i < m_size; ++i)
            {
                std::construct_at(newData+i, std::move(m_data[i]));
                std::destroy_at(&m_data[i]);
            }
            free(m_data);
        }
        m_data = newData;
        m_capacity = capacity;
    }

    /// Assumes arr[startIndex-1] is constructed
    /// @param startIndex = the starting index for shift,
    /// as in arr[startIndex] is the first element that will be moved to arr[startIndex-1]
    void shiftElementsLeft(int startIndex)
    {
        assert(startIndex > 0 && startIndex <= m_size);
        //<= allows to remove last element because we won't even be shifting anything
        //due to loop condition being less than size
        for (int i = startIndex; i < m_size; ++i)
        {
                m_data[i-1] = std::move(m_data[i]);
        }
    }

    /// Assumes the last element is unconstructed, but allocated
    /// does not destruct m_data[startIndex]!
    /// @param startIndex = the last index for shift going from end to this index,
    /// as in arr[startIndex-1] is the last element that will be moved to arr[startIndex]
    void shiftElementsRight(int startIndex)
    {
        assert(startIndex >= 0 && startIndex < m_size && m_size < m_capacity);

        std::construct_at(m_data+m_size, std::move(m_data[m_size-1]));
        for (int i = m_size-1; i > startIndex; --i)
        {
            m_data[i] = std::move(m_data[i-1]);
        }
        m_size+=1;
    }

private:
    T* m_data;
    int m_size{0};
    int m_capacity{0};

    static constexpr int DEFAULT_CAPACITY = 8;
    static constexpr float GROWTH_FACTOR = 2.f; //1.6 .. 2 as defined by task
    static constexpr bool ZERO_FILL = false;

public: //Additional functions
    Iterator begin()
    {
        return iterator();
    }
    ConstIterator begin() const
    {
        return iterator();
    }

    ConstIterator cbegin() const
    {
        return iterator();
    }

    Iterator end()
    {
        return Iterator(*this, m_size, false);
    }
    ConstIterator end() const
    {
        return ConstIterator(*this, m_size, false);
    }
    ConstIterator cend() const
    {
        return ConstIterator(*this, m_size, false);
    }

    //for lab3
    T* beginPtr() const
    {
        return m_data;
    }
    T* endPtr() const
    {
        return m_data+m_size;
    }
};


#pragma region ITERATORS
template <typename T>
Array<T>::Iterator::Iterator(Array& arr, int index, bool rIter)
    : m_array(arr), m_index(index), m_reverse(rIter)
{

}

template <typename T>
T& Array<T>::Iterator::get() const
{
    return m_array[m_index];
}

template <typename T>
void Array<T>::Iterator::set(const T& value)
{
    m_array[m_index] = value;
}

template <typename T>
void Array<T>::Iterator::next()
{
    if (m_reverse) --m_index;
    else ++m_index;
}

template <typename T>
bool Array<T>::Iterator::hasNext() const
{
    if (m_reverse) return (m_index >= 0);
    else return (m_index < m_array.size());
}

template <typename T>
Array<T>::Iterator& Array<T>::Iterator::operator++()
{
    next();
    return *this;
}

template <typename T>
T& Array<T>::Iterator::operator*() const
{
    return get();
}

template <typename T>
bool Array<T>::Iterator::operator!=(const Iterator& other) const
{
    return &m_array != &other.m_array || m_index != other.m_index;
}

template <typename T>
Array<T>::ConstIterator::ConstIterator(const Array& arr, int index, bool rIter)
    : m_array(arr), m_index(index), m_reverse(rIter)
{

}

template <typename T>
const T& Array<T>::ConstIterator::get() const
{
    return m_array[m_index];
}

template <typename T>
void Array<T>::ConstIterator::next()
{
    if (m_reverse) --m_index;
    else ++m_index;
}

template <typename T>
bool Array<T>::ConstIterator::hasNext() const
{
    if (m_reverse) return (m_index >= 0);
    else return (m_index < m_array.size());
}

template <typename T>
Array<T>::ConstIterator& Array<T>::ConstIterator::operator++()
{
    next();
    return *this;
}

template <typename T>
const T& Array<T>::ConstIterator::operator*() const
{
    return get();
}

template <typename T>
bool Array<T>::ConstIterator::operator!=(const ConstIterator& other) const
{
    return &m_array != &other.m_array || m_index != other.m_index;
}

#pragma endregion ITERATORS


