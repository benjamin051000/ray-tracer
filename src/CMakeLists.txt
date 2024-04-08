include_directories(${RayTracing_SOURCE_DIR}/inc)

add_executable(
  raytracer
  aabb.cpp
  aarect.cpp
  box.cpp
  bvh.cpp
  CMakeLists.txt
  common.cpp
  hittable_list.cpp
  main.cpp
  moving_sphere.cpp
  ray.cpp
  scenes.cpp
  sphere.cpp
  vec3.cpp)
