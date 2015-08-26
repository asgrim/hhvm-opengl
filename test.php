<?php

$gl = new OpenGL(1024, 768);

// Set the scene
$cube = new OpenGLCuboid(new OpenGLVertex(-1, -1, -1), 3, 2, 2);

$gl->setBackgroundColour(0, 0, 0.1, 1);
$gl->setVertexBuffer($cube->getVertexBuffer());
$gl->setColourBuffer();

// Render the scene
$gl->render();

// Grab the scene
$f = $gl->getBackBufferContent();
$c = file_get_contents($f);
$gl->close();

// Display the scene
header('Content-Type:image/bmp');
echo $c;
