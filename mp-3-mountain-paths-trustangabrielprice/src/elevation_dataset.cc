#include "elevation_dataset.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

void ElevationDataset::DataCounter(const std::vector<int>& input,
                                   int& max,
                                   int& min) {
  if (input.empty()) {
    throw std::invalid_argument("Input vector is empty");
  }

  max = input[0];
  min = input[0];

  for (size_t i = 1; i < input.size(); ++i) {
    if (input[i] > max) {
      max = input[i];
    }
    if (input[i] < min) {
      min = input[i];
    }
  }
}

ElevationDataset::ElevationDataset(const std::string& filename,
                                   size_t width,
                                   size_t height):
    width_(width), height_(height) {
  int max = 0;
  int min = 0;
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file: " + filename);
  }

  std::vector<int> tmp;
  int value = 0;
  while (file >> value) {
    tmp.push_back(value);
  }

  if (tmp.size() != width_ * height_) {
    throw std::runtime_error("Too many or too little data points");
  }

  DataCounter(tmp, max, min);
  max_ele_ = max;
  min_ele_ = min;

  size_t i = 0;
  for (size_t row = 0; row < height_; ++row) {
    std::vector<int> data;
    for (size_t col = 0; col < width_; ++col) {
      data.push_back(tmp[i]);
      ++i;
    }
    data_.push_back(data);
  }

  file.close();
}

size_t ElevationDataset::Width() const { return width_; }

size_t ElevationDataset::Height() const { return height_; }

void ElevationDataset::ComputeMinMaxElevations() {
  if (data_.empty()) {
    throw std::runtime_error("Dataset is empty.");
  }

  min_ele_ = std::numeric_limits<int>::max();
  max_ele_ = std::numeric_limits<int>::min();

  for (const auto& row : data_) {
    for (int elevation : row) {
      min_ele_ = std::min(min_ele_, elevation);
      max_ele_ = std::max(max_ele_, elevation);
    }
  }
}

int ElevationDataset::MaxEle() const { return max_ele_; }

int ElevationDataset::MinEle() const { return min_ele_; }

int ElevationDataset::DatumAt(size_t row, size_t col) const {
  // Check if the requested coordinates are within the valid range
  if (row < height_ && col < width_) {
    return data_[row][col];
  }
  throw std::out_of_range(
      "ElevationDataset::DatumAt(): Invalid coordinates (row, col).");
}

const std::vector<std::vector<int>>& ElevationDataset::GetData() const {
  if (data_.empty()) {
    throw std::logic_error("Dataset, of name file, is empty.");
  }
  return data_;
}