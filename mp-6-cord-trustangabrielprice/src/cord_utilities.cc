#include "cord_utilities.hpp"

#include "cord.hpp"
#include "shared_pointer.hpp"

SharedPointer<Cord> ConcatCords(SharedPointer<Cord> left_cord,
                                SharedPointer<Cord> right_cord) {
  // Check if either of the cords is null
  if (left_cord.Get() == nullptr || right_cord.Get() == nullptr) {
    throw std::invalid_argument("Either left_cord or right_cord is null");
  }

  // Get the lengths of the left and right cords
  unsigned int left_length = left_cord->Length();
  unsigned int right_length = right_cord->Length();

  // Create a new Cord object for the concatenated string
  SharedPointer<Cord> concatenated_cord = new Cord();
  concatenated_cord->left_ =
      left_cord;  // assign a value that points to left cord
  concatenated_cord->right_ =
      right_cord;  // assign a value that points to right cord
  concatenated_cord->length_ = left_length + right_length;
  return concatenated_cord;
}

SharedPointer<Cord> SubString(SharedPointer<Cord> curr_cord,
                              unsigned int lower_idx,
                              unsigned int upper_idx) {
  if (curr_cord.Get() == nullptr) {
    throw std::invalid_argument("curr_cord is null");
  }
  if (lower_idx >= upper_idx) {
    throw std::invalid_argument(
        "lower_idx is greater than or equal to upper_idx");
  }
  if (upper_idx > curr_cord->Length()) {
    throw std::out_of_range("upper_idx is outside the bounds of the cord");
  }
  unsigned int substring_length = upper_idx - lower_idx;
  if (substring_length == curr_cord->Length()) return curr_cord;
  if (curr_cord->Left().Get() == nullptr &&
      curr_cord->Right().Get() == nullptr) {
    char* substring_data = new char[substring_length + 1];
    for (unsigned int i = 0; i < substring_length; ++i) {
      substring_data[i] = curr_cord->Data()[lower_idx + i];
    }
    substring_data[substring_length] = '\0';
    SharedPointer<Cord> substring_cord(new Cord(substring_data));
    return substring_cord;
  }
  unsigned int left_length = 0;
  if (curr_cord->Left().Get() != nullptr)
    left_length = curr_cord->Left()->Length();
  if (upper_idx <= left_length)
    return SubString(curr_cord->Left(), lower_idx, upper_idx);
  if (lower_idx >= left_length) {
    return SubString(
        curr_cord->Right(), lower_idx - left_length, upper_idx - left_length);
  }
  SharedPointer<Cord> left_substring =
      SubString(curr_cord->Left(), lower_idx, left_length);
  SharedPointer<Cord> right_substring =
      SubString(curr_cord->Right(), 0, upper_idx - left_length);
  return ConcatCords(left_substring, right_substring);
}
