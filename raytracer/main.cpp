#include <cmath>
#include <cstdlib>
#include <ios>
#include <iostream>

struct Vector3D {
  double x;
  double y;
  double z;

  Vector3D operator-(const Vector3D &);
  Vector3D operator+(const Vector3D &);
  friend std::ostream &operator<<(std::ostream &, const Vector3D &);
};

// operator overalod for Vector3D
std::ostream &operator<<(std::ostream &os, const Vector3D &vec) {
  os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
  return os;
}

Vector3D Vector3D::operator-(const Vector3D &vec) {
  Vector3D difference = {0, 0, 0};

  difference.x = this->x - vec.x;
  difference.y = this->y - vec.y;
  difference.z = this->z - vec.z;

  return difference;
}

Vector3D Vector3D::operator+(const Vector3D &vec) {
  Vector3D sum = {0, 0, 0};

  sum.x = this->x + vec.x;
  sum.y = this->y + vec.y;
  sum.z = this->z + vec.z;

  return sum;
}
// end

struct Ray {
  Vector3D origin;
  Vector3D direction;
};

struct Plane {
  Vector3D center;
  Vector3D normal;
};

int vector_size(Vector3D &vec) {
  int size = 0;

  size += pow(std::abs(vec.x), 2);
  size += pow(std::abs(vec.y), 2);
  size += pow(std::abs(vec.z), 2);

  return size;
}

int dot_product(Vector3D &vec1, Vector3D &vec2) {
  int prod = 0;

  prod += vec1.x * vec2.x;
  prod += vec1.y * vec2.y;
  prod += vec1.z * vec2.z;

  return prod;
}

int intersects_plane(Plane &plane, Ray &ray) {
  int denominator = dot_product(plane.normal, ray.direction);

  if (denominator < 0.1) {
    return false; // doesn't intersect
  }

  auto difference = plane.center - ray.origin;
  auto t = dot_product(difference, plane.normal) / denominator;

  return t > 0.001;
}

// sphere stuff
struct Sphere {
  Vector3D center;
  float radius;
};

Vector3D multiply_scalar(Vector3D vec, int scalar) {
  vec.x *= scalar;
  vec.y *= scalar;
  vec.z *= scalar;

  return vec;
}

bool intersects_sphere(Sphere &sphere, Ray &ray) {
  auto L = sphere.center - ray.origin;
  auto t = dot_product(L, ray.direction);
  std::cout << t << std::endl;

  // find the value of point at 't' on the ray
  Vector3D pt = ray.origin + multiply_scalar(ray.direction, t);
  std::cout << pt << std::endl;

  // find dist between center of sphere and point at t
  float dist = sqrt(std::pow(sphere.center.x - pt.x, 2) +
                    std::pow(sphere.center.y - pt.y, 2) +
                    std::pow(sphere.center.z - pt.z, 2));

  // std::cout << dist << std::endl;
  return dist <= sphere.radius;
}

int main(int argc, char *argv[]) {
  std::cout << std::boolalpha;

  Sphere s = {
      .center = {4, 5, 0},
      .radius = 2,
  };

  Ray r = {
      .origin = {0, 3, 0},
      .direction = {0.707, 0.707, 0},
  };

  // check intersection
  std::cout << intersects_sphere(s, r) << std::endl;

  // Plane plane = {
  //     .center = {0, 0, 0},
  //     .normal = {0, 1, 0},
  // };
  //
  // Ray ray = {.origin = {0, -1, 0}, .direction = {0, 1, 0}};
  Plane plane = {
      .center = {0, 0, 0},
      .normal = {0, 0, 1},
  };

  Ray ray = {.origin = {5, 5, 3}, .direction = {1, 1, 0}};

  // std::cout << intersects_plane(plane, ray) << std::endl;

  return 0;
}

//
//
//
//
//
//
//
