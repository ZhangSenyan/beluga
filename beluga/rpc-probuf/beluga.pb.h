// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: beluga.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_beluga_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_beluga_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3008000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3008000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_beluga_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_beluga_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_beluga_2eproto;
namespace beluga {
class RPCMsg;
class RPCMsgDefaultTypeInternal;
extern RPCMsgDefaultTypeInternal _RPCMsg_default_instance_;
}  // namespace beluga
PROTOBUF_NAMESPACE_OPEN
template<> ::beluga::RPCMsg* Arena::CreateMaybeMessage<::beluga::RPCMsg>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace beluga {

// ===================================================================

class RPCMsg :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:beluga.RPCMsg) */ {
 public:
  RPCMsg();
  virtual ~RPCMsg();

  RPCMsg(const RPCMsg& from);
  RPCMsg(RPCMsg&& from) noexcept
    : RPCMsg() {
    *this = ::std::move(from);
  }

  inline RPCMsg& operator=(const RPCMsg& from) {
    CopyFrom(from);
    return *this;
  }
  inline RPCMsg& operator=(RPCMsg&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const RPCMsg& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const RPCMsg* internal_default_instance() {
    return reinterpret_cast<const RPCMsg*>(
               &_RPCMsg_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(RPCMsg& a, RPCMsg& b) {
    a.Swap(&b);
  }
  inline void Swap(RPCMsg* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline RPCMsg* New() const final {
    return CreateMaybeMessage<RPCMsg>(nullptr);
  }

  RPCMsg* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<RPCMsg>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const RPCMsg& from);
  void MergeFrom(const RPCMsg& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RPCMsg* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "beluga.RPCMsg";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_beluga_2eproto);
    return ::descriptor_table_beluga_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kServiceNameFieldNumber = 1,
    kMethodNameFieldNumber = 2,
    kRequestDataFieldNumber = 3,
  };
  // string service_name = 1;
  void clear_service_name();
  const std::string& service_name() const;
  void set_service_name(const std::string& value);
  void set_service_name(std::string&& value);
  void set_service_name(const char* value);
  void set_service_name(const char* value, size_t size);
  std::string* mutable_service_name();
  std::string* release_service_name();
  void set_allocated_service_name(std::string* service_name);

  // string method_name = 2;
  void clear_method_name();
  const std::string& method_name() const;
  void set_method_name(const std::string& value);
  void set_method_name(std::string&& value);
  void set_method_name(const char* value);
  void set_method_name(const char* value, size_t size);
  std::string* mutable_method_name();
  std::string* release_method_name();
  void set_allocated_method_name(std::string* method_name);

  // string request_data = 3;
  void clear_request_data();
  const std::string& request_data() const;
  void set_request_data(const std::string& value);
  void set_request_data(std::string&& value);
  void set_request_data(const char* value);
  void set_request_data(const char* value, size_t size);
  std::string* mutable_request_data();
  std::string* release_request_data();
  void set_allocated_request_data(std::string* request_data);

  // @@protoc_insertion_point(class_scope:beluga.RPCMsg)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr service_name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr method_name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr request_data_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_beluga_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RPCMsg

// string service_name = 1;
inline void RPCMsg::clear_service_name() {
  service_name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& RPCMsg::service_name() const {
  // @@protoc_insertion_point(field_get:beluga.RPCMsg.service_name)
  return service_name_.GetNoArena();
}
inline void RPCMsg::set_service_name(const std::string& value) {
  
  service_name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:beluga.RPCMsg.service_name)
}
inline void RPCMsg::set_service_name(std::string&& value) {
  
  service_name_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:beluga.RPCMsg.service_name)
}
inline void RPCMsg::set_service_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  service_name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:beluga.RPCMsg.service_name)
}
inline void RPCMsg::set_service_name(const char* value, size_t size) {
  
  service_name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:beluga.RPCMsg.service_name)
}
inline std::string* RPCMsg::mutable_service_name() {
  
  // @@protoc_insertion_point(field_mutable:beluga.RPCMsg.service_name)
  return service_name_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* RPCMsg::release_service_name() {
  // @@protoc_insertion_point(field_release:beluga.RPCMsg.service_name)
  
  return service_name_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void RPCMsg::set_allocated_service_name(std::string* service_name) {
  if (service_name != nullptr) {
    
  } else {
    
  }
  service_name_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), service_name);
  // @@protoc_insertion_point(field_set_allocated:beluga.RPCMsg.service_name)
}

// string method_name = 2;
inline void RPCMsg::clear_method_name() {
  method_name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& RPCMsg::method_name() const {
  // @@protoc_insertion_point(field_get:beluga.RPCMsg.method_name)
  return method_name_.GetNoArena();
}
inline void RPCMsg::set_method_name(const std::string& value) {
  
  method_name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:beluga.RPCMsg.method_name)
}
inline void RPCMsg::set_method_name(std::string&& value) {
  
  method_name_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:beluga.RPCMsg.method_name)
}
inline void RPCMsg::set_method_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  method_name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:beluga.RPCMsg.method_name)
}
inline void RPCMsg::set_method_name(const char* value, size_t size) {
  
  method_name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:beluga.RPCMsg.method_name)
}
inline std::string* RPCMsg::mutable_method_name() {
  
  // @@protoc_insertion_point(field_mutable:beluga.RPCMsg.method_name)
  return method_name_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* RPCMsg::release_method_name() {
  // @@protoc_insertion_point(field_release:beluga.RPCMsg.method_name)
  
  return method_name_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void RPCMsg::set_allocated_method_name(std::string* method_name) {
  if (method_name != nullptr) {
    
  } else {
    
  }
  method_name_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), method_name);
  // @@protoc_insertion_point(field_set_allocated:beluga.RPCMsg.method_name)
}

// string request_data = 3;
inline void RPCMsg::clear_request_data() {
  request_data_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& RPCMsg::request_data() const {
  // @@protoc_insertion_point(field_get:beluga.RPCMsg.request_data)
  return request_data_.GetNoArena();
}
inline void RPCMsg::set_request_data(const std::string& value) {
  
  request_data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:beluga.RPCMsg.request_data)
}
inline void RPCMsg::set_request_data(std::string&& value) {
  
  request_data_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:beluga.RPCMsg.request_data)
}
inline void RPCMsg::set_request_data(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  request_data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:beluga.RPCMsg.request_data)
}
inline void RPCMsg::set_request_data(const char* value, size_t size) {
  
  request_data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:beluga.RPCMsg.request_data)
}
inline std::string* RPCMsg::mutable_request_data() {
  
  // @@protoc_insertion_point(field_mutable:beluga.RPCMsg.request_data)
  return request_data_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* RPCMsg::release_request_data() {
  // @@protoc_insertion_point(field_release:beluga.RPCMsg.request_data)
  
  return request_data_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void RPCMsg::set_allocated_request_data(std::string* request_data) {
  if (request_data != nullptr) {
    
  } else {
    
  }
  request_data_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), request_data);
  // @@protoc_insertion_point(field_set_allocated:beluga.RPCMsg.request_data)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace beluga

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_beluga_2eproto
