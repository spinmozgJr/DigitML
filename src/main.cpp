
#include "dataset.hpp"
#include "NN.hpp"
#include "../lib/matrix.h"
#include <vector>
#include <iostream>

void debug(Example e) {
    static std::string shades = " .:-=+*#%@";
    for (unsigned int i = 0; i < 28 * 28; i++) {
        if (i % 28 == 0) printf("\n");
        printf("%c", shades[e.data[i] / 30]);
    }
    printf("\nLabel: %d\n", e.label);
}

std::vector<double> load_matrix(Example& e) {
    std::vector<double> result(e.data, e.data + 28 * 28);
    return result;
}

const double calculate_accuracy(const Matrix<unsigned char>& images, const Matrix<unsigned char>& labels, NeuralNetwork n) {
  unsigned int correct = 0;
  for (unsigned int i = 0; i < images.rows(); ++i) {
    Example e;
    for (int j = 0; j < 28*28; ++j) {
        e.data[j] = images[i][j];
    }
    e.label = labels[i][0];
    unsigned int guess = n.compute(e);
    if (guess == (unsigned int)e.label) correct++;
  }
  const double accuracy = (double)correct/images.rows();

  return accuracy;
}

double arctg(double x) {
    return atan(x);
}

#ifdef TESTS
#include <gtest/gtest.h>

NeuralNetwork n;

TEST(FunctionTesting, test_arctg1) {  
  EXPECT_EQ(arctg(-1), -0.785398);
}

TEST(FunctionTesting, test_arctg2) {  
  EXPECT_EQ(arctg(0), 0);
}

//EXPECT_THAT(test1, ::testing::ContainerEq(test2));

TEST(FunctionTesting, test_count_arctg) {  
  std::vector<double> test = {-1, 0, 1};
  EXPECT_EQ(n.arctg(test).size(), 3);
}

TEST(FunctionTesting, test_vector_of_artcg) {
  std::vector<double> test1 = {-1, 0, 1};
  std::vector<double> test2 = {-0.785398, 0, 0.785398};
  EXPECT_THAT(n.arctg(test1), ::testing::Eq(test2));
  //EXPECT_DOUBLE_EQ(n.arctg(test1), test2);
}

TEST(FunctionTesting, test_empty_vector) {
  std::vector<double> test = {};
  EXPECT_EQ(n.arctg(test).size(), 0);
}
//EXPECT_NEAR(addend1(-1),-0.54030230586,1e-6);
//EXPECT_NEAR(NeuralNetwork.arctg(test), new std::vector<double>{}, 1e-6);
#endif

int main(int argc, char **argv) {
    #ifdef TEST
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    #endif

    Matrix<unsigned char> images_train(0, 0);
    Matrix<unsigned char> labels_train(0, 0);
    load_dataset(images_train, labels_train, "data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte");

    Matrix<unsigned char> images_test(0, 0);
    Matrix<unsigned char> labels_test(0, 0);
    load_dataset(images_test, labels_test, "data/t10k-images-idx3-ubyte", "data/t10k-labels-idx1-ubyte");

    NeuralNetwork n;

    // Tests to see that data was read in properly
    /*for (int i = 0; i < 10; ++i) {
        Example e;
        for (int j = 0; j < 28*28; ++j) {
            e.data[j] = images_train[i][j];
        }
        e.label = labels_train[i][0];
        debug(e);
        printf("Guess: %d\n", n.compute(e));
    }
    for (int i = 0; i < 10; ++i) {
        Example e;
        for (int j = 0; j < 28*28; ++j) {
            e.data[j] = images_test[i][j];
        }
        e.label = labels_test[i][0];
        debug(e);
        printf("Guess: %d\n", n.compute(e));
    }*/
    const unsigned int num_iterations = 5;
    n.train(num_iterations, images_train, labels_train);

    const double accuracy_train = calculate_accuracy(images_train, labels_train, n);
    const double accuracy_test = calculate_accuracy(images_test, labels_test, n);

    printf("Accuracy on training data: %f\n", accuracy_train);
    printf("Accuracy on test data: %f\n", accuracy_test);

    return 0;
}
