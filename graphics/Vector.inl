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


