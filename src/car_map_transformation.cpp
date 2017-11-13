//
// Created by Juha-Matti Tirila on 13/11/2017.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cmath>

std::vector<double> car_map_transform(std::vector<double>& meas, std::vector<double>& particle_pos) {
  std::vector<double> result;
  double px = particle_pos[0];
  double py = particle_pos[1];
  double theta = particle_pos[2];

  double cx = meas[0];
  double cy = meas[1];

  result.push_back(px + std::cos(theta) * cx - std::sin(theta) * cy);
  result.push_back(py + std::sin(theta) * cx - std::cos(theta) * cy);
  result.push_back(1.0);
  return result;
}

double VectorDist(std::vector<double>& x, std::vector<double>& y){
  double dx = x[0] - y[0];
  double dy = x[1] - y[1];
  return std::sqrt(dx * dx + dy * dy);
}

int FindClosestLandmark(std::vector<std::vector<double>> &landmarks, std::vector<double> &tobs){
  double min_dist;
  int min_ind = 0;
  for(int i = 0; i < landmarks.size(); i++){
    double current_dist = VectorDist(landmarks[i], tobs);
    // std::cout << "\nindex " << i << ": dist between (" << landmarks[i][0] << ", " << landmarks[i][1] << ") and (" <<
    //              tobs[0] << ", " << tobs[1] << "): " <<  current_dist << "; min so far: " << min_dist;
    if(i > 0) {
      if (current_dist < min_dist) {
        min_ind = i;
        min_dist = current_dist;
      }
    }
    else {
        min_ind = 0;
        min_dist = current_dist;
    }
  }
  return min_ind;
}


double EvaluateGaussian(std::vector<double>& tobs, std::vector<double>& landmark){
  double std_x = 0.3;
  double std_y = 0.3;
  double diff_x = tobs[0] - landmark[0];
  double diff_y = tobs[1] - landmark[1];
  double x_sqr_diff = diff_x * diff_x  / (2.0 * std_x * std_x);
  double y_sqr_diff = diff_y * diff_y  / (2.0 * std_y * std_y);
  return 1.0 / (2.0 * M_PI * std_x * std_y) * std::exp(-(x_sqr_diff + y_sqr_diff));
}



int main(){

  // Just some quick hacking.

  std::vector<double> particle_pos = {4.0, 5.0, -M_PI / 2.0};
  std::vector<double> meas1 = {2.0, 2.0};
  std::vector<double> meas2 = {3.0, -2.0};
  std::vector<double> meas3 = {0.0, -4.0};

  std::vector<std::vector<double>> landmarks;
  std::vector<double> landmark1 = {5.0, 3.0};
  std::vector<double> landmark2 = {2.0, 1.0};
  std::vector<double> landmark3 = {6.0, 1.0};
  std::vector<double> landmark4 = {7.0, 4.0};
  std::vector<double> landmark5 = {4.0, 7.0};

  landmarks.push_back(landmark1);
  landmarks.push_back(landmark2);
  landmarks.push_back(landmark3);
  landmarks.push_back(landmark4);
  landmarks.push_back(landmark5);

  std::vector<std::vector<double>> transformed;
  std::vector<double> transformed1 = car_map_transform(meas1, particle_pos);
  std::vector<double> transformed2 = car_map_transform(meas2, particle_pos);
  std::vector<double> transformed3 = car_map_transform(meas3, particle_pos);
  transformed.push_back(transformed1);
  transformed.push_back(transformed2);
  transformed.push_back(transformed3);

  int indeksi;
  for(int i = 0; i < transformed.size(); i++){
    std::cout << "transformed " << i + 1 << ":  (" << transformed[i][0] << ", " << transformed[i][1] << ")\n";
    indeksi = FindClosestLandmark(landmarks, transformed[i]);
    std::cout << "Closest to transformed " << i + 1 << ": L" << indeksi + 1 << "\n";
  }

  double gauss_val = EvaluateGaussian(transformed1, landmark1);
  double gauss_val2 = EvaluateGaussian(transformed2, landmark2);
  double gauss_val3 = EvaluateGaussian(transformed3, landmark2);
  std::cout << "Exp(1)" << std::exp(1) << "\n";
  std::cout << "Proba of first landmark, evaluated against first measurement: " << gauss_val << "\n";
  std::cout << "Proba of 2nd landmark, evaluated against second measurement: " << gauss_val2 << "\n";
  std::cout << "Proba of 2nd landmark, evaluated against third measurement: " << gauss_val3 << "\n";
  std::cout << "Product: " << gauss_val * gauss_val2 * gauss_val3 << "\n";


  return 0;
}
