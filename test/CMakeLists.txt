link_libraries(flag_enum)

add_executable(operator_overloading operator_overloading.cpp)
add_test(NAME "Operator overloading test" COMMAND operator_overloading)

add_executable(format format.cpp)
add_test(NAME "Formatting test" COMMAND format)

add_executable(format_nonmonotonic format_nonmonotonic.cpp)
add_test(NAME "Formatting with non-monotonic enum test" COMMAND format_nonmonotonic)

add_executable(contains contains.cpp)
add_test(NAME "contains() function test" COMMAND contains)