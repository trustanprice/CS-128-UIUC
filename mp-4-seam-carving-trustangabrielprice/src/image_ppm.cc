#include "image_ppm.hpp"

Pixel ImagePPM::GetPixel(int row, int col) const { return pixels_[row][col]; }

int ImagePPM::GetMaxColorValue() const { return max_color_value_; }

// implement the rest of ImagePPM's functions here

// given functions below, DO NOT MODIFY

ImagePPM::ImagePPM(const std::string& path) {
  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }

  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}

std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;
  // ignore magic number line
  getline(is, line);
  // check to see if there's a comment line
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  // parse width and height
  int space = static_cast<int>(line.find_first_of(' '));
  image.width_ = std::stoi(line.substr(0, space));
  image.height_ = std::stoi(line.substr(space + 1));
  // get max color value
  getline(is, line);
  image.max_color_value_ = std::stoi(line);
  // init and fill in Pixels array
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  return is;
}

void ImagePPM::RemovingVerticalSeam(const int* seam) {
  // Initialize new pixels array
  Pixel** new_pixels = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    new_pixels[row] = new Pixel[width_ - 1];
  }

  int width_holder = width_;
  int height_holder = height_;
  // Move original pixels to the new pixels array, skipping pixels in the seam
  for (int row = 0; row < height_; row++) {
    int col_counts = 0;
    for (int col = 0; col < width_; col++) {
      if (col == seam[row]) {
        continue;
      }
      new_pixels[row][col_counts] = pixels_[row][col];
      col_counts++;
    }
  }

  // Clear original pixels_
  Clear();
  pixels_ = new_pixels;
  height_ = height_holder;
  // Update width
  width_ = width_holder - 1;

  // Update pixels_ to new
}

void ImagePPM::RemovingHorizontalSeam(const int* seam) {
  // Initialize new pixels array
  Pixel** new_pixels = new Pixel*[height_ - 1];
  for (int row = 0; row < height_ - 1; row++) {
    new_pixels[row] = new Pixel[width_];
  }

  int width_holder = width_;
  int height_holder = height_;

  // Move original pixels to the new pixels array, skipping pixels in the seam
  for (int col = 0; col < width_; col++) {
    int row_counts = 0;
    for (int row = 0; row < height_; row++) {
      if (row == seam[col]) {
        continue;
      }
      new_pixels[row_counts][col] = pixels_[row][col];
      row_counts++;
    }
  }

  // Clear original pixels_
  Clear();

  pixels_ = new_pixels;
  height_ = height_holder - 1;
  width_ = width_holder;
}

std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  // Output PPM header
  os << "P3\n";
  os << image.width_ << " " << image.height_ << "\n";
  os << image.max_color_value_ << "\n";

  // Output pixel values
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      os << image.pixels_[row][col].GetRed() << "\n";
      os << image.pixels_[row][col].GetGreen() << "\n";
      os << image.pixels_[row][col].GetBlue() << "\n";
    }
  }

  return os;
}