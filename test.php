<?php

$gl = new OpenGL(1024, 768);

// Set the scene
$gl->setBackgroundColour(0, 0, 0.1, 1);
$gl->setVertexBuffer(new OpenGLPolygon([
    new OpenGLTriangle(new OpenGLVertex(-1, -1, -1), new OpenGLVertex(-1, -1, 1), new OpenGLVertex(-1, 1, 1)),
    new OpenGLTriangle(new OpenGLVertex(1, 1, -1), new OpenGLVertex(-1, -1, -1), new OpenGLVertex(-1, 1, -1)),
    new OpenGLTriangle(new OpenGLVertex(1, -1, 1), new OpenGLVertex(-1, -1, -1), new OpenGLVertex(1, -1, -1)),
    new OpenGLTriangle(new OpenGLVertex(1, 1, -1), new OpenGLVertex(1, -1, -1), new OpenGLVertex(-1, -1, -1)),
    new OpenGLTriangle(new OpenGLVertex(-1, -1, -1), new OpenGLVertex(-1, 1, 1), new OpenGLVertex(-1, 1, -1)),
    new OpenGLTriangle(new OpenGLVertex(1, -1, 1), new OpenGLVertex(-1, -1, 1), new OpenGLVertex(-1, -1, -1)),
    new OpenGLTriangle(new OpenGLVertex(-1, 1, 1), new OpenGLVertex(-1, -1, 1), new OpenGLVertex(1, -1, 1)),
    new OpenGLTriangle(new OpenGLVertex(1, 1, 1), new OpenGLVertex(1, -1, -1), new OpenGLVertex(1, 1, -1)),
    new OpenGLTriangle(new OpenGLVertex(1, -1, -1), new OpenGLVertex(1, 1, 1), new OpenGLVertex(1, -1, 1)),
    new OpenGLTriangle(new OpenGLVertex(1, 1, 1), new OpenGLVertex(1, 1, -1), new OpenGLVertex(-1, 1, -1)),
    new OpenGLTriangle(new OpenGLVertex(1, 1, 1), new OpenGLVertex(-1, 1, -1), new OpenGLVertex(-1, 1, 1)),
    new OpenGLTriangle(new OpenGLVertex(1, 1, 1), new OpenGLVertex(-1, 1, 1), new OpenGLVertex(1, -1, 1)),
]));
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
