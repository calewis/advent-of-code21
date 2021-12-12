find_package(Eigen3 3.3 REQUIRED NO_MODULE)
add_executable(day4 day4.cpp)
target_link_libraries(day4 fmt::fmt Eigen3::Eigen)
