cmake_minimum_required(VERSION 3.15)

add_executable(SingleLinkedList_UT
    ut_single_linked_list.cpp
)
target_link_libraries(SingleLinkedList_UT
    PRIVATE
        SingleLinkedList::SingleLinkedList
        gtest
        gtest_main
        DataStructures::CompilerConfig
)

add_test(NAME SingleLinkedList_UT COMMAND SingleLinkedList_UT)

add_executable(SingleLinkedList_Iterator_UT
    ut_sll_iterator.cpp
)
target_link_libraries(SingleLinkedList_Iterator_UT
    PRIVATE
        SingleLinkedList::SingleLinkedList
        gtest
        gtest_main
        DataStructures::CompilerConfig
)

add_test(NAME SingleLinkedList_Iterator_UT COMMAND SingleLinkedList_Iterator_UT)
