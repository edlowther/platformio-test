#include <unity.h>

#include "../lib/Config.h"

#include "../lib/Flock.h"
#include "../lib/View.h"

void test_flock_size_is_correct() {
    Flock flock;
    TEST_ASSERT_TRUE(flock.positions.xs.Rows == 7);
}

void test_position_update_is_working() {
    Flock flock; 
    flock.positions.xs         = {1, 1, 1, 1, 1, 1, 1};
    flock.positions.ys         = {1, 1, 1, 1, 1, 1, 1};
    flock.velocities.dxs       = {1, 1, 2, 1, 4, 1,-1};
    flock.velocities.dys       = {1, 1,-1, 1, 4, 1,-1};
    flock.update();
    BLA::Matrix<7> expected_xs = {2, 2, 3, 2, 5, 2, 0};
    BLA::Matrix<7> expected_ys = {2, 2, 0, 2, 5, 2, 0};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_INT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_INT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_reverts_to_zero_if_past_edge_x() {
    Flock flock; 
    flock.positions.xs = {1, 1, 1, 1, 1, 1, 6};
    flock.positions.ys = {1, 1, 1, 1, 1, 1, 1};
    flock.velocities.dxs = {1, 1, 1, 1, 1, 1, 1};
    flock.velocities.dys = {1, 1, 1, 1, 1, 1, 1};
    flock.update();
    BLA::Matrix<7> expected_xs = {2, 2, 2, 2, 2, 2, 0};
    BLA::Matrix<7> expected_ys = {2, 2, 2, 2, 2, 2, 2};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_INT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_INT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_reverts_to_zero_if_past_edge_y() {
    Flock flock; 
    flock.positions.xs         = {1, 1, 1, 1, 1, 1, 1};
    flock.positions.ys         = {1, 1, 1, 6, 1, 1, 1};
    flock.velocities.dxs       = {1, 1, 1, 1, 1, 1, 1};
    flock.velocities.dys       = {1, 1, 1, 1, 1, 1, 1};
    flock.update();
    BLA::Matrix<7> expected_xs = {2, 2, 2, 2, 2, 2, 2};
    BLA::Matrix<7> expected_ys = {2, 2, 2, 0, 2, 2, 2};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_INT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_INT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_update_with_cohesion_is_working() {
    Flock flock; 
    flock.positions.xs         = {1., 2, 4, 5, 3, 1, 2};
    flock.positions.ys         = {1., 5, 1, 5, 5, 1, 1};
    flock.velocities.dxs       = {1., 1, -2, 1, -2, 1,-1};
    flock.velocities.dys       = {1., 1,-1, 1, -4, 1,-1};
    flock.update();
    BLA::Matrix<7> expected_xs = {2.01571429, 3.00571429, 1.98571429, 5.97571429, 0.99571429,
        2.01571429, 1.00571429};
    BLA::Matrix<7> expected_ys = {2.01714286, 5.97714286, 0.01714286, 5.97714286, 0.97714286,
        2.01714286, 0.01714286};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_FLOAT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_update_with_cohesion_is_working_2() {
    Flock flock; 
    flock.positions.xs         = {1.0, 1, 4, 5, 3, 5, 2};
    flock.positions.ys         = {1.0, 5, 1, 1, 3, 5, 5};
    flock.velocities.dxs         = {3.0, 1, -2, 1, -2, 1, -1};
    flock.velocities.dys         = {1.0, -3, -1, 1, -1, 1, -1};
    flock.update();
    BLA::Matrix<7> expected_xs = {4.2       ,  2.2       ,  1.9       ,  5.8       ,  1.        ,
         5.8       ,  1.1};
    BLA::Matrix<7> expected_ys = {2.2, 1.8, 0.2, 2.2, 2. , 5.8, 3.8};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_FLOAT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_update_with_cohesion_is_working_3() {
    Flock flock; 
    flock.positions.xs         = {1.0, 1, 4, 5, 3, 5, 2};
    flock.positions.ys         = {1.0, 5, 1, 1, 3, 5, 5};
    flock.velocities.dxs         = {3.0, 1, -2, 1, -2, 1, -1};
    flock.velocities.dys         = {1.0, -3, -1, 1, -1, 1, -1};
    flock.update();
    BLA::Matrix<7> expected_xs = {6.        ,  4.        ,  1.        ,  4.        ,  1.        ,
         4.        ,  2.};
    BLA::Matrix<7> expected_ys = {4., 0., 2., 4., 2., 4., 2.};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_FLOAT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_update_with_separation_is_working() {
    Flock flock; 
    flock.positions.xs         = {1.0, 1, 4, 5, 3, 5, 2};
    flock.positions.ys         = {1.0, 5, 1, 1, 3, 5, 5};
    flock.velocities.dxs         = {3.0, 1, -2, 1, -2, 1, -1};
    flock.velocities.dys         = {1.0, -3, -1, 1, -1, 1, -1};
    flock.update();
    BLA::Matrix<7> expected_xs = {4.2, 2. , 1.7, 6. , 1. , 5.8, 1.3};
    BLA::Matrix<7> expected_ys = {2.2, 1.8, 0.2, 2.2, 2. , 5.8, 3.8};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_FLOAT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_update_with_separation_is_working_2() {
    Flock flock; 
    flock.positions.xs         = {1.0, 1, 4, 5, 3, 5, 2};
    flock.positions.ys         = {1.0, 5, 1, 1, 3, 5, 5};
    flock.velocities.dxs         = {3.0, 1, -2, 1, -2, 1, -1};
    flock.velocities.dys         = {1.0, -3, -1, 1, -1, 1, -1};
    flock.update();
    BLA::Matrix<7> expected_xs = {4.2,  1.9,  1.9,  0.0999999,  1. ,  5.8,  1.1};
    BLA::Matrix<7> expected_ys = {2.2,  1.8,  5.6,  2.2,  2. ,  5.8,  4.4};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_FLOAT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_update_with_separation_is_working_3() {
    Flock flock; 
    flock.positions.xs         = {1., 1, 4, 4, 3, 3, 2};
    flock.positions.ys         = {1., 5, 1, 1, 3, 5, 5};
    flock.velocities.dxs         = {3., 1,-2, 1, -2, 1,-1};
    flock.velocities.dys         = {1.,-3,-1, 1, -1, 1,-1};
    flock.update();
    BLA::Matrix<7> expected_xs = {3.35714286, 1.65714286, 2.25714286, 5.25714286, 1.25714286,
        4.25714286, 0.95714286};
    BLA::Matrix<7> expected_ys = {2.        , 2.        , 0.        , 2.        , 2.        ,
        6.        , 4.};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_FLOAT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_position_update_with_alignment_is_working() {
    Flock flock; 
    flock.positions.xs         = {1., 1, 4, 4, 3, 3, 2};
    flock.positions.ys         = {1., 5, 1, 1, 3, 5, 5};
    flock.velocities.dxs         = {3., 1,-2, 1, -2, 1,-1};
    flock.velocities.dys         = {1.,-3,-1, 1, -1, 1,-1};
    flock.update();
    BLA::Matrix<7> expected_xs = {3.10714286, 1.60357143, 2.4       , 5.18571429, 1.525     ,
        4.16785714, 1.01071429};
    BLA::Matrix<7> expected_ys = {1.85714286, 2.21428571, 0.07142857, 1.92857143, 2.07142857,
        5.85714286, 4.};
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected_xs(i), flock.positions.xs(i));
        TEST_ASSERT_EQUAL_FLOAT(expected_ys(i), flock.positions.ys(i));
    } 
}

void test_view_map_ints_only() {
    View view; 
    Positions positions;
    positions.xs = {0, 0, 0, 2, 1, 3, 7};
    positions.ys = {0, 1, 2, 1, 5, 5, 0};
    view.map(positions);
    BLA::Matrix<7> expectedIdxs = {0, 1, 2, 13, 6, 18, 47};
    for (int i = 0; i < 7; i++) {
        int expectedIdx = expectedIdxs(i);
        TEST_ASSERT_EQUAL_FLOAT(BRIGHTNESS_INCREMENT, view.output(expectedIdx));
    }
}

void test_view_map_rounding() {
    View view; 
    Positions positions;
    positions.xs = {-0.4, 0, 0, 2.7, 1, 3, 7};
    positions.ys = {0, 1.2, 2, 1, 5.4999, 5, 0};
    view.map(positions);
    BLA::Matrix<7> expectedIdxs = {0, 1, 2, 22, 6, 18, 47};
    for (int i = 0; i < 7; i++) {
        int expectedIdx = expectedIdxs(i);
        TEST_ASSERT_EQUAL_FLOAT(BRIGHTNESS_INCREMENT, view.output(expectedIdx));
    }
}

void test_view_map_brightness() {
    View view; 
    Positions positions;
    positions.xs = {0, 0, 0, 3, 3, 3, 3};
    positions.ys = {0, 1, 1, 1, 1, 1, 1};
    view.map(positions);
    TEST_ASSERT_EQUAL_FLOAT(BRIGHTNESS_INCREMENT, view.output(0));
    TEST_ASSERT_EQUAL_FLOAT(BRIGHTNESS_INCREMENT * 2, view.output(1));
    TEST_ASSERT_EQUAL_FLOAT(BRIGHTNESS_INCREMENT * 4, view.output(22));
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    NROWS = 7;
    NCOLS = 7;
    RUN_TEST(test_flock_size_is_correct);
    // Scenario one: no cohesion, separation, or alignment
    COHESION_STRENGTH = 0.0;
    SEPARATION_DISTANCE = 0.0;
    SEPARATION_STRENGTH = 0.0; 
    ALIGNMENT_DISTANCE = 0.0;
    ALIGNMENT_STRENGTH = 0.0; 
    RUN_TEST(test_position_update_is_working);
    // RUN_TEST(test_position_reverts_to_zero_if_past_edge_x);
    // RUN_TEST(test_position_reverts_to_zero_if_past_edge_y);
    // Scenario two: cohesion
    COHESION_STRENGTH = 0.01;
    RUN_TEST(test_position_update_with_cohesion_is_working);
    COHESION_STRENGTH = 0.1;
    RUN_TEST(test_position_update_with_cohesion_is_working_2);
    COHESION_STRENGTH = 1;
    RUN_TEST(test_position_update_with_cohesion_is_working_3);
    // Scenario three: cohesion and separation
    COHESION_STRENGTH = 0.1;
    SEPARATION_DISTANCE = 4;
    SEPARATION_STRENGTH = 0.2;
    RUN_TEST(test_position_update_with_separation_is_working);
    SEPARATION_DISTANCE = 5;
    SEPARATION_STRENGTH = 0.3;
    RUN_TEST(test_position_update_with_separation_is_working_2);
    SEPARATION_DISTANCE = 9;
    SEPARATION_STRENGTH = 0.1;
    RUN_TEST(test_position_update_with_separation_is_working_3);
    // Scenario four: cohesion, separation and alignment
    ALIGNMENT_DISTANCE = 16;
    ALIGNMENT_STRENGTH = 0.125; 
    RUN_TEST(test_position_update_with_alignment_is_working);
    // View:
    NROWS = 6;
    NCOLS = 8;
    RUN_TEST(test_view_map_ints_only);
    RUN_TEST(test_view_map_rounding);
    RUN_TEST(test_view_map_brightness);

    return UNITY_END();
}
