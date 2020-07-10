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
template<typename T, size_t N>
T Vector<T,N>::length() const noexcept 
{
  auto ret = std::accumulate(begin(), end(), T {},
      [] (const auto& a, const auto& b) {
        return a + b * b;
      });
  return std::sqrt(ret);
}

template<typename T, size_t N>
Vector<T, N>& Vector<T,N>::normalize() noexcept 
{
  auto len = length();
  if (len > 0) {
    auto inv_len = 1 / length();
    std::for_each(begin(), end(), [inv_len](auto& x) { x *= inv_len; });  
  }
  return *this;
}

template<typename T, size_t N>
constexpr auto Vector<T,N>::begin() noexcept -> iterator
{
  auto ptr = reinterpret_cast<T*>(this); 
  return {ptr};
}

template<typename T, size_t N>
constexpr auto Vector<T,N>::end() noexcept -> iterator
{
  auto ptr = reinterpret_cast<T*>(this); 
  return {ptr + N};
}

template<typename T, size_t N>
constexpr auto Vector<T,N>::begin() const  noexcept -> const_iterator
{
  auto ptr = reinterpret_cast<const T*>(this); 
  return {ptr};
}

template<typename T, size_t N>
constexpr auto Vector<T,N>::end() const noexcept -> const_iterator
{
  auto ptr = reinterpret_cast<const T*>(this); 
  return {ptr + N};
}

template<typename T, size_t N>
constexpr auto Vector<T,N>::cbegin() const noexcept -> const_iterator
{
  auto ptr = reinterpret_cast<T*>(this); 
  return {ptr};
}

template<typename T, size_t N>
constexpr auto Vector<T,N>::cend() const noexcept -> const_iterator
{
  auto ptr = reinterpret_cast<T*>(this); 
  return {ptr + N};
}


template<typename T, int N>
Vector<T, N>& operator+=(Vector<T, N>& lhs, const Vector<T, N>& rhs)
{
  std::transform(lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(),
      [] (auto a, auto b) {
        return a + b;
      });
  return lhs;
}

template<typename T, int N>
Vector<T, N> operator+(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
{
  auto ret = lhs; 
  return ret+=rhs;
}

template<typename T, int N>
Vector<T, N>& operator-=(Vector<T, N>& lhs, const Vector<T, N>& rhs)
{
  std::transform(lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(),
      [] (auto a, auto b) {
        return a - b;
      });
  return lhs;
}

template<typename T, int N>
Vector<T, N> operator-(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
{
  auto ret = lhs; 
  ret -= rhs;
  return ret;
}

template<typename T, int N>
Vector<T, N> operator*(const Vector<T, N>& lhs, T rhs)
{
  auto tmp = lhs;
  std::for_each(tmp.begin(), tmp.end(), [rhs] (auto& a) {
        a *= rhs;
      });
  return tmp;
}

template<typename T, int N>
Vector<T, N> operator*(T rhs, const Vector<T, N>& lhs)
{
  auto tmp = lhs;
  std::for_each(tmp.begin(), tmp.end(), [rhs] (auto& a) {
        a *= rhs;
      });
  return tmp;
}

template<typename T, int N>
Vector<T, N>& operator*(Vector<T, N>& lhs, T rhs)
{
  lhs = lhs * rhs;
  return lhs;
}

template<typename T, int N>
T dot(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
{
  return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), T {});
}

template<typename T, int N>
Vector<T, N> cross(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
{
  static_assert(N == 3);
  Vector<T, N> result = {};
  result.x = lhs.y * rhs.z - lhs.z * rhs.y;
  result.y = lhs.z * rhs.x - lhs.x * rhs.z;
  result.z = lhs.x * rhs.y - lhs.y * rhs.x;
  return result;
}


