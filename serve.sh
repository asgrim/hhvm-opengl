#!/bin/bash

while true; do
  /usr/bin/hhvm \
    -d extension_dir=. \
    -d hhvm.extensions[]=hhvm_opengl.so \
    --mode server \
    -vServer.Type=fastcgi \
    -vServer.Port=4444
done

# debugging:
#gdb --args /usr/bin/hhvm -d extension_dir=. -d hhvm.extensions[]=hhvm_opengl.so --mode server -vServer.Type=fastcgi -vServer.Port=4444
#break src/opengl.cpp:144
#r
