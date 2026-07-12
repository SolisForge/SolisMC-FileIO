// ============================================================================
// Project: SOLISMC-FILEIO
//
// Unit-test for C++ struct adapter
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/07/2026 (created 12/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "adapter.hpp"
#include <doctest/doctest.h>
#include <memory>

const std::map<std::string, minecraft::nbt::Tag>
    TestObjectWriteAdapter::fields = {{"name", minecraft::nbt::Tag::STRING},
                                      {"index", minecraft::nbt::Tag::INT}};

namespace minecraft::nbt {

TEST_CASE("WriteAdapter::is_field") {
  auto obj = std::make_shared<TestObject>();
  TestObjectWriteAdapter adapt{obj};

  SUBCASE("Field existing") {
    FieldInfo field{std::string{"name"}, Tag::STRING};
    CHECK_EQ(adapt.is_field(field), FieldState::EXIST);
  }
  SUBCASE("Exist but different type") {
    FieldInfo field{std::string{"name"}, Tag::INT};
    CHECK_EQ(adapt.is_field(field), FieldState::WRONG_TYPE);
  }
  SUBCASE("Field does not exist") {
    FieldInfo field{std::string{"fdsfdsfds"}, Tag::INT};
    CHECK_EQ(adapt.is_field(field), FieldState::DO_NOT_EXIST);
  }
}

} // namespace minecraft::nbt