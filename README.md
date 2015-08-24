HHVM OpenGL test
================

This is not very useful at all, but just demonstrates potential of using OpenGL library in an HHVM extension.

Instructions (YMMV):
 * Need various deps (-dev packages of GLFW, GLEW, probably other stuff)
 * `git clone`
 * `./build.sh`
 * Should successfully have built `hhvm_opengl.so` now...
 * `./serve.sh` starts HHVM with the extension as FCGI server on port 4444
 * Configure apache to point to this
 * Hit the site (e.g. http://hhvm_opengl.localhost/test.php), hopefully you'll see red triangle on blue background
 
Usage:
 * Look in test.php

Resources used:
 * http://www.opengl-tutorial.org/
 * https://github.com/hhvm/extension-example
 * http://blog.golemon.com/2015/01/hhvm-extension-writing-part-iii.html
 * https://github.com/derickr/hhvm-hni-cookbook
 * https://github.com/facebook/hhvm/wiki/Extension%20API
 * http://stackoverflow.com/a/11209314
 
Example apache config:

```
<VirtualHost *:80>
  ServerName hhvm_opengl.localhost
  ProxyPassMatch ^/(.+\.(hh|php)(/.*)?)$ fcgi://127.0.0.1:4444/full/path/to/hhvm-opengl/$1
  <Directory /full/path/to/hhvm-opengl>
    DirectoryIndex test.php
  </Directory>
</VirtualHost>

```