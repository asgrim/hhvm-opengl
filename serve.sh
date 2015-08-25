#!/bin/bash

/usr/bin/hhvm \
  -d extension_dir=. \
  -d hhvm.extensions[]=hhvm_opengl.so \
  --mode server \
  -vServer.Type=fastcgi \
  -vServer.Port=4444