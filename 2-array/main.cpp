//
// Created by Volkov Sergey on 11/12/2025.
//

#include <iostream>

#include <gtest/gtest.h>
#include "valarray.hpp"


#pragma region TESTS

class ArrayTest : public ::testing::Test {
protected:
    Array<int> intArray{10};
    Array<std::string> stringArray{5};
};

TEST(ArrayConstructorTest, DefaultConstructor) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayConstructorTest, CapacityConstructor) {
    Array<int> arr(20);
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayConstructorTest, CopyConstructor) {
    Array<int> arr1;
    arr1.insert(1);
    arr1.insert(2);
    arr1.insert(3);

    Array<int> arr2(arr1);
    EXPECT_EQ(arr2.size(), 3);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr2[2], 3);

    // Ensure deep copy
    arr2[0] = 99;
    EXPECT_EQ(arr1[0], 1);
}

TEST(ArrayConstructorTest, MoveConstructor) {
    Array<int> arr1;
    arr1.insert(1);
    arr1.insert(2);

    Array<int> arr2(std::move(arr1));
    EXPECT_EQ(arr2.size(), 2);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr1.size(), 0);  // arr1 should be empty after move
}

// ===== Insert Tests =====
TEST_F(ArrayTest, InsertAtEnd) {
    int idx = intArray.insert(42);
    EXPECT_EQ(intArray.size(), 1);
    EXPECT_EQ(idx, 0);
    EXPECT_EQ(intArray[0], 42);

    idx = intArray.insert(100);
    EXPECT_EQ(intArray.size(), 2);
    EXPECT_EQ(idx, 1);
    EXPECT_EQ(intArray[1], 100);
}

TEST_F(ArrayTest, InsertAtIndex) {
    intArray.insert(1);
    intArray.insert(2);
    intArray.insert(3);

    intArray.insert(1, 99);
    EXPECT_EQ(intArray.size(), 4);
    EXPECT_EQ(intArray[0], 1);
    EXPECT_EQ(intArray[1], 99);
    EXPECT_EQ(intArray[2], 2);
    EXPECT_EQ(intArray[3], 3);
}

TEST_F(ArrayTest, InsertAtBeginning) {
    intArray.insert(1);
    intArray.insert(2);

    intArray.insert(0, 99);
    EXPECT_EQ(intArray.size(), 3);
    EXPECT_EQ(intArray[0], 99);
    EXPECT_EQ(intArray[1], 1);
    EXPECT_EQ(intArray[2], 2);
}

TEST_F(ArrayTest, InsertTriggersGrowth) {
    Array<int> smallArray(2);
    smallArray.insert(1);
    smallArray.insert(2);
    smallArray.insert(3);  // Should trigger growth

    EXPECT_EQ(smallArray.size(), 3);
    EXPECT_EQ(smallArray[0], 1);
    EXPECT_EQ(smallArray[1], 2);
    EXPECT_EQ(smallArray[2], 3);
}

// ===== Remove Tests =====
TEST_F(ArrayTest, RemoveMiddleElement) {
    intArray.insert(1);
    intArray.insert(2);
    intArray.insert(3);
    intArray.insert(4);

    intArray.remove(1);
    EXPECT_EQ(intArray.size(), 3);
    EXPECT_EQ(intArray[0], 1);
    EXPECT_EQ(intArray[1], 3);
    EXPECT_EQ(intArray[2], 4);
}

TEST_F(ArrayTest, RemoveFirstElement) {
    intArray.insert(1);
    intArray.insert(2);
    intArray.insert(3);

    intArray.remove(0);
    EXPECT_EQ(intArray.size(), 2);
    EXPECT_EQ(intArray[0], 2);
    EXPECT_EQ(intArray[1], 3);
}

TEST_F(ArrayTest, RemoveLastElement) {
    intArray.insert(1);
    intArray.insert(2);
    intArray.insert(3);

    intArray.remove(2);
    EXPECT_EQ(intArray.size(), 2);
    EXPECT_EQ(intArray[0], 1);
    EXPECT_EQ(intArray[1], 2);
}

// ===== Operator Tests =====
TEST_F(ArrayTest, SubscriptOperator) {
    intArray.insert(10);
    intArray.insert(20);

    EXPECT_EQ(intArray[0], 10);
    EXPECT_EQ(intArray[1], 20);

    intArray[0] = 99;
    EXPECT_EQ(intArray[0], 99);
}

TEST_F(ArrayTest, ConstSubscriptOperator) {
    intArray.insert(10);
    const Array<int>& constRef = intArray;
    EXPECT_EQ(constRef[0], 10);
}

// ===== Iterator Tests =====
TEST_F(ArrayTest, ForwardIterator) {
    intArray.insert(1);
    intArray.insert(2);
    intArray.insert(3);

    auto it = intArray.iterator();
    EXPECT_EQ(it.get(), 1);
    EXPECT_TRUE(it.hasNext());

    it.next();
    EXPECT_EQ(it.get(), 2);

    it.next();
    EXPECT_EQ(it.get(), 3);

    it.next();
    EXPECT_FALSE(it.hasNext());
}

TEST_F(ArrayTest, ReverseIterator) {
    intArray.insert(1);
    intArray.insert(2);
    intArray.insert(3);

    auto it = intArray.reverseIterator();
    EXPECT_EQ(it.get(), 3);
    EXPECT_TRUE(it.hasNext());

    it.next();
    EXPECT_EQ(it.get(), 2);

    it.next();
    EXPECT_EQ(it.get(), 1);

    it.next();
    EXPECT_FALSE(it.hasNext());
}

TEST_F(ArrayTest, IteratorSet) {
    intArray.insert(1);
    intArray.insert(2);

    auto it = intArray.iterator();
    it.set(99);
    EXPECT_EQ(intArray[0], 99);
}

TEST_F(ArrayTest, RangeBasedForLoop) {
    intArray.insert(1);
    intArray.insert(2);
    intArray.insert(3);

    int sum = 0;
    for (auto& val : intArray) {
        sum += val;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(ArrayTest, RangeBasedForLoopModify) {
    intArray.insert(1);
    intArray.insert(2);
    intArray.insert(3);

    for (auto& val : intArray) {
        val *= 2;
    }

    EXPECT_EQ(intArray[0], 2);
    EXPECT_EQ(intArray[1], 4);
    EXPECT_EQ(intArray[2], 6);
}

TEST_F(ArrayTest, ConstIterator) {
    intArray.insert(1);
    intArray.insert(2);

    const Array<int>& constRef = intArray;
    auto it = constRef.iterator();
    EXPECT_EQ(it.get(), 1);
    it.next();
    EXPECT_EQ(it.get(), 2);
}

// ===== String Tests =====
TEST_F(ArrayTest, StringArray) {
    stringArray.insert("hello");
    stringArray.insert("world");

    EXPECT_EQ(stringArray.size(), 2);
    EXPECT_EQ(stringArray[0], "hello");
    EXPECT_EQ(stringArray[1], "world");
}

TEST_F(ArrayTest, StringArrayRemove) {
    stringArray.insert("one");
    stringArray.insert("two");
    stringArray.insert("three");

    stringArray.remove(1);
    EXPECT_EQ(stringArray.size(), 2);
    EXPECT_EQ(stringArray[0], "one");
    EXPECT_EQ(stringArray[1], "three");
}

// ===== Edge Cases =====
TEST(ArrayEdgeCaseTest, EmptyArraySize) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayEdgeCaseTest, SingleElementOperations) {
    Array<int> arr;
    arr.insert(42);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 42);

    arr.remove(0);
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayEdgeCaseTest, MultipleGrowths) {
    Array<int> arr(2);
    for (int i = 0; i < 20; ++i) {
        arr.insert(i);
    }

    EXPECT_EQ(arr.size(), 20);
    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(arr[i], i);
    }
}

struct ComplexType {
    int value;
    std::string name;

    ComplexType(int v, std::string n) : value(v), name(std::move(n)) {}

    bool operator==(const ComplexType& other) const {
        return value == other.value && name == other.name;
    }
};

TEST(ArrayComplexTypeTest, ComplexTypeInsertRemove) {
    Array<ComplexType> arr;
    arr.insert(ComplexType(1, "one"));
    arr.insert(ComplexType(2, "two"));

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].value, 1);
    EXPECT_EQ(arr[0].name, "one");

    arr.remove(0);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0].value, 2);
}

// ===== Copy Assignment Tests =====
TEST(ArrayAssignmentTest, CopyAssignmentBasic) {
    Array<int> arr1;
    arr1.insert(1);
    arr1.insert(2);
    arr1.insert(3);

    Array<int> arr2;
    arr2.insert(99);

    arr2 = arr1;

    EXPECT_EQ(arr2.size(), 3);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr2[2], 3);
    arr2[0] = 100;
    EXPECT_EQ(arr1[0], 1);  // Deep copy check
}

TEST(ArrayAssignmentTest, CopyAssignmentSelfAssignment) {
    Array<int> arr;
    arr.insert(1);
    arr = arr;
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 1);
}

// ===== Move Assignment Tests =====
TEST(ArrayAssignmentTest, MoveAssignmentBasic) {
    Array<int> arr1;
    arr1.insert(1);
    arr1.insert(2);
    arr1.insert(3);

    Array<int> arr2;
    arr2.insert(99);

    arr2 = std::move(arr1);

    EXPECT_EQ(arr2.size(), 3);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr1.size(), 0);  // arr1 should be empty
}

TEST(ArrayAssignmentTest, MoveAssignmentSelfAssignment) {
    Array<int> arr;
    arr.insert(1);
    arr = std::move(arr);
}

// ===== Advanced Iterator Tests =====
TEST(ArrayIteratorTest, IteratorOperatorPlusPlus) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    auto it = arr.begin();
    EXPECT_EQ(*it, 10);

    ++it;
    EXPECT_EQ(*it, 20);

    ++it;
    EXPECT_EQ(*it, 30);
}

TEST(ArrayIteratorTest, IteratorComparison) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);

    auto it1 = arr.begin();
    auto it2 = arr.begin();
    auto it3 = arr.end();

    EXPECT_FALSE(it1 != it2);  // Same position
    EXPECT_TRUE(it1 != it3);   // Different positions
}

TEST(ArrayIteratorTest, EmptyArrayIteration) {
    Array<int> arr;

    int count = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(ArrayIteratorTest, ConstIteratorIteration) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    const Array<int>& constArr = arr;

    std::vector<int> values;
    for (auto it = constArr.cbegin(); it != constArr.cend(); ++it) {
        values.push_back(*it);
    }

    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values[1], 2);
    EXPECT_EQ(values[2], 3);
}

TEST(ArrayIteratorTest, ReverseIteratorFullTraversal) {
    Array<int> arr;
    for (int i = 1; i <= 5; ++i) {
        arr.insert(i);
    }

    std::vector<int> values;

    for (auto it = arr.reverseIterator(); it.hasNext(); it.next()) {
        values.push_back(it.get());
    }

    EXPECT_EQ(values.size(), 5);
    EXPECT_EQ(values[0], 5);
    EXPECT_EQ(values[1], 4);
    EXPECT_EQ(values[2], 3);
    EXPECT_EQ(values[3], 2);
    EXPECT_EQ(values[4], 1);
}

TEST(ArrayIteratorTest, IteratorModifyElements) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    auto it = arr.iterator();
    it.set(100);
    it.next();
    it.set(200);
    it.next();
    it.set(300);

    EXPECT_EQ(arr[0], 100);
    EXPECT_EQ(arr[1], 200);
    EXPECT_EQ(arr[2], 300);
}

// ===== Range-Based For Loop Tests =====
TEST(ArrayRangeForTest, BasicRangeFor) {
    Array<int> arr;
    arr.insert(5);
    arr.insert(10);
    arr.insert(15);

    std::vector<int> collected;
    for (const auto& val : arr) {
        collected.push_back(val);
    }

    EXPECT_EQ(collected.size(), 3);
    EXPECT_EQ(collected[0], 5);
    EXPECT_EQ(collected[1], 10);
    EXPECT_EQ(collected[2], 15);
}

TEST(ArrayRangeForTest, RangeForModification) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    for (auto& val : arr) {
        val = val * 10;
    }

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

TEST(ArrayRangeForTest, RangeForWithStrings) {
    Array<std::string> arr;
    arr.insert("hello");
    arr.insert("world");
    arr.insert("test");

    std::string concatenated;
    for (const auto& str : arr) {
        concatenated += str + " ";
    }

    EXPECT_EQ(concatenated, "hello world test ");
}

TEST(ArrayRangeForTest, RangeForEmptyArray) {
    Array<int> arr;

    int count = 0;
    for (const auto& val : arr) {
        (void)val;  // Suppress unused variable warning
        count++;
    }

    EXPECT_EQ(count, 0);
}

TEST(ArrayRangeForTest, RangeForSingleElement) {
    Array<int> arr;
    arr.insert(42);

    int sum = 0;
    for (const auto& val : arr) {
        sum += val;
    }

    EXPECT_EQ(sum, 42);
}

TEST(ArrayRangeForTest, RangeForWithComplexType) {
    Array<ComplexType> arr;
    arr.insert(ComplexType(1, "one"));
    arr.insert(ComplexType(2, "two"));
    arr.insert(ComplexType(3, "three"));

    int sumValues = 0;
    for (const auto& item : arr) {
        sumValues += item.value;
    }

    EXPECT_EQ(sumValues, 6);
}

TEST(ArrayRangeForTest, RangeForConstArray) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    const Array<int>& constArr = arr;

    int product = 1;
    for (const auto& val : constArr) {
        product *= val;
    }

    EXPECT_EQ(product, 6000);
}

TEST(ArrayRangeForTest, RangeForBreakEarly) {
    Array<int> arr;
    for (int i = 0; i < 10; ++i) {
        arr.insert(i);
    }

    int count = 0;
    for (const auto& val : arr) {
        count++;
        if (val == 5) break;
    }

    EXPECT_EQ(count, 6);  // 0, 1, 2, 3, 4, 5
}

TEST(ArrayRangeForTest, RangeForContinue) {
    Array<int> arr;
    for (int i = 0; i < 10; ++i) {
        arr.insert(i);
    }

    int evenSum = 0;
    for (const auto& val : arr) {
        if (val % 2 != 0) continue;
        evenSum += val;
    }

    EXPECT_EQ(evenSum, 20);  // 0 + 2 + 4 + 6 + 8
}

TEST(ArrayRangeForTest, NestedRangeFor) {
    Array<Array<int>> arr;

    Array<int> arr1;
    arr1.insert(1);
    arr1.insert(2);

    Array<int> arr2;
    arr2.insert(3);
    arr2.insert(4);

    arr.insert(arr1);
    arr.insert(arr2);

    int sum = 0;
    for (const auto& innerArr : arr) {
        for (const auto& val : innerArr) {
            sum += val;
        }
    }

    EXPECT_EQ(sum, 10);  // 1 + 2 + 3 + 4
}

#pragma endregion
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
