cmake_minimum_required(VERSION 3.12)
project(stochastic_tsp LANGUAGES CXX)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR})

set(HEADER_FILES
        src/utils/ProgramOptions.hxx
        src/utils/disable_stream.h
        src/utils/cache.h
        src/utils/tsp_bc.h
        src/utils/draw_solution.h
        src/occ/occ_exact.h
        src/occ/occ_montecarlo.h
        src/occ/occ_ml.h
        src/occ/occ_ml_rt.h
        src/occ/occ_ml_rtlr.h
        src/solvers/improving_customer.h
        src/solvers/solver_enum.h
        src/solvers/solver_fwd.h
        src/solvers/solver_bck.h
        src/solvers/solver_fwdbck.h
        src/solvers/solver_bckfwd.h
        src/solvers/randomise_set.h
        src/accepted_cost_calculator.h
        src/customer_set.h
        src/dataset.h
        src/instance.h
        src/offered_cost_calculator.h
        src/params.h
        src/solution.h
        src/solver.h
        src/tsp_solver.h)
set(TRANSLATION_UNITS
        src/utils/draw_solution.cpp
        src/utils/tsp_bc.cpp
        src/occ/occ_exact.cpp
        src/occ/occ_montecarlo.cpp
        src/occ/occ_ml.cpp
        src/occ/occ_ml_rt.cpp
        src/occ/occ_ml_rtlr.cpp
        src/solvers/improving_customer.cpp
        src/solvers/solver_enum.cpp
        src/solvers/solver_fwd.cpp
        src/solvers/solver_bck.cpp
        src/solvers/solver_fwdbck.cpp
        src/solvers/solver_bckfwd.cpp
        src/solvers/randomise_set.cpp
        src/accepted_cost_calculator.cpp
        src/main.cpp
        src/offered_cost_calculator.cpp
        src/params.cpp
        src/solution.cpp
        src/solver.cpp
        src/tsp_solver.cpp)
set(SOURCES ${HEADER_FILES} ${TRANSLATION_UNITS})

add_executable(stochastic_tsp ${SOURCES})

set(WARNING_OPTIONS             -Wall -Wextra -Werror)
set(GCC_WARNING_OPTIONS         -Wduplicated-cond -Wlogical-op -Wuseless-cast -Wrestrict)
set(ARCH_OPTIONS                -march=native)
set(RELEASE_OPTIONS             -O3)
set(DEBUG_OPTIONS               -O0 -g)
set(GDB_DEBUG_OPTIONS           -ggdb)
set(GCC_DEBUG_DEFINES           _GLIBCXX_DEBUG)
set(LINKER_OPTIONS              -flto -Wl,--no-as-needed)

configure_file(opt/ml/tree_utils.py tree_utils.py COPYONLY)

find_package(Cplex REQUIRED)
find_package(Concorde REQUIRED)
find_package(Discorde REQUIRED)
find_package(Boost REQUIRED COMPONENTS python graph filesystem)
find_package(Python3 REQUIRED COMPONENTS Development)
find_package(X11 REQUIRED)

target_compile_features(    stochastic_tsp PRIVATE cxx_std_17)
target_include_directories( stochastic_tsp SYSTEM PRIVATE "src/utils")
target_compile_definitions( stochastic_tsp PRIVATE $<$<AND:$<CONFIG:DEBUG>,$<CXX_COMPILER_ID:GNU>>:${GCC_DEBUG_DEFINES}>)
target_compile_options(     stochastic_tsp PRIVATE ${WARNING_OPTIONS} ${ARCH_OPTIONS})
target_compile_options(     stochastic_tsp PRIVATE $<$<CXX_COMPILER_ID:GNU>:${GCC_WARNING_OPTIONS}>)
target_compile_options(     stochastic_tsp PRIVATE $<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>)
target_compile_options(     stochastic_tsp PRIVATE $<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>)
target_compile_options(     stochastic_tsp PRIVATE $<$<AND:$<CONFIG:DEBUG>,$<CXX_COMPILER_ID:GNU>>:${GDB_DEBUG_OPTIONS}>)
target_link_libraries(      stochastic_tsp PRIVATE ${LINKER_OPTIONS})
target_link_libraries(      stochastic_tsp PRIVATE X11)
target_link_libraries(      stochastic_tsp PRIVATE Python3::Python)
target_link_libraries(      stochastic_tsp PRIVATE Boost::python)
target_link_libraries(      stochastic_tsp PRIVATE Boost::filesystem)
target_link_libraries(      stochastic_tsp PRIVATE Cplex::cplex)
target_link_libraries(      stochastic_tsp PRIVATE Discorde::discorde)
target_link_libraries(      stochastic_tsp PRIVATE Concorde::concorde)
target_link_libraries(      stochastic_tsp PRIVATE pthread)
target_link_libraries(      stochastic_tsp PRIVATE dl)
