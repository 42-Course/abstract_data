#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

struct Point {
  int x, y;

  Point(int a = 0, int b = 0) : x(a), y(b) {}

  // Comparison: equality
  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Point& other) const {
    return !(*this == other);
  }

  // Comparison: ordering (for sort, set, map, etc.)
  bool operator<(const Point& other) const {
    if (x != other.x)
      return x < other.x;
    return y < other.y;
  }

  bool operator>(const Point& other) const {
    return other < *this;
  }

  bool operator<=(const Point& other) const {
    return !(other < *this);
  }

  bool operator>=(const Point& other) const {
    return !(*this < other);
  }

  // Stream output
  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};

#endif // POINT_HPP
