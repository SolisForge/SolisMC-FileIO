// ============================================================================
// Project: SOLISMC-FILEIO
//
// Adapater for the test C++ struct
//
// Author    Meltwin (github@meltwin.fr)
// Date      12/07/2026 (created 12/07/2026)
// Version   1.0.0
// Copyright Solis Forge | 2026
//           Distributed under MIT License (https://opensource.org/licenses/MIT)
// ============================================================================
#include "minecraft/io/nbt/bytes/field_value.hpp"
#include "minecraft/io/nbt/bytes/struct.hpp"
#include "object.hpp"
#include <cstring>
#include <map>
#include <memory>

using namespace minecraft::nbt::byte;

struct TestObjectWriteAdapter : WriteAdapter {

  TestObjectWriteAdapter(const std::shared_ptr<TestObject> &ptr) : value(ptr) {}

  static const std::map<std::string, minecraft::nbt::Tag> fields;

  FieldState is_field(const FieldInfo &info) const noexcept override {
    if (auto it = fields.find(info.name); it == fields.end())
      return FieldState::DO_NOT_EXIST;
    else if (it->second != info.tag)
      return FieldState::WRONG_TYPE;
    return FieldState::EXIST;
  }

  void set_field(const FieldInfo &info, const FieldValue &v) override {

    if (info.name.compare("name") == 0) {
      value->name = std::string{v.as<std::string>()};
      return;
    } else if (info.name.compare("foo") == 0) {
      value->foo = v.as<int>();
      return;
    }
  }

protected:
  std::shared_ptr<TestObject> value;
};