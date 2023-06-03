#ifndef OPENDRIVE_CPP_COMMON_MACROS_H_
#define OPENDRIVE_CPP_COMMON_MACROS_H_
#include <iostream>
#include <memory>

namespace opendrive {

#define REGISTER_MEMBER_BASIC_TYPE(type, var, value) \
 private:                                            \
  type var##_ = value;                               \
                                                     \
 public:                                             \
  type var() const noexcept { return var##_; }       \
  void set_##var(const type var) noexcept { var##_ = var; }

#define REGISTER_MEMBER_COMPLEX_TYPE(type, var)              \
 private:                                                    \
  type var##_;                                               \
                                                             \
 public:                                                     \
  const type& var() const noexcept { return var##_; }        \
  type* mutable_##var() noexcept { return &var##_; }         \
  void set_##var(const type& var) noexcept { var##_ = var; } \
  void set_##var(type&& var) noexcept { var##_ = std::move(var); }

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_COMMON_MACROS_H_
