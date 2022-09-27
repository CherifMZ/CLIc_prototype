#ifndef __CORE_CLESCALAR_HPP
#define __CORE_CLESCALAR_HPP

#include "cleLightObject.hpp"

#include <sstream>

namespace cle
{

template <class Type>
class Scalar : public LightObject
{
public:
  Scalar() = delete;
  ~Scalar() = default;
  Scalar(const Scalar & obj) noexcept = default;
  Scalar(Scalar && obj) = default;
  auto
  operator=(const Scalar & obj) -> Scalar & = default;
  auto
  operator=(Scalar && obj) noexcept -> Scalar & = default;
  explicit Scalar(const Type & data);

  [[nodiscard]] auto
  Get() const -> Type;
  [[nodiscard]] auto
  Ndim() const -> unsigned int override;
  [[nodiscard]] auto
  Shape() const -> ShapeArray override;
  [[nodiscard]] auto
  ObjectInfo() const -> std::string override;
  [[nodiscard]] auto
  Object() const -> ObjectType override;
  [[nodiscard]] auto
  DataInfo() const -> std::string override;
  [[nodiscard]] auto
  DataInfoShort() const -> std::string override;
  [[nodiscard]] auto
  Data() const -> DataType override;
  [[nodiscard]] auto
  Bytes() const -> size_t override;
  [[nodiscard]] auto
  ToString() const -> std::string override;

  template <class T>
  friend auto
  operator<<(std::ostream & out, const Scalar<T> & scalar) -> std::ostream &;

private:
  Type     data_;
  DataType data_type_;
};

template <class Type>
Scalar<Type>::Scalar(const Type & data)
  : data_(data)
  , data_type_(TypeToDataType<Type>())
{
  static_assert(std::is_fundamental<Type>::value, "Scalar can only be of native type");
}

template <class Type>
auto
Scalar<Type>::Get() const -> Type
{
  return this->data_;
}

template <class Type>
auto
Scalar<Type>::Ndim() const -> unsigned int
{
  return 1;
}

template <class Type>
auto
Scalar<Type>::Shape() const -> ShapeArray
{
  return { 1, 1, 1 };
}

template <class Type>
auto
Scalar<Type>::ObjectInfo() const -> std::string
{
  return "scalar";
}

template <class Type>
auto
Scalar<Type>::Object() const -> ObjectType
{
  return SCALAR;
}

template <class Type>
auto
Scalar<Type>::DataInfo() const -> std::string
{
  return DataTypeToString(this->Data());
}

template <class Type>
auto
Scalar<Type>::DataInfoShort() const -> std::string
{
  return typeid(this->Get()).name();
}

template <class Type>
auto
Scalar<Type>::Data() const -> DataType
{
  return this->data_type_;
}

template <class Type>
auto
Scalar<Type>::Bytes() const -> size_t
{
  return sizeof(this->Get());
}

template <class Type>
auto
Scalar<Type>::ToString() const -> std::string
{
  std::stringstream out_string;
  out_string << this->Get() << "(" << this->DataInfoShort() << ")";
  return out_string.str();
}

template <class Type>
auto
operator<<(std::ostream & out, const Scalar<Type> & scalar) -> std::ostream &
{
  out << scalar.ToString();
  return out;
}

} // namespace cle

#endif // __CORE_CLESCALAR_HPP
