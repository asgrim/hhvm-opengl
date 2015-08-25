<?php

$gl = new OpenGL(1024, 768);

$c = new OpenGLCoordinate(1, 1, 1);

// Set the scene
$gl->setBackgroundColour(0, 0, 0.1, 1);
$gl->setVertexBuffer();
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
