include_directories(${ray-tracer_SOURCE_DIR}/inc)
include_directories(${ray-tracer_SOURCE_DIR}/stb_image)

add_executable(
  raytracer
  aarect.cpp
  box.cpp
  bvh.cpp
  CMakeLists.txt
  common.cpp
  hittable_list.cpp
  main.cpp
  moving_sphere.cpp
  scenes.cpp
  sphere.cpp
  vec3.cpp)
