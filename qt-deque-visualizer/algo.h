#pragma once

#include <deque>

template<class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1, const std::deque<T>& half2, const Comp& comparator) {
    std::deque<T> result;
    auto it1 = half1.begin();
    auto it2 = half2.begin();
    
    while (it1 != half1.end() && it2 != half2.end()) {
        if (comparator(*it1, *it2)) {
            result.push_back(*it1);
            ++it1;
        } else {
            result.push_back(*it2);
            ++it2;
        }
    }
    
    result.insert(result.end(), it1, half1.end());
    result.insert(result.end(), it2, half2.end());
    
    return result;
}

template<class T, class Comp>
std::deque<T> MergeSort(const std::deque<T>& src, const Comp& comparator) {
    if (src.size() <= 1) {
        return src;
    }
    
    auto middle = src.begin() + src.size() / 2;
    std::deque<T> half1(src.begin(), middle);
    std::deque<T> half2(middle, src.end());
    
    return Merge(MergeSort(half1, comparator), MergeSort(half2, comparator), comparator);
}
