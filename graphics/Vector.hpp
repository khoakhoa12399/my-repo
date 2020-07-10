#ifndef _SAITOU_VECTOR_HPP
#define _SAITOU_VECTOR_HPP

#include <array>
#include <numeric>
#include <algorithm>

namespace saitou {

  namespace graphics {

    namespace detail {

      template<typename T, size_t N>
      struct VectorMember {
        std::array<T, N> data;
      };

      template<typename T>
      struct VectorMember<T, 1> {
        T x;
      };

      template<typename T>
      struct VectorMember<T, 2> {
        T x;
        T y;
      };

      template<typename T>
      struct VectorMember<T, 3> {
        T x;
        T y;
        T z; 
      };

      template<typename T>
      struct VectorMember<T, 4> {
        T x;
        T y;
        T z; 
        T t;
      };

      template<typename T>
      struct VectorIterator {
        public:
          using iterator_category = std::random_access_iterator_tag;
          using value_type = T;
          using difference_type = std::ptrdiff_t;
          using pointer = T*;
          using reference = T&;
          using const_reference = const T&;

          VectorIterator(T* ptr = nullptr) 
          {
            m_ptr = ptr;
          }
          VectorIterator(const VectorIterator<T>& re) = default;

          VectorIterator<T>& operator=(const VectorIterator<T>& re) = default;

          VectorIterator<T>& operator=(T& re)
          {
            m_ptr = re;
          }

          operator bool() const
          {
            if (m_ptr) return true;
            else return false;
          }

          VectorIterator<T>& operator+=(const difference_type& dif) 
          {
            m_ptr += dif;
            return *this;
          }

          VectorIterator<T>& operator-=(const difference_type& dif) 
          {
            m_ptr -= dif;
            return *this;
          }

          VectorIterator<T>& operator++() 
          {
            ++m_ptr;
            return *this;
          }

          VectorIterator<T>& operator--()
          {
            --m_ptr;
            return *this;
          }

          VectorIterator<T> operator++(int)
          {
            auto tmp = *this;
            ++m_ptr;
            return tmp;
          }

          VectorIterator<T> operator--(int)
          {
            auto tmp = *this;
            --m_ptr;
            return tmp;
          }

          VectorIterator<T> operator+(const difference_type& dif)
          {
            return VectorIterator<T> {m_ptr + dif}; 
          }

          VectorIterator<T> operator-(const difference_type& dif)
          {
            return VectorIterator<T> {m_ptr - dif};
          }

          difference_type operator-(const VectorIterator<T>& rhs)
          {
            return m_ptr - rhs.m_ptr;
          }

          reference operator*()
          {
            return *m_ptr;
          }

          const_reference operator*() const
          {
            return *m_ptr;
          }

          pointer operator->()
          {
            return m_ptr;
          }
        private:
          pointer m_ptr;
      };
    }

    /* An static custom type of Vector to with enough operator
     * overloading to work with graphics, only work in * c++20. 
     *
     * Base on template argument N, the Vector has coressponding
     * member x, y, z, t. For example: 
     *
     *      Vector<int, 2> has member x, y, not z
     *      Vector<int, 3> has member x, y, z.
     *      Vector<int, 5> has int[5], and x, y,... are disabled
     *
     * Vector supports operator [] for array-like accessing. You can access
     * Vector<..., 5> using this 
     *
     * Also, Vector supports iterator for some usage in range for loop and
     * other algorithm
     */
    template<typename T, size_t N>
    struct Vector: detail::VectorMember<T, N> {

      using iterator = detail::VectorIterator<T>;
      using const_iterator = detail::VectorIterator<const T>;
      using reference = T&;
      using const_reference = const T&;
      using size_type = size_t;

      T length() const noexcept;
      Vector<T, N>& normalize() noexcept;

      constexpr reference operator[](size_type pos);
      constexpr const_reference operator[](size_type pos) const;

      constexpr iterator begin() noexcept;
      constexpr iterator end() noexcept;

      constexpr const_iterator begin() const noexcept;
      constexpr const_iterator end() const noexcept;

      constexpr const_iterator cbegin() const noexcept;
      constexpr const_iterator cend() const noexcept;
    };

    using Vec2i = Vector<int, 2>;
    using Vec3i = Vector<int, 3>;
    using Vec2f = Vector<float, 2>;
    using Vec3f = Vector<float, 3>;

    template<typename T, int N>
    Vector<T, N>& operator+=(Vector<T, N>& lhs, const Vector<T, N>& rhs);

    template<typename T, int N>
    Vector<T, N> operator+(const Vector<T, N>& lhs, const Vector<T, N>& rhs);

    template<typename T, int N>
    Vector<T, N>& operator-=(Vector<T, N>& lhs, const Vector<T, N>& rhs);

    template<typename T, int N>
    Vector<T, N> operator-(const Vector<T, N>& lhs, const Vector<T, N>& rhs);

    template<typename T, int N>
    Vector<T, N>& operator*=(Vector<T, N>& lhs, T rhs);

    template<typename T, int N>
    Vector<T, N> operator*(const Vector<T, N>& lhs, T rhs);

    template<typename T, int N>
    Vector<T, N> operator*(T lsh, const Vector<T, N>& rhs);

    template<typename T, int N>
    T dot(const Vector<T, N>& lsh, const Vector<T, N>& rhs);

    template<typename T, int N>
    Vector<T, N> cross(const Vector<T, N>& lsh, const Vector<T, N>& rhs);

    template<typename T, int N>
    Vector<T, N> normalize(const Vector<T, N>& v);
    
#include "Vector.inl"
  }
}
  

#endif
