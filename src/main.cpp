//
// Created by Andreea Zaharia on 31/01/2022.
//

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "line_fit_model.h"
#include "synthetic_dataset.h"
//#include "start.cc"
//#include "line_fit_client.cpp"

int main(int argc, char** argv) {
    // TODO: Save the dataset to disk and write a load method.
    SyntheticDataset training_data = SyntheticDataset(1, 1000, 100, 500);

    LineFitModel model = LineFitModel(500, 0.01, 1);
    model.StochasticGradientDescent(training_data);

    return 0;
}
