#!/bin/bash -e

hphpize
cmake \
    -DCMAKE_C_FLAGS="-O0 -ggdb3" \
    -DCMAKE_CXX_FLAGS="-O0 -ggdb3" \
    -DCMAKE_BUILD_TYPE=Debug \
    .
make

printf "=================================\n"
printf "All done :D\n\n"
printf "You can now run ./serve.sh and connect Apache to FCGI port 4444\n"
printf "Alternative, you may run:\n\n"
printf "/usr/bin/hhvm \\ \n"
printf "  -d extension_dir=. \\ \n"
printf "  -d hhvm.extensions[]=hhvm_opengl.so \\ \n"
printf "  test.php\n\n"
printf "but be warned, by default, this just dumps bitmap data to your terminal...\n"
