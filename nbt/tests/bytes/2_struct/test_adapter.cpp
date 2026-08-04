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

using namespace minecraft::nbt::byte::base;

const std::map<std::string, minecraft::nbt::Tag>
    TestObjectWriteAdapter::fields = {{"name", minecraft::nbt::Tag::STRING},
                                      {"foo", minecraft::nbt::Tag::INT}};

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

TEST_CASE("WriteAdapter::set_field") {
  auto obj = std::make_shared<TestObject>();
  TestObjectWriteAdapter adapt{obj};

  SUBCASE("Setting int value") {
    FieldInfo info{"foo", Tag::INT};
    FieldValue value{Tag::INT, 10};

    CHECK_EQ(obj->foo, 0);
    adapt.set_field(info, value);
    CHECK_EQ(obj->foo, 10);
  }
  SUBCASE("Setting value of wrong type") {
    FieldInfo info{"foo", Tag::INT};
    FieldValue value{Tag::INT, std::string{}};

    CHECK_EQ(obj->foo, 0);
    CHECK_THROWS(adapt.set_field(info, value));
  }
  SUBCASE("Setting string value") {
    FieldInfo info{"name", Tag::STRING};
    std::string new_value{"test object"};
    FieldValue value{Tag::STRING, new_value};

    CHECK_EQ(obj->name.size(), 0);
    adapt.set_field(info, value);
    CHECK_EQ(obj->name.size(), new_value.size());
    CHECK_EQ(new_value.compare(obj->name), 0);
  }
}

} // namespace minecraft::nbt