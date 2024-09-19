#include "grayscale_image.hpp"

GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset) {
  // Get the width and height from the dataset
  width_ = dataset.Width();
  height_ = dataset.Height();

  // Iterate through the dataset and populate the image vector
  for (size_t row = 0; row < height_; ++row) {
    std::vector<Color> row_colors;
    for (size_t col = 0; col < width_; ++col) {
      // Assuming DatumAt returns the elevation at the specified row and column
      int elevation = dataset.DatumAt(row, col);
      int grayscale_value = 0;
      if (dataset.MaxEle() != dataset.MinEle()) {
        grayscale_value = (int)std::round(
            (double)(elevation - dataset.MinEle()) /
            (double)(dataset.MaxEle() - dataset.MinEle()) * kMaxColorValue);
      }

      // Assuming a constructor in Color that takes a grayscale value
      Color color(grayscale_value);

      row_colors.push_back(color);
    }
    image_.push_back(row_colors);
  }
}

GrayscaleImage::GrayscaleImage(const std::string& filename,
                               size_t width,
                               size_t height):
    width_(width), height_(height) {
  ElevationDataset elevation_dataset(filename, width, height);

  // Assuming you have a function in ElevationDataset to get the data
  // Iterate through the dataset and populate the image vector
  for (size_t row = 0; row < height_; ++row) {
    std::vector<Color> row_colors;
    for (size_t col = 0; col < width_; ++col) {
      // Assuming DatumAt returns the elevation at the specified row and column
      int elevation = elevation_dataset.DatumAt(row, col);
      int grayscale_value = 0;
      if (elevation_dataset.MaxEle() != elevation_dataset.MinEle()) {
        grayscale_value = (int)std::round(
            (double)(elevation - elevation_dataset.MinEle()) /
            (double)(elevation_dataset.MaxEle() - elevation_dataset.MinEle()) *
            kMaxColorValue);
      }

      // Assuming a constructor in Color that takes a grayscale value
      Color color(grayscale_value);

      row_colors.push_back(color);
    }
    image_.push_back(row_colors);
  }
}

size_t GrayscaleImage::Width() const { return width_; }
size_t GrayscaleImage::Height() const { return height_; }

unsigned int GrayscaleImage::MaxColorValue() const { return kMaxColorValue; }

const Color& GrayscaleImage::ColorAt(int row, int col) const {
  if (row >= 0 && col >= 0 && static_cast<size_t>(row) < height_ &&
      static_cast<size_t>(col) < width_) {
    return image_[static_cast<size_t>(row)][static_cast<size_t>(col)];
  }
  throw std::out_of_range("Invalid range.");
}

const std::vector<std::vector<Color>>& GrayscaleImage::GetImage() const {
  return image_;
}

void GrayscaleImage::ToPpm(const std::string& name) const {
  std::ofstream ppm_file(name);

  if (ppm_file.is_open()) {
    // Write PPM header
    ppm_file << "P3\n";
    ppm_file << width_ << " " << height_ << "\n";
    ppm_file << "255\n";  // Maximum color value (0 to 65535)

    // Write image data
    for (size_t row = 0; row < height_; ++row) {
      for (size_t col = 0; col < width_; ++col) {
        const Color& color = image_[row][col];
        ppm_file << color.Red() << " " << color.Green() << " " << color.Blue()
                 << "\n";
      }
    }

    ppm_file.close();
  } else {
    throw std::runtime_error("This ppm file will not open: " + name);
  }
}