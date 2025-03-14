// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_EXTRAINFO_MNN_H_
#define FLATBUFFERS_GENERATED_EXTRAINFO_MNN_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 25 &&
              FLATBUFFERS_VERSION_MINOR == 2 &&
              FLATBUFFERS_VERSION_REVISION == 10,
             "Non-compatible flatbuffers version included");

#include "Tensor_generated.h"

namespace MNN {

struct ExtraInfo;
struct ExtraInfoBuilder;

struct ExtraInfo FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ExtraInfoBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_BUFFER = 4,
    VT_NAME = 6,
    VT_VERSION = 8
  };
  const ::flatbuffers::Vector<int8_t> *buffer() const {
    return GetPointer<const ::flatbuffers::Vector<int8_t> *>(VT_BUFFER);
  }
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  const ::flatbuffers::String *version() const {
    return GetPointer<const ::flatbuffers::String *>(VT_VERSION);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_BUFFER) &&
           verifier.VerifyVector(buffer()) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_VERSION) &&
           verifier.VerifyString(version()) &&
           verifier.EndTable();
  }
};

struct ExtraInfoBuilder {
  typedef ExtraInfo Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_buffer(::flatbuffers::Offset<::flatbuffers::Vector<int8_t>> buffer) {
    fbb_.AddOffset(ExtraInfo::VT_BUFFER, buffer);
  }
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(ExtraInfo::VT_NAME, name);
  }
  void add_version(::flatbuffers::Offset<::flatbuffers::String> version) {
    fbb_.AddOffset(ExtraInfo::VT_VERSION, version);
  }
  explicit ExtraInfoBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<ExtraInfo> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<ExtraInfo>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<ExtraInfo> CreateExtraInfo(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<int8_t>> buffer = 0,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    ::flatbuffers::Offset<::flatbuffers::String> version = 0) {
  ExtraInfoBuilder builder_(_fbb);
  builder_.add_version(version);
  builder_.add_name(name);
  builder_.add_buffer(buffer);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<ExtraInfo> CreateExtraInfoDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<int8_t> *buffer = nullptr,
    const char *name = nullptr,
    const char *version = nullptr) {
  auto buffer__ = buffer ? _fbb.CreateVector<int8_t>(*buffer) : 0;
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto version__ = version ? _fbb.CreateString(version) : 0;
  return MNN::CreateExtraInfo(
      _fbb,
      buffer__,
      name__,
      version__);
}

}  // namespace MNN

#endif  // FLATBUFFERS_GENERATED_EXTRAINFO_MNN_H_
