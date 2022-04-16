#include "assignment/min_binary_heap.hpp"

#include <algorithm>  // fill
#include <stdexcept>  // invalid_argument
#include <limits>     // numeric_limits

namespace assignment {

  MinBinaryHeap::MinBinaryHeap(int capacity) {

    if (capacity <= 0) {
      throw std::invalid_argument("capacity must be positive");
    }

    // инициализируем поля
    size_ = 0;
    capacity_ = capacity;

    // выделяем память для узлов
    data_ = new Node[capacity_];

    // заполняем массив "пустыми узлами"
    std::fill(data_, data_ + capacity_, Node{});
  }

  MinBinaryHeap::~MinBinaryHeap() {

    // обнуляем поля
    size_ = 0;
    capacity_ = 0;

    delete[] data_;
    data_ = nullptr;
  }

  bool MinBinaryHeap::Insert(int key, int value) {

    if (size_ == capacity_) {
      return false;
    }


    data_[size_] = Node(key, value);
    size_ += 1;
    sift_up(size_-1);
    return true;
  }

  std::optional<int> MinBinaryHeap::Extract() {

    if (size_ == 0) {
      // двоичная куча пустая, операция извлечения корня невозможна
      return std::nullopt;
    }


    Node temp = data_[0];
    data_[0] = data_[size_-1];
    size_ -= 1;
    heapify(0);
    return temp.value;
  }

  bool MinBinaryHeap::Remove(int key) {

    constexpr int min_key_value = std::numeric_limits<int>::min();


    if (size_ != 0) {
      auto ind = search_index(key);
      if (ind != std::nullopt) {
        data_[ind.value()].key = min_key_value;
        sift_up(ind.value());
        Extract();
        return true;
      }
    }
    return false;
  }

  void MinBinaryHeap::Clear() {
    size_ = 0;
  }

  std::optional<int> MinBinaryHeap::Search(int key) const {
    if (size_ != 0) {
      auto ind = search_index(key);
      if (ind != std::nullopt) {
        return data_[ind.value()].value;
      }
    }
    return std::nullopt;
  }

  bool MinBinaryHeap::Contains(int key) const {
    return Search(key).has_value();
  }

  bool MinBinaryHeap::IsEmpty() const {
    return size_ == 0;
  }

  int MinBinaryHeap::capacity() const {
    return capacity_;
  }

  int MinBinaryHeap::size() const {
    return size_;
  }

  // вспомогательные функции

  void MinBinaryHeap::sift_up(int index) {

    while (index != 0 && data_[index].key < data_[parent_index(index)].key) {

      std::swap(data_[index], data_[parent_index(index)]);
      index = parent_index(index);
    }
  }

  void MinBinaryHeap::heapify(int index) {

    const int left_index = left_child_index(index);
    const int right_index = right_child_index(index);

    if (left_index >= size_) {
      return;
    }

    int smallest_key_index = index;

    if (data_[left_index].key < data_[index].key) {
      smallest_key_index = left_index;
    }

    if (data_[right_index].key < data_[smallest_key_index].key) {
      smallest_key_index = right_index;
    }

    // если индекс наименьшего узла найден (не равен индексу самого узла)
    if (smallest_key_index != index) {

      // меняем местами родителя и потомка (swap)
      std::swap(data_[index], data_[smallest_key_index]);

      // рекурсивно спускаемся по куче, следуя индексу
      heapify(smallest_key_index);
    }
  }

  std::optional<int> MinBinaryHeap::search_index(int key) const {
    for (int i = 0; i < size_; i += 1) {
      if (data_[i].key == key) {
        return i;
      }
    }
    return std::nullopt;
  }

}  // namespace assignment