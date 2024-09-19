#include "cord.hpp"

#include <stdexcept>

#include "cord_utilities.hpp"

Cord::Cord(std::string_view sv) {
  if (sv.empty()) {
    throw std::invalid_argument("String view size is 0");
  }

  // Allocate memory for the new C-style string
  data_ = new char[sv.size() + 1];  // +1 for null terminator

  // Copy the data from sv.data() to the dynamically allocated C-style string
  for (size_t i = 0; i < sv.size(); ++i) {
    data_[i] = sv[i];
  }
  data_[sv.size()] = '\0';  // Add the null terminator

  length_ = sv.size();
}

Cord::~Cord() { delete[] data_; }
SharedPointer<char> Cord::ToString() const {
  if (data_ != nullptr) {
    char* char_array = new char[length_ + 1];
    for (size_t i = 0; i < length_; ++i) {
      char_array[i] = data_[i];
    }
    char_array[length_] = '\0';  // terminal character
    return {char_array, kSharedPointerType::kArray};
  }

  auto left_string = left_->ToString();
  auto right_string = right_->ToString();  // have to get the char_array from
                                           // either and then index it

  char* char_array = new char[length_ + 1];
  for (size_t i = 0; i < left_->Length(); ++i) {
    char_array[i] = left_string.Get()[i];  // need to get and then index
  }

  for (size_t i = 0; i < right_->Length(); ++i) {
    char_array[i + left_->Length()] = right_string.Get()[i];
  }
  char_array[length_] = '\0';  // terminal character
  return {char_array, kSharedPointerType::kArray};
}
