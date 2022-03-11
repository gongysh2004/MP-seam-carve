#include "image_ppm.hpp"

#include <iostream>

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

void ImagePPM::PrintImageRgb(std::string header) const{
  char *debug = std::getenv("DEBUG");
  if(debug==nullptr) return;
  std::cout << header << std::endl;
  for (int row = 0; row < height_; row++)
    {
        for (int col = 0; col < width_; col++)
        {
           std::cout << pixels_[row][col].GetRed() << ","
               << pixels_[row][col].GetGreen() << ","
               << pixels_[row][col].GetBlue() << (col==width_-1? "":"\t");
        }
        std::cout << std::endl;
    }
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
  int space = line.find_first_of(' ');
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

// return pixel integer at a specific graph
Pixel ImagePPM::GetPixel(int row, int col) const { return pixels_[row][col]; }

int ImagePPM::GetMaxColorValue() const { return max_color_value_; }

void ImagePPM::CarveVerticalSeam(const int* vertical_delete) {
  auto** tmp_v = new Pixel*[height_];
  for (int row = 0; row < height_; ++row) {
    tmp_v[row] = new Pixel[width_ - 1];
  }
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_-1; col++) {
      if (col < vertical_delete[row]) {
        tmp_v[row][col] = pixels_[row][col];
      }else{
        tmp_v[row][col] = pixels_[row][col+1];
      }
    }
  }
  for (int row = 0; row < height_; ++row) delete[] pixels_[row];
  delete[] pixels_;
  pixels_ = tmp_v;
  width_--;
}

void ImagePPM::CarveHorizontalSeam(const int* horizontal_delete) {
  auto** tmp = new Pixel*[height_ - 1];
  for (int row = 0; row < height_ - 1; ++row) {
    tmp[row] = new Pixel[width_];
  }
  // std::cout << "a test";

  // std::cout << sizeof(horizontal_delete);
  for (int col = 0; col < width_; col++) {
    for (int row = 0; row < height_ -1; row++) {
      if (row < horizontal_delete[col]) {
        tmp[row][col] = pixels_[row][col];
      }else{
        tmp[row][col] = pixels_[row+1][col];
      } 
    }
  }
  for (int row = 0; row < height_; ++row) delete[] pixels_[row];
  delete[] pixels_;
  pixels_ = tmp;
  height_--;
}

std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  os << "P3" << std::endl;
  os << image.GetWidth() << " " << image.GetHeight() << std::endl;
  os << image.GetMaxColorValue() << std::endl;
  for (int row = 0; row < image.GetHeight(); ++row) {
    for (int col = 0; col < image.GetWidth(); ++col) {
      os << image.GetPixel(row, col).GetRed() << std::endl;
      os << image.GetPixel(row, col).GetGreen() << std::endl;
      os << image.GetPixel(row, col).GetBlue() << std::endl;
    }
  }
  return os;
}
