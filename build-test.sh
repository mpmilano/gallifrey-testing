LD_LIBRARY_PATH=. clang++ $* ChildProcess.cpp configuration_params.cpp run_result.cpp test_utils.cpp -ferror-limit=1 -lmutils  -pthread -L.
