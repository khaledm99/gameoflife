add_test([=[ModTest.NonNegativeValues]=]  /home/khaled/Documents/gameoflife/gol_test [==[--gtest_filter=ModTest.NonNegativeValues]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ModTest.NonNegativeValues]=]  PROPERTIES WORKING_DIRECTORY /home/khaled/Documents/gameoflife SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[ModTest.NegativeValues]=]  /home/khaled/Documents/gameoflife/gol_test [==[--gtest_filter=ModTest.NegativeValues]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ModTest.NegativeValues]=]  PROPERTIES WORKING_DIRECTORY /home/khaled/Documents/gameoflife SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  gol_test_TESTS ModTest.NonNegativeValues ModTest.NegativeValues)
