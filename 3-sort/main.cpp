#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <iomanip>
#include <valarray>
#include <gtest/gtest.h>
#include "sort.hpp"

template<typename T>
std::vector<T> generateRandomVector(size_t size, T min_val, T max_val)
{
    std::vector<T> v(size);
    std::random_device rd;
    std::mt19937 gen(rd());

    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dis(min_val, max_val);
        for (auto& elem : v) elem = dis(gen);
    } else if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> dis(min_val, max_val);
        for (auto& elem : v) elem = dis(gen);
    }

    return v;
}

#pragma region BASIC_TESTS
class SortBasicTest : public ::testing::Test {};

TEST_F(SortBasicTest, EmptyArray) {
    std::vector<int> v;
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(v.empty());
}

TEST_F(SortBasicTest, SingleElement) {
    std::vector<int> v = {42};
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST_F(SortBasicTest, TwoElements) {
    std::vector<int> v = {2, 1};
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(SortBasicTest, AlreadySorted) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortBasicTest, ReverseSorted) {
    std::vector<int> v = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortBasicTest, RandomIntegers) {
    std::vector<int> v = {3, 7, 1, 9, 2, 8, 5, 4, 6};
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortBasicTest, WithDuplicates) {
    std::vector<int> v = {5, 2, 8, 2, 9, 1, 5, 5, 2};
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
    std::vector<int> expected = {1, 2, 2, 2, 5, 5, 5, 8, 9};
    EXPECT_EQ(v, expected);
}

TEST_F(SortBasicTest, AllSameElements) {
    std::vector<int> v = {7, 7, 7, 7, 7, 7, 7};
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortBasicTest, SmallArrayInsertionSort) {
    std::vector<int> v = {4, 2, 5, 1, 3};
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

// Type Tests
class SortTypeTest : public ::testing::Test {};

TEST_F(SortTypeTest, Strings) {
    std::vector<std::string> v = {"zebra", "apple", "banana", "cherry", "date"};
    val::sort(v.data(), v.data() + v.size(), std::less<std::string>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
    std::vector<std::string> expected = {"apple", "banana", "cherry", "date", "zebra"};
    EXPECT_EQ(v, expected);
}

TEST_F(SortTypeTest, Doubles) {
    std::vector<double> v = {3.14, 2.71, 1.41, 9.8, 0.5};
    val::sort(v.data(), v.data() + v.size(), std::less<double>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortTypeTest, GreaterComparator) {
    std::vector<int> v = {3, 7, 1, 9, 2, 8, 5, 4, 6};
    val::sort(v.data(), v.data() + v.size(), std::greater<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end(), std::greater<int>()));
}

// Large Random Data Tests
class SortLargeDataTest : public ::testing::Test {};

TEST_F(SortLargeDataTest, Random1000Elements) {
    auto v = generateRandomVector<int>(1000, -1000, 1000);
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortLargeDataTest, Random10000Elements) {
    auto v = generateRandomVector<int>(10000, -10000, 10000);
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortLargeDataTest, Random100000Elements) {
    auto v = generateRandomVector<int>(100000, -100000, 100000);
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortLargeDataTest, Random1000000Elements) {
    auto v = generateRandomVector<int>(1000000, -1000000, 1000000);
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortLargeDataTest, RandomDoubles100000) {
    auto v = generateRandomVector<double>(100000, -1000.0, 1000.0);
    val::sort(v.data(), v.data() + v.size(), std::less<double>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST_F(SortLargeDataTest, ManyDuplicates) {
    std::vector<int> v = generateRandomVector<int>(50000, 0, 100);  // Only 100 unique values
    val::sort(v.data(), v.data() + v.size(), std::less<int>());
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

#pragma endregion BASIC_TESTS

#pragma region PERF_TESTS
class SortPerformanceTest : public ::testing::Test {
protected:
    void BenchmarkSort(const std::string& test_name, std::vector<int>& v) {
        auto v_copy = v;  // Keep original for std::sort comparison

        // Time custom sort
        auto start = std::chrono::high_resolution_clock::now();
        val::sort(v.data(), v.data() + v.size(), std::less<int>());
        auto end = std::chrono::high_resolution_clock::now();
        auto custom_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));

        // Time std::sort for comparison
        start = std::chrono::high_resolution_clock::now();
        std::sort(v_copy.begin(), v_copy.end());
        end = std::chrono::high_resolution_clock::now();
        auto std_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "\n" << test_name << ":\n";
        std::cout << "  Custom sort: " << custom_duration.count() << " μs\n";
        std::cout << "  std::sort:   " << std_duration.count() << " μs\n";
        std::cout << "  Ratio:       " << (double)custom_duration.count() / std_duration.count() << "x\n";
    }
};

TEST_F(SortPerformanceTest, Benchmark_RandomData_10K) {
    auto v = generateRandomVector<int>(10000, -10000, 10000);
    BenchmarkSort("Random 10K elements", v);
}

TEST_F(SortPerformanceTest, Benchmark_RandomData_100K) {
    auto v = generateRandomVector<int>(100000, -100000, 100000);
    BenchmarkSort("Random 100K elements", v);
}

TEST_F(SortPerformanceTest, Benchmark_RandomData_1M) {
    auto v = generateRandomVector<int>(1000000, -1000000, 1000000);
    BenchmarkSort("Random 1M elements", v);
}

TEST_F(SortPerformanceTest, Benchmark_AlreadySorted_100K) {
    std::vector<int> v(100000);
    std::iota(v.begin(), v.end(), 0);
    BenchmarkSort("Already sorted 100K elements", v);
}

TEST_F(SortPerformanceTest, Benchmark_ReverseSorted_100K) {
    std::vector<int> v(100000);
    std::iota(v.begin(), v.end(), 0);
    std::reverse(v.begin(), v.end());
    BenchmarkSort("Reverse sorted 100K elements", v);
}

TEST_F(SortPerformanceTest, Benchmark_ManyDuplicates_100K) {
    auto v = generateRandomVector<int>(100000, 0, 100);
    BenchmarkSort("Many duplicates 100K elements", v);
}

TEST_F(SortPerformanceTest, Benchmark_NearlySorted_100K) {
    std::vector<int> v(100000);
    std::iota(v.begin(), v.end(), 0);
    // Randomly swap 1% of elements
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99999);
    for (int i = 0; i < 1000; ++i) {
        int idx1 = dis(gen);
        int idx2 = dis(gen);
        std::swap(v[idx1], v[idx2]);
    }
    BenchmarkSort("Nearly sorted 100K elements (1% swapped)", v);
}

#pragma endregion PERF_TESTS

#pragma region THRESHOLD_TESTS
// class ThresholdOptimizationTest : public ::testing::Test {
// protected:
//     static constexpr int NUM_RUNS = 10;
//     static constexpr int WARMUP_RUNS = 2;
//
//     struct ThresholdResult {
//         size_t threshold;
//         double avg_time_us;
//         double min_time_us;
//         double max_time_us;
//         double stddev_us;
//     };
//
//     ThresholdResult TestThreshold(size_t threshold, const std::vector<int>& original_data) {
//         std::vector<double> times;
//         times.reserve(NUM_RUNS);
//
//         // Store original threshold and set new one
//         size_t old_threshold = val::INSERTION_THRESHOLD;
//         val::INSERTION_THRESHOLD = threshold;
//
//         // Warmup runs (not measured)
//         for (int i = 0; i < WARMUP_RUNS; ++i) {
//             auto v = original_data;
//             val::sort(v.data(), v.data() + v.size(), std::less<int>());
//         }
//
//         // Measured runs
//         for (int i = 0; i < NUM_RUNS; ++i) {
//             auto v = original_data;
//
//             auto start = std::chrono::high_resolution_clock::now();
//             val::sort(v.data(), v.data() + v.size(), std::less<int>());
//             auto end = std::chrono::high_resolution_clock::now();
//
//             auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
//             times.push_back(duration.count() / 1000.0);  // Convert to microseconds
//
//             // Verify correctness
//             EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
//         }
//
//         // Restore original threshold
//         val::INSERTION_THRESHOLD = old_threshold;
//
//         // Calculate statistics
//         double sum = 0;
//         double min_val = times[0];
//         double max_val = times[0];
//
//         for (double t : times) {
//             sum += t;
//             min_val = std::min(min_val, t);
//             max_val = std::max(max_val, t);
//         }
//
//         double avg = sum / times.size();
//
//         // Calculate standard deviation
//         double sq_sum = 0;
//         for (double t : times) {
//             sq_sum += (t - avg) * (t - avg);
//         }
//         double stddev = std::sqrt(sq_sum / times.size());
//
//         return {threshold, avg, min_val, max_val, stddev};
//     }
//
//     void PrintThresholdResults(const std::string& test_name, const std::vector<ThresholdResult>& results) {
//         std::cout << "\n" << test_name << " (averaged over " << NUM_RUNS << " runs):\n";
//         std::cout << "┌──────────┬─────────────┬─────────────┬─────────────┬─────────────┐\n";
//         std::cout << "│Threshold │ Avg Time(μs)│ Min Time(μs)│ Max Time(μs)│  StdDev(μs) │\n";
//         std::cout << "├──────────┼─────────────┼─────────────┼─────────────┼─────────────┤\n";
//
//         // Find best threshold
//         auto best = std::min_element(results.begin(), results.end(),
//             [](const ThresholdResult& a, const ThresholdResult& b) {
//                 return a.avg_time_us < b.avg_time_us;
//             });
//
//         for (const auto& result : results) {
//             std::cout << "│" << std::setw(9) << result.threshold << " │";
//             std::cout << std::setw(12) << std::fixed << std::setprecision(2) << result.avg_time_us;
//
//             // Mark best with asterisk
//             if (result.threshold == best->threshold) {
//                 std::cout << "*│";
//             } else {
//                 std::cout << " │";
//             }
//
//             std::cout << std::setw(12) << std::fixed << std::setprecision(2) << result.min_time_us << " │";
//             std::cout << std::setw(12) << std::fixed << std::setprecision(2) << result.max_time_us << " │";
//             std::cout << std::setw(12) << std::fixed << std::setprecision(2) << result.stddev_us << " │\n";
//         }
//         std::cout << "└──────────┴─────────────┴─────────────┴─────────────┴─────────────┘\n";
//         std::cout << "Best threshold: " << best->threshold << " (" << std::fixed << std::setprecision(2)
//                   << best->avg_time_us << " μs)\n";
//     }
// };
//
// TEST_F(ThresholdOptimizationTest, FindOptimalThreshold_SmallArrays) {
//     // Test on arrays of size 50-100 (where threshold choice matters most)
//     std::vector<size_t> thresholds = {5, 10, 15, 20, 25, 30, 40, 50};
//     std::vector<ThresholdResult> results;
//
//     // Generate test data once
//     auto test_data = generateRandomVector<int>(100, -1000, 1000);
//
//     std::cout << "\nTesting thresholds on small arrays (size 100)...\n";
//
//     for (size_t threshold : thresholds) {
//         results.push_back(TestThreshold(threshold, test_data));
//     }
//
//     PrintThresholdResults("Small Array Threshold Optimization (100 elements)", results);
// }
//
// TEST_F(ThresholdOptimizationTest, FindOptimalThreshold_LargeArrays) {
//     // Test on large arrays
//     std::vector<size_t> thresholds = {5, 10, 15, 20, 25, 30, 40, 50, 75, 100};
//     std::vector<ThresholdResult> results;
//
//     // Generate test data once
//     auto test_data = generateRandomVector<int>(100000, -100000, 100000);
//
//     std::cout << "\nTesting thresholds on large arrays (size 100K)...\n";
//
//     for (size_t threshold : thresholds) {
//         results.push_back(TestThreshold(threshold, test_data));
//     }
//
//     PrintThresholdResults("Large Array Threshold Optimization (100K elements)", results);
// }
//
// TEST_F(ThresholdOptimizationTest, FindOptimalThreshold_MediumArrays) {
//     // Test on medium arrays
//     std::vector<size_t> thresholds = {5, 10, 15, 20, 25, 30, 40, 50};
//     std::vector<ThresholdResult> results;
//
//     // Generate test data once
//     auto test_data = generateRandomVector<int>(10000, -10000, 10000);
//
//     std::cout << "\nTesting thresholds on medium arrays (size 10K)...\n";
//
//     for (size_t threshold : thresholds) {
//         results.push_back(TestThreshold(threshold, test_data));
//     }
//
//     PrintThresholdResults("Medium Array Threshold Optimization (10K elements)", results);
// }
//
// TEST_F(ThresholdOptimizationTest, FindOptimalThreshold_AlreadySorted) {
//     // Test on already sorted data (different performance characteristics)
//     std::vector<size_t> thresholds = {5, 10, 15, 20, 25, 30, 40, 50};
//     std::vector<ThresholdResult> results;
//
//     // Generate sorted data
//     std::vector<int> test_data(50000);
//     std::iota(test_data.begin(), test_data.end(), 0);
//
//     std::cout << "\nTesting thresholds on already sorted arrays (size 50K)...\n";
//
//     for (size_t threshold : thresholds) {
//         results.push_back(TestThreshold(threshold, test_data));
//     }
//
//     PrintThresholdResults("Already Sorted Array Threshold Optimization (50K elements)", results);
// }
//
// TEST_F(ThresholdOptimizationTest, FindOptimalThreshold_ManyDuplicates) {
//     // Test on data with many duplicates
//     std::vector<size_t> thresholds = {5, 10, 15, 20, 25, 30, 40, 50};
//     std::vector<ThresholdResult> results;
//
//     // Generate data with many duplicates
//     auto test_data = generateRandomVector<int>(50000, 0, 100);  // Only 100 unique values
//
//     std::cout << "\nTesting thresholds on arrays with many duplicates (size 50K)...\n";
//
//     for (size_t threshold : thresholds) {
//         results.push_back(TestThreshold(threshold, test_data));
//     }
//
//     PrintThresholdResults("Many Duplicates Threshold Optimization (50K elements)", results);
// }
#pragma endregion

#pragma region PERF_CSV_TESTS

// TEST(SortPerformance, InsertionSortTiming) {
//     std::ofstream csv("insertion_sort_times.csv");
//     csv << "Size,Time_ms\n";
//
//     std::random_device rd;
//     std::mt19937 gen(rd());
//
//     constexpr int TIMES = 10;
//     constexpr int MIN_SIZE = 50;
//     constexpr int MAX_SIZE = 4000;
//
//     for (int size = MIN_SIZE; size <= MAX_SIZE; size+=20) {
//
//         double time_total = 0;
//
//         for (int i = 0; i < TIMES; ++i)
//         {
//             std::vector<int> data(size);
//             std::uniform_int_distribution<> dis(1, 100000);
//
//             for (int& val : data) {
//                 val = dis(gen);
//             }
//             auto start = std::chrono::high_resolution_clock::now();
//             val::InsertionSort(data.data(), data.data() + data.size(), std::less<>());
//             auto end = std::chrono::high_resolution_clock::now();
//
//             auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//             time_total = duration.count() / 1000.0;
//         }
//
//         csv << size << "," << time_total/TIMES << "\n";
//         //std::cout << "InsertionSort - Size: " << size << ", Time: " << time_total/TIMES << " ms\n";
//     }
//
//     csv.close();
//     SUCCEED();
// }
//
// TEST(SortPerformance, QuickSortTiming) {
//     std::ofstream csv("quick_sort_times.csv");
//     csv << "Size,Time_ms\n";
//
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     constexpr int TIMES = 5;
//     constexpr int MIN_SIZE = 50;
//     constexpr int MAX_SIZE = 4000;
//
//     for (int size = MIN_SIZE; size <= MAX_SIZE; size+=20) {
//         double total_time = 0;
//         for (int i = 0 ;i < TIMES; ++i)
//         {
//             std::vector<int> data(size);
//             std::uniform_int_distribution<> dis(1, 100000);
//
//             for (int& val : data) {
//                 val = dis(gen);
//             }
//             auto start = std::chrono::high_resolution_clock::now();
//             val::QuickSort(data.data(), data.data() + data.size(), std::less<>());
//             auto end = std::chrono::high_resolution_clock::now();
//
//             auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//             total_time+= duration.count() / 1000.0;
//         }
//         csv << size << "," << total_time/TIMES << "\n";
//         //std::cout << "QuickSort - Size: " << size << ", Time: " << total_time/TIMES << " ms\n";
//     }
//
//     csv.close();
//     SUCCEED();
// }

#pragma endregion

#include "../2-array/valarray.hpp"
int main(int argc, char **argv)
{

    // std::vector<int> v2{3, 7, 1, 9, 2, 8, 5, 4, 6};
    // val::QuickSort(v2.data(), v2.data()+v2.size(), std::less<>());

    Array<int> arr;
    arr.insert(3);
    arr.insert(2);
    arr.insert(5);
    arr.insert(4);
    arr.insert(1);
    arr.insert(9);
    arr.insert(2);
    arr.insert(3);
    arr.insert(4);

    std::cout << "Original custom Array: ";
    for (auto val : arr)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    //1 2 2 3 3 4 4 5 9
    val::sort(arr.beginPtr(), arr.endPtr(), std::less<int>());

    std::cout << "Sorted custom Array: ";
    for (auto val : arr)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}