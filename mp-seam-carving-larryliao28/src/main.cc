#include <fstream>
#include <iostream>

#include "image_ppm.hpp"
#include "seam_carver.hpp"

void PrintUsage(const std::string& command) {
  std::cout << "Usage: " << command << " INPUT_FILE [options] -o OUTPUT_FILE\n"
            << "Options:\n"
            << "\t-h times\tcarve times horizontal seams\n"
            << "\t-v times\tcarve times vertical seams\n";
}

int PrintTooMuchCarve(int argc, char* argv[],ImagePPM& image){
  int height = image.GetHeight();
  int width = image.GetWidth();
  int h_times = 0;
  int v_times = 0;
  for (int i = 2; i < argc - 2; i += 2) {
    std::string times_string = argv[i + 1];
    int times = 0;
    try {
      times = std::stoi(times_string);
    } catch (const std::exception& e) {
      std::cout << "Malformed option" << std::endl;
      PrintUsage(argv[0]);
      return 1;
    }
    std::string option = argv[i];
    if (option == "-h") {
      h_times += times;
    } else if (option == "-v") {
      v_times += times;
    } else {
      std::cout << argv[i] << " not an option" << std::endl;
      PrintUsage(argv[0]);
      return 1;
    }
  }
  if (h_times  >= height || v_times >= width){
    std::cout << "images with dimension " << height << "*" << width << " cannot be carved so much times!"<< std::endl;
    PrintUsage(argv[0]);
    return 1;
  }
  return 0;

}
int main(int argc, char* argv[]) {
  std::string out_option = "-o";
  if (argc < 4 || out_option != argv[argc - 2]) {
    PrintUsage(argv[0]);
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile.good()) {
    std::cout << "file '" << argv[1] << "' not found" << std::endl;
    return 1;
  }

  /* UNCOMMENT COMMENTED CODE BELOW AS YOU IMPLEMENT */

  ImagePPM image;
  infile >> image;
  infile.close();
  int carveerror = PrintTooMuchCarve(argc, argv, image);
  if (carveerror!=0)  return carveerror;
  image.PrintImageRgb("Image RGB:");
  SeamCarver carver(image);
  for (int i = 2; i < argc - 2; i += 2) {
    std::string times_string = argv[i + 1];
    int times = 0;
    try {
      times = std::stoi(times_string);
    } catch (const std::exception& e) {
      std::cout << "Malformed option" << std::endl;
      PrintUsage(argv[0]);
      return 1;
    }

    // std::cout << carver.GetEnergy(0, 0);

    std::string option = argv[i];
    if (option == "-h") {
      for (int t = 0; t < times; t++) {
        carver.RemoveHorizontalSeam();
        carver.GetImage().PrintImageRgb("rgb aftrer horizontal carve");
      }
    } else if (option == "-v") {
      for (int t = 0; t < times; t++) {
        carver.RemoveVerticalSeam();
        carver.GetImage().PrintImageRgb("rgb aftrer vertical carve");
      }
    } else {
      std::cout << argv[i] << " not an option" << std::endl;
      PrintUsage(argv[0]);
      return 1;
    }
  }

  std::ofstream outfile(argv[argc - 1]);
  outfile << carver.GetImage();
  outfile.close();
}
