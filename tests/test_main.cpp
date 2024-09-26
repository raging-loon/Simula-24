#include <stdio.h>
#include <gtest/gtest.h>
#include "simula24/simula24_tests.h"

int main(int argc, char** argv)
{
    printf("Running unit tests...\n");
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}