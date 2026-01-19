// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_MUSIC_BYTE_BUFFER_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_MUSIC_BYTE_BUFFER_H_
#include <stdexcept>
#include <vector>
namespace music {

enum class Endian { kLittle = 0, kBig = 1, kNative = 2 };

class MusicByteBuffer {
 private:
  std::vector<char> data_;
  Endian endian_layout_ = Endian::kNative;
  int read_pointer_ = 0;

 public:
  MusicByteBuffer() { data_ = std::vector<char>(); }
  MusicByteBuffer(const char *data, unsigned int length,
                  Endian endian_layout = Endian::kNative) {
    data_ = std::vector<char>();
    if (data == nullptr) {
      throw new std::runtime_error(
          "Attempted to initialize with a null pointer!");
    }

    // Copy data and take ownership of it
    for (int i = 0; i < length; i++) {
      char read = data[i];
      data_.push_back(read);
    }
    endian_layout_ = endian_layout;
  }

  unsigned int GetLength() const { return data_.size(); }

  Endian GetBufferEndian() const { return endian_layout_; }

  std::vector<char> GetDataAt(unsigned int end, unsigned int start = 0) const {
    if (end - start > data_.size()) {
      // Hard error to prevent memory corruption and out-of-bounds reads
      throw std::runtime_error("Invalid buffer read offset!");
    }
    std::vector<char> read_buffer = std::vector<char>();
    for (int i = start; i < end; i++) {
      read_buffer.push_back(data_.at(i));
    }
    return read_buffer;
  }
  const char &operator[](unsigned int index) const {
    if (index >= data_.size()) {
      // Hard error to prevent memory corruption and out-of-bounds reads
      throw std::runtime_error("Invalid buffer read offset!");
    }

    return data_.at(index);
  }

  template <class T>
  void InsertValue(const int start_index, const T *value) {
    if (value == nullptr) {
      throw new std::runtime_error("Attempted to insert a null pointer!");
    }

    if (start_index >= data_.size()) {
      AppendValue(value, sizeof(*value));
      return;
    }

    const char *value_bytes = reinterpret_cast<const char *>(value);
    int size = sizeof(*value);
    for (int i = 0; i < size; i++) {
      data_.insert(data_.begin() + start_index + i, value_bytes[i]);
    }
  }

  void ResetReadPointerToBeginning() { read_pointer_ = 0; }

  /** -- CALL WITH CAUTION! -- Can cause segmentation faults if read value bytes
   * can't be correctly dereferenced  */
  template <class T>
  T ReadValueSequential() {
    std::vector<char> read =
        GetDataAt(read_pointer_ + sizeof(T), read_pointer_);
    read_pointer_ += sizeof(T);
    return *reinterpret_cast<T *>(&read.data()[0]);
  }

  /**Reads a value of the given type back at the provided location. -- CALL WITH
   * CAUTION! -- Can cause segmentation faults if read value bytes can't be
   * correctly dereferenced  */
  template <class T>
  T ReadValueAt(unsigned int size, int) {
    std::vector<char> read = GetDataAt(size, read_pointer_);
    read_pointer_ += size;
    return *reinterpret_cast<T *>(&read.data()[0]);
  }
  void AppendCharVector(std::vector<char> vec) {
    for (char c : vec) {
      data_.push_back(c);
    }
  }

  std::vector<char> GetNextBytes(int size) {
    std::vector<char> read = GetDataAt(read_pointer_ + size, read_pointer_);
    read_pointer_ += size;
    return read;
  }

  // Overload for better ergonomics. Value will be copied.
  template <class T>
  void AppendValue(const T value, uint size) {
    AppendValue(&value, size);
  }

  /** Appends a value to the END of the data buffer */
  template <class T>
  void AppendValue(const T *value, uint size) {
    if (value == nullptr) {
      throw new std::runtime_error("Attempted to append a null pointer!");
    }

    const char *value_bytes = reinterpret_cast<const char *>(value);
    for (int i = 0; i < size; i++) {
      data_.push_back(value_bytes[i]);
    }
  }
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_MUSIC_BYTE_BUFFER_H_