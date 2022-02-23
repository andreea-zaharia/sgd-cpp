//
// Created by Andreea Zaharia on 01/02/2022.
//

#ifndef FLOWER_CPPV2_LINE_FIT_MODEL_H
#define FLOWER_CPPV2_LINE_FIT_MODEL_H

#include <vector>

#include "synthetic_dataset.h"
#include "linear_algebra_util.h"

class LineFitModel {
public:
    LineFitModel(int num_iterations, double learning_rate, int num_params);

    std::vector<double> predict(std::vector<std::vector<double> > X);

    void StochasticGradientDescent(SyntheticDataset dataset);

private:
    int num_iterations;
    int batch_size;
    double learning_rate;

    std::vector<double> pred_weights;
    double pred_b;

    double compute_mse(std::vector<double> true_y, std::vector<double> pred) ;
};


#endif //FLOWER_CPPV2_LINE_FIT_MODEL_H
