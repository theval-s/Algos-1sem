#pragma once

#include <memory>

namespace val
{
    size_t INSERTION_THRESHOLD = 20;

    //TODO: rewrite using pointer math instead of [] op?
    //(even though techically [] is pointer math)

    template <typename T, typename Compare>
    void InsertionSort(T* first, T* last, Compare comp)
    {
        if (!first || !last) return;
        size_t len = last - first;
        for (int i = 1; i < len; i++)
        {
            T val = std::move(first[i]);
            int j = i - 1;
            for (; j >= 0 && comp(val, first[j]); j--)
            {
                first[j + 1] = std::move(first[j]);
            }
            first[j + 1] = std::move(val);
        }
    }

    template <typename T, typename Compare>
    T* GetMedian(T* first, T* last, Compare comp)
    {
        int len = last - first;
        if (len < 3) return last - 1;
        T* mid = first + (len / 2);
        //find max of 3
        if (comp(*first, *mid))
        {
            //first < mid
            if (comp(*mid, *(last - 1)))
            {
                //first < mid < last
                return mid;
            }
            if (comp(*first, *(last - 1)))
            {
                //first < last < mid
                return last - 1;
            }
            // last < first < mid
            return first;
        }
        else
        {
            //mid < first
            if (comp(*first, *(last - 1)))
            {
                //mid < first < last
                return first;
            }

            else if (comp(*mid, *(last - 1)))
            {
                //mid < last < first
                return last - 1;
            }
            return mid;
        }
    }

    template <typename T, typename Compare>
    T* HoarePartition(T* first, T* last, Compare comp)
    {
        T* pivotPos = first;

        //median
        //pivotPos = GetMedian<T>(first, last, comp);
        T pivotVal = *pivotPos;
        T* i = first - 1;
        T* j = last;

        while (true)
        {
            do
            {
                --j;
            }
            while (comp(pivotVal, *j));
            do
            {
                ++i;
            }
            while (comp(*i, pivotVal));
            if (i >= j)
                return j;
            std::swap(*i, *j);
        }
    }

    template <typename T, typename Compare>
    T* HoarePartitionWithMedian(T* first, T* last, Compare comp)
    {
        T* pivotPos = first;

        //median
        std::swap(*GetMedian<T>(first, last, comp), *first);


        T pivotVal = *pivotPos;
        T* i = first - 1;
        T* j = last;

        while (true)
        {
            do
            {
                --j;
            }
            while (comp(pivotVal, *j));
            do
            {
                ++i;
            }
            while (comp(*i, pivotVal));
            if (i >= j)
                return j;
            std::swap(*i, *j);
        }
    }

    template <typename T, typename Compare>
    T* Partition(T* first, T* last, Compare comp)
    {
        int len = last - first;

        T* pivot;


        //middle element pivot
        //pivot = first + (len / 2);

        //get median and put it to the end
        pivot = GetMedian<T>(first, last, comp);
        std::swap(*pivot, first[len - 1]);

        //last element pivot
        pivot = first + len - 1;

        int i = -1;
        for (int j = 0; j < len - 1; j++)
        {
            if (comp(first[j], *pivot))
            {
                std::swap(first[++i], first[j]);
            }
        }
        std::swap(first[++i], first[len - 1]);
        return (first + i);
    }


    template <typename T, typename Compare>
    void QuickSort(T* first, T* last, Compare comp)
    {
        if (last - first <= 1) return;
        T* q = HoarePartitionWithMedian(first, last, comp);
        QuickSort(first, q + 1, comp);
        QuickSort(q + 1, last, comp);
    }

    template <typename T, typename Compare>
    void HybridSort(T* first, T* last, Compare comp)
    {
        size_t len = last - first;
        if (len <= INSERTION_THRESHOLD)
        {
            if (len > 1)
                InsertionSort(first, last, comp);
            return;
        }
        T* q = HoarePartitionWithMedian(first, last, comp);
        //if (q - first < last - q)
        HybridSort(first, q + 1, comp);
        HybridSort(q + 1, last, comp);
    }

    template <typename T, typename Compare>
    void HybridSortNoTailRecursion(T* first, T* last, Compare comp)
    {
        while (last - first > INSERTION_THRESHOLD)
        {
            T* q = HoarePartitionWithMedian(first, last, comp);

            //left includes q, right doesnt
            size_t left = (q+1)-first;
            size_t right = last - (q+1);
            if (left < right)
            {
                HybridSortNoTailRecursion(first, q + 1, comp);
                first=q+1;
            }
            else
            {
                HybridSortNoTailRecursion(q + 1, last, comp);
                last = q + 1;
            }
        }
        if (last - first > 1)
            InsertionSort(first, last, comp);
    }

    //As with std::, last is expected to be the next pos after the final element
    template <typename T, typename Compare>
    void sort(T* first, T* last, Compare comp)
    {
        //Utilize quick sort for larger splits, and use insertion sort for small
        // HybridSortNoTailRecursion(first, last, comp);
        QuickSort(first, last, comp);
    }

}
