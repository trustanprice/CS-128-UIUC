#ifndef SHARED_POINTER_HPP
#define SHARED_POINTER_HPP

enum class kSharedPointerType { kArray, kObject };

template <typename T>
class SharedPointer {
public:
  SharedPointer(T* address,
                kSharedPointerType type =
                    kSharedPointerType::kObject);  // parameterized constructor
  ~SharedPointer();
  SharedPointer(const SharedPointer<T>& source);  // copy assignment constructor
  SharedPointer<T>& operator=(
      const SharedPointer<T>&
          source);  // copy assignment operator, you define this

  // Provided, do not modify!
  SharedPointer() = default;
  T& operator*() { return *holder_; }
  T* operator->() { return holder_; }
  const T* operator->() const { return holder_; }
  T* Get() { return holder_; }  //  return the address of the object(holder)
  const T* Get() const { return holder_; }
  unsigned int UseCount() const {
    return (ref_count_ != nullptr) ? *ref_count_
                                   : 0;  // return the reference of holder
  }

private:
  T* holder_ = nullptr;
  kSharedPointerType type_ = kSharedPointerType::kObject;
  unsigned int* ref_count_ = nullptr;
};

// non-member helper function declarations

// non-member helper function definitions

// member function definitions

template <typename T>
SharedPointer<T>::SharedPointer(T* address, kSharedPointerType type):
    holder_(address), type_(type) {
  ref_count_ = new unsigned int(
      1);  // 1 shared pointer object engaged in the shared ownership
}

template <typename T>
SharedPointer<T>::~SharedPointer() {
  if (ref_count_ == nullptr) {
    return;
  }

  (*ref_count_) -= 1;  // Decrement reference count if ref_count_ is not null
  if ((*ref_count_) == 0) {  // Check if reference count is 0
    if (type_ == kSharedPointerType::kObject) {
      delete holder_;  // Delete the object pointed to by holder_
    } else {
      delete[] holder_;  // Delete the array pointed to by holder_
    }
    holder_ = nullptr;
    delete ref_count_;  // Delete the reference count
    ref_count_ = nullptr;
  }
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& source):
    holder_(source.holder_),
    type_(source.type_),
    ref_count_(source.ref_count_) {
  if (source.ref_count_ != nullptr) {
    (*(source.ref_count_)) +=
        1;  // Increment reference count if ref_count_ is not null
  }
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<T>& source) {
  // Check for self-assignment
  if (this == &source) {
    return *this;  // Self-assignment, do nothing
  }

  // Decrement reference count of current object (if applicable)
  if (ref_count_ != nullptr) {
    (*ref_count_) -= 1;
    if ((*ref_count_) == 0) {
      if (type_ == kSharedPointerType::kObject) {
        delete holder_;
      } else {
        delete[] holder_;
      }
      delete ref_count_;
    }
  }

  // Perform shallow copy
  holder_ = source.holder_;
  type_ = source.type_;
  ref_count_ = source.ref_count_;

  // Increment reference count
  if (ref_count_ != nullptr) {
    (*ref_count_) += 1;
  }

  return *this;
}

#endif