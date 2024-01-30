# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/5.paper/code/xzxpir/build/_deps/googletest-src"
  "E:/5.paper/code/xzxpir/build/_deps/googletest-build"
  "E:/5.paper/code/xzxpir/build/_deps/googletest-subbuild/googletest-populate-prefix"
  "E:/5.paper/code/xzxpir/build/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "E:/5.paper/code/xzxpir/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "E:/5.paper/code/xzxpir/build/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "E:/5.paper/code/xzxpir/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/5.paper/code/xzxpir/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/5.paper/code/xzxpir/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
