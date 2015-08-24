#!/bin/bash

hphpize
cmake .
make

/usr/bin/hhvm \
  -d extension_dir=. \
  -d hhvm.extensions[]=hhvm_opengl.so \
  test.php
