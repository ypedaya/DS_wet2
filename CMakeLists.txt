cmake_minimum_required(VERSION 3.29)
project(DS_wet2_Spring_2026)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(DS_wet2_Spring_2026
    AVLtree.h
    main26b2.cpp
    Racenion26b2.cpp
    Racenion26b2.h
    wet2util.h
        DynamicArray.h
        UnionFind.h
        GroupsTreeId.h
        GroupsTreeId.cpp
        GroupsTreeMotivation.h
        GroupsTreeMotivation.cpp
        contestantsArray.h
        StructsDef.h
        contestantsArray.cpp)
