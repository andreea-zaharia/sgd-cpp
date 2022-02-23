//
// Created by Andreea Zaharia on 01/02/2022.
//

#include "line_fit_model.h"
#include "synthetic_dataset.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

LineFitModel::LineFitModel(int num_iterations, double learning_rate, int num_params) : num_iterations(num_iterations), learning_rate(learning_rate) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> distr(-10.0, 10.0);
    for (int i = 0; i < num_params; i++) {
        this->pred_weights.push_back(distr(mt));
    }

    this->pred_b = 0.0;
    this->batch_size = 64;
}


std::vector<double> LineFitModel::predict(std::vector<std::vector<double> > X) {
    std::vector<double> prediction(X.size(), 0.0);
    for (int i = 0; i < X.size(); i++) {
        for (int j = 0; j < X[i].size(); j++) {
            prediction[i] += this->pred_weights[j] * X[i][j];
        }
        prediction[i] += this->pred_b;
    }

    return prediction;
}

void LineFitModel::StochasticGradientDescent(SyntheticDataset dataset) {
    int features = dataset.get_features_count();
    std::vector<std::vector<double> > data_points = dataset.get_training_data();

    std::vector<double> data_indices(dataset.size());
    for (int i = 0; i < dataset.size(); i++) {
        data_indices.push_back(i);
    }


    std::vector<double> dW(features);
    std::vector<double> err(batch_size, 10000);
    std::vector<double> pW(features);
    for (int iteration = 0; iteration < num_iterations; iteration++) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(data_indices.begin(), data_indices.end(), g);


        std::vector<std::vector<double> > X(this->batch_size, std::vector<double>(features));
        std::vector<double> y(this->batch_size);

        for (int i = 0; i < this->batch_size; i++) {
            std::vector<double> point = data_points[data_indices[i]];
            y[i] = point.back();
            point.pop_back();
            X[i] = point;
        }

        pW = this->pred_weights;
        double pB = this->pred_b;
        double dB;

        std::vector<double> pred = predict(X);

        err = LinearAlgebraUtil::subtract_vector(y, pred);

        dW = LinearAlgebraUtil::multiply_matrix_vector(LinearAlgebraUtil::transpose_vector(X), err);
        dW = LinearAlgebraUtil::multiply_vector_scalar(dW, (-2.0/this->batch_size));

        dB =  (-2.0/this->batch_size) * std::accumulate(err.begin(), err.end(), 0.0);

        this->pred_weights = LinearAlgebraUtil::subtract_vector(pW, LinearAlgebraUtil::multiply_vector_scalar(dW, learning_rate));
        this->pred_b = pB - learning_rate * dB;

        if (iteration % 250 == 0) {
            double training_error = this->compute_mse(y, predict(X));
            std::cout << "iteration: " << iteration << "  Training error: " << training_error << '\n';
        }

    }

    for (int i = 0; i < pred_weights.size(); i++) {
        std::cout << std::fixed << pred_weights[i] << " ";
    }
    std::cout << "\n" << std::fixed<< pred_b;
}

double LineFitModel::compute_mse(std::vector<double> true_y, std::vector<double> pred) {
    double error = 0.0;

    for (int i = 0; i < true_y.size(); i++) {
        error += (pred[i] - true_y[i]) * (pred[i] - true_y[i]);
    }

    return error / (1.0 * true_y.size());
}
