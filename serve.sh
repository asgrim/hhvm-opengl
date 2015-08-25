#!/bin/bash

/usr/bin/hhvm \
  -d extension_dir=. \
  -d hhvm.extensions[]=hhvm_opengl.so \
  --mode server \
  -vServer.Type=fastcgi \
  -vServer.Port=4444

# debugging:
# $ gdb --args /usr/bin/hhvm -d extension_dir=. -d hhvm.extensions[]=hhvm_opengl.so --mode server -vServer.Type=fastcgi -vServer.Port=4444
# $ break src/opengl.cpp:134 # breaks on L56 in opengl.cpp ...
# $ r -d extension_dir=. -d hhvm.extensions[]=hhvm_opengl.so --mode server -vServer.Type=fastcgi -vServer.Port=4444
