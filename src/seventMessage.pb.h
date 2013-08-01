// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: seventMessage.proto

#ifndef PROTOBUF_seventMessage_2eproto__INCLUDED
#define PROTOBUF_seventMessage_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_seventMessage_2eproto();
void protobuf_AssignDesc_seventMessage_2eproto();
void protobuf_ShutdownFile_seventMessage_2eproto();

class seventMessage;

// ===================================================================

class seventMessage : public ::google::protobuf::Message {
 public:
  seventMessage();
  virtual ~seventMessage();

  seventMessage(const seventMessage& from);

  inline seventMessage& operator=(const seventMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const seventMessage& default_instance();

  void Swap(seventMessage* other);

  // implements Message ----------------------------------------------

  seventMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const seventMessage& from);
  void MergeFrom(const seventMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required bytes str = 1;
  inline bool has_str() const;
  inline void clear_str();
  static const int kStrFieldNumber = 1;
  inline const ::std::string& str() const;
  inline void set_str(const ::std::string& value);
  inline void set_str(const char* value);
  inline void set_str(const void* value, size_t size);
  inline ::std::string* mutable_str();
  inline ::std::string* release_str();
  inline void set_allocated_str(::std::string* str);

  // @@protoc_insertion_point(class_scope:seventMessage)
 private:
  inline void set_has_str();
  inline void clear_has_str();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* str_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_seventMessage_2eproto();
  friend void protobuf_AssignDesc_seventMessage_2eproto();
  friend void protobuf_ShutdownFile_seventMessage_2eproto();

  void InitAsDefaultInstance();
  static seventMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// seventMessage

// required bytes str = 1;
inline bool seventMessage::has_str() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void seventMessage::set_has_str() {
  _has_bits_[0] |= 0x00000001u;
}
inline void seventMessage::clear_has_str() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void seventMessage::clear_str() {
  if (str_ != &::google::protobuf::internal::kEmptyString) {
    str_->clear();
  }
  clear_has_str();
}
inline const ::std::string& seventMessage::str() const {
  return *str_;
}
inline void seventMessage::set_str(const ::std::string& value) {
  set_has_str();
  if (str_ == &::google::protobuf::internal::kEmptyString) {
    str_ = new ::std::string;
  }
  str_->assign(value);
}
inline void seventMessage::set_str(const char* value) {
  set_has_str();
  if (str_ == &::google::protobuf::internal::kEmptyString) {
    str_ = new ::std::string;
  }
  str_->assign(value);
}
inline void seventMessage::set_str(const void* value, size_t size) {
  set_has_str();
  if (str_ == &::google::protobuf::internal::kEmptyString) {
    str_ = new ::std::string;
  }
  str_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* seventMessage::mutable_str() {
  set_has_str();
  if (str_ == &::google::protobuf::internal::kEmptyString) {
    str_ = new ::std::string;
  }
  return str_;
}
inline ::std::string* seventMessage::release_str() {
  clear_has_str();
  if (str_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = str_;
    str_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void seventMessage::set_allocated_str(::std::string* str) {
  if (str_ != &::google::protobuf::internal::kEmptyString) {
    delete str_;
  }
  if (str) {
    set_has_str();
    str_ = str;
  } else {
    clear_has_str();
    str_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_seventMessage_2eproto__INCLUDED
