<?php

$gl = new OpenGL();
$gl->render();
$f = $gl->getBackBufferContent();
$c = file_get_contents($f);
$gl->close();

header('Content-Type:image/bmp');
echo $c;
