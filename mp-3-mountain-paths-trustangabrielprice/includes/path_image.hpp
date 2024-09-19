#ifndef PATH_IMAGE_H
#define PATH_IMAGE_H

#include <cstdlib>  // for size_t
#include <vector>

#include "color.hpp"
#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "path.hpp"

class PathImage {
public:
  // write behavior declarations here; define in path_image.cc.
  PathImage(const GrayscaleImage& image, const ElevationDataset& dataset);
  void GeneratePaths(const ElevationDataset& dataset);

  void Uno(const ElevationDataset& dataset,
           std::vector<std::pair<int, int>>& next_move,
           int temp_row,
           int col) {
    int current_datum = dataset.DatumAt(static_cast<size_t>(temp_row),
                                        static_cast<size_t>(col));
    next_move.push_back(std::make_pair(
        abs(current_datum - dataset.DatumAt(static_cast<int>(temp_row),
                                            static_cast<int>(col + 1))),
        temp_row));
    next_move.push_back(std::make_pair(
        abs(current_datum - dataset.DatumAt(static_cast<int>(temp_row + 1),
                                            static_cast<int>(col + 1))),
        temp_row + 1));
  }

  void Dos(const ElevationDataset& dataset,
           std::vector<std::pair<int, int>>& next_move,
           int temp_row,
           int col) {
    int current_datum = dataset.DatumAt(static_cast<size_t>(temp_row),
                                        static_cast<size_t>(col));
    next_move.push_back(std::make_pair(
        abs(current_datum - dataset.DatumAt(static_cast<size_t>(temp_row - 1),
                                            static_cast<size_t>(col + 1))),
        temp_row - 1));
    next_move.push_back(std::make_pair(
        abs(current_datum - dataset.DatumAt(static_cast<size_t>(temp_row),
                                            static_cast<int>(col + 1))),
        temp_row));
  }

  void Tres(const ElevationDataset& dataset,
            std::vector<std::pair<int, int>>& next_move,
            int temp_row,
            int col) {
    int current_datum = dataset.DatumAt(static_cast<size_t>(temp_row),
                                        static_cast<size_t>(col));
    next_move.push_back(std::make_pair(
        abs(current_datum - dataset.DatumAt(static_cast<size_t>(temp_row - 1),
                                            static_cast<size_t>(col + 1))),
        temp_row - 1));
    next_move.push_back(std::make_pair(
        abs(current_datum - dataset.DatumAt(static_cast<size_t>(temp_row),
                                            static_cast<int>(col + 1))),
        temp_row));
    next_move.push_back(std::make_pair(
        abs(current_datum - dataset.DatumAt(static_cast<int>(temp_row + 1),
                                            static_cast<int>(col + 1))),
        temp_row + 1));
  }

  size_t Width() const;
  size_t Height() const;
  unsigned int MaxColorValue() const;
  const std::vector<Path>& Paths() const;
  const std::vector<std::vector<Color>>& GetPathImage() const;
  std::pair<int, int> CurrentRow(std::vector<std::pair<int, int>> next_moves);
  std::pair<int, int> FirstRow(std::vector<std::pair<int, int>> next_moves);
  std::pair<int, int> LastRow(std::vector<std::pair<int, int>> next_moves);
  int MinEle();
  void ToPpm(const std::string& name) const;

private:
  std::vector<Path> paths_;
  size_t width_ = 0;
  size_t height_ = 0;
  std::vector<std::vector<Color>> path_image_;
  static const int kMaxColorValue = 255;
};

#endif