//
// Created by Andreea Zaharia on 31/01/2022.
//

#include "synthetic_dataset.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>

SyntheticDataset::SyntheticDataset(int ms_size, int training_size, int validation_size, int test_size) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> distr(-10.0, 10.0);

    std::vector<double> ms;
    for (int i = 0; i < ms_size; i++) {
        double m = distr(mt);
        ms.push_back(m);
        std::cout << std::fixed<< "m_" << i << " = " << m << '\n';
    }

//    std::vector<std::vector<double> > training_data(size, std::vector<double>(ms_size, 0));
    double b = rd() % 100;
    std::cout << "b = " << b <<'\n';

    int size = training_size + validation_size + test_size;

    std::vector<std::vector<double> > xs(size, std::vector<double>(ms_size));
    std::vector<double> ys(size, 0);
    for (int m_ind = 0; m_ind < ms_size; m_ind++) {
        std::uniform_real_distribution<double> distx(-10.0, 10.0);

        for (int i = 0; i < size; i++) {
            xs[i][m_ind] = distx(mt);
        }
    }

    for (int i = 0; i < size; i++) {
        ys[i] = b;
        for (int m_ind = 0; m_ind < ms_size; m_ind++) {
            ys[i] += ms[m_ind] * xs[i][m_ind];
        }
    }

    std::vector<std::vector<double> > training_points;
    for (int i = 0; i < training_size; i++) {
        std::vector<double> data_point;
        data_point.insert(data_point.end(), xs[i].begin(), xs[i].end());
        data_point.push_back(ys[i]);

        training_points.push_back(data_point);
    }

    std::vector<std::vector<double> > validation_points;
    for (int i = training_size; i < training_size + validation_size; i++) {
        std::vector<double> data_point;
        data_point.insert(data_point.end(), xs[i].begin(), xs[i].end());
        data_point.push_back(ys[i]);

        validation_points.push_back(data_point);
    }

    std::vector<std::vector<double> > test_points;
    for (int i = validation_size; i < training_size + validation_size + test_size; i++) {
        std::vector<double> data_point;
        data_point.insert(data_point.end(), xs[i].begin(), xs[i].end());
        data_point.push_back(ys[i]);

        test_points.push_back(data_point);
    }

    this->training_data = training_points;
    this->validation_data = validation_points;
    this->test_data = test_points;
}


int SyntheticDataset::size() {
    return this->training_data.size();
}

int SyntheticDataset::get_features_count() {
    return this->training_data[0].size() - 1;
}

std::vector<std::vector<double> > SyntheticDataset::get_training_data() {
    return training_data;
}

std::vector<std::vector<double> > SyntheticDataset::get_validation_data() {
    return validation_data;
}

std::vector<std::vector<double> > SyntheticDataset::get_test_data() {
    return test_data;
}
