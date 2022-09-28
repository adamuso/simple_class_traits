set -e

CXX="${CXX:-g++}" 
CXXFLAGS="${CXXFLAGS:--std=c++11}"

$CXX $CXXFLAGS generic_trait_with_output.cpp; ./a.out
$CXX $CXXFLAGS generic_trait.cpp; ./a.out
$CXX $CXXFLAGS json_serialize.cpp; ./a.out
$CXX $CXXFLAGS macro.cpp; ./a.out
$CXX $CXXFLAGS shared_ptr.cpp; ./a.out
$CXX $CXXFLAGS simple_print_generic.cpp; ./a.out
$CXX $CXXFLAGS simple_print.cpp; ./a.out
$CXX $CXXFLAGS simplest_print.cpp; ./a.out
$CXX $CXXFLAGS simple_print_generic.cpp; ./a.out
$CXX $CXXFLAGS trait_in_vector.cpp; ./a.out