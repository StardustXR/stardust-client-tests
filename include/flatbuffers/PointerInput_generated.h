// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_POINTERINPUT_STARDUSTXR_H_
#define FLATBUFFERS_GENERATED_POINTERINPUT_STARDUSTXR_H_

#include "flatbuffers/flatbuffers.h"

#include "common_generated.h"

namespace StardustXR {

struct Pointer;
struct PointerBuilder;

struct Pointer FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef PointerBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ORIGIN = 4,
    VT_DIRECTION = 6,
    VT_TILT = 8,
    VT_DEEPEST_POINT = 10,
    VT_DEEPEST_SURFACE_POINT = 12
  };
  const StardustXR::vec3 *origin() const {
    return GetStruct<const StardustXR::vec3 *>(VT_ORIGIN);
  }
  const StardustXR::vec3 *direction() const {
    return GetStruct<const StardustXR::vec3 *>(VT_DIRECTION);
  }
  float tilt() const {
    return GetField<float>(VT_TILT, 0.0f);
  }
  const StardustXR::vec3 *deepest_point() const {
    return GetStruct<const StardustXR::vec3 *>(VT_DEEPEST_POINT);
  }
  const StardustXR::vec3 *deepest_surface_point() const {
    return GetStruct<const StardustXR::vec3 *>(VT_DEEPEST_SURFACE_POINT);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<StardustXR::vec3>(verifier, VT_ORIGIN) &&
           VerifyField<StardustXR::vec3>(verifier, VT_DIRECTION) &&
           VerifyField<float>(verifier, VT_TILT) &&
           VerifyField<StardustXR::vec3>(verifier, VT_DEEPEST_POINT) &&
           VerifyField<StardustXR::vec3>(verifier, VT_DEEPEST_SURFACE_POINT) &&
           verifier.EndTable();
  }
};

struct PointerBuilder {
  typedef Pointer Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_origin(const StardustXR::vec3 *origin) {
    fbb_.AddStruct(Pointer::VT_ORIGIN, origin);
  }
  void add_direction(const StardustXR::vec3 *direction) {
    fbb_.AddStruct(Pointer::VT_DIRECTION, direction);
  }
  void add_tilt(float tilt) {
    fbb_.AddElement<float>(Pointer::VT_TILT, tilt, 0.0f);
  }
  void add_deepest_point(const StardustXR::vec3 *deepest_point) {
    fbb_.AddStruct(Pointer::VT_DEEPEST_POINT, deepest_point);
  }
  void add_deepest_surface_point(const StardustXR::vec3 *deepest_surface_point) {
    fbb_.AddStruct(Pointer::VT_DEEPEST_SURFACE_POINT, deepest_surface_point);
  }
  explicit PointerBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Pointer> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Pointer>(end);
    return o;
  }
};

inline flatbuffers::Offset<Pointer> CreatePointer(
    flatbuffers::FlatBufferBuilder &_fbb,
    const StardustXR::vec3 *origin = 0,
    const StardustXR::vec3 *direction = 0,
    float tilt = 0.0f,
    const StardustXR::vec3 *deepest_point = 0,
    const StardustXR::vec3 *deepest_surface_point = 0) {
  PointerBuilder builder_(_fbb);
  builder_.add_deepest_surface_point(deepest_surface_point);
  builder_.add_deepest_point(deepest_point);
  builder_.add_tilt(tilt);
  builder_.add_direction(direction);
  builder_.add_origin(origin);
  return builder_.Finish();
}

}  // namespace StardustXR

#endif  // FLATBUFFERS_GENERATED_POINTERINPUT_STARDUSTXR_H_
