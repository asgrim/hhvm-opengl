<?php

$x = (float)($_GET['f'] ?: 1);

$gl = new OpenGL(1024, 768);

// Set the scene
$redGrey = new OpenGLColour(195, 179, 179);
$scenery = new OpenGLPolygon([
    new OpenGLCuboid(new OpenGLVertex(0.9, -1, 0 * $x), 1, 1, 1),
    new OpenGLCuboid(new OpenGLVertex(-2, -1, -0.5 * $x), 2, 2, 2),
    new OpenGLCuboid(new OpenGLVertex(-7, -1, -1 * $x), 3, 3, 3),
    new OpenGLPolygon([
        new OpenGLTriangle(new OpenGLVertex(-20, -1, -20, $redGrey), new OpenGLVertex(10, -1, 10, $redGrey), new OpenGLVertex(10, -1, -20, $redGrey)),
        new OpenGLTriangle(new OpenGLVertex(-20, -1, -20, $redGrey), new OpenGLVertex(-20, -1, 10, $redGrey), new OpenGLVertex(10, -1, 10, $redGrey)),
    ]),
]);

$camera = new OpenGLCamera(4.0, 3.0, 45.0, 0.1, 100.0); // All params optional, given here for demo
$camera->setCameraPosition(new OpenGLVertex(4, 3, 3));
$camera->setCameraTarget(new OpenGLVertex(0, 0, 0));
$camera->setYInvert(false);

$gl->setBackgroundColour(0, 0, 0.1, 1);
$gl->setCameraSettings($camera->getCameraSettings());
$gl->setVertexBuffer($scenery->getVertexBuffer());
$gl->setColourBuffer($scenery->getColourBuffer());

// Render the scene
$gl->render();

// Purposefully prevent rendering too quickly (we get black screens and too much flicker in animation)
usleep(200000);

// Grab the scene
$f = $gl->getBackBufferContent();
$c = file_get_contents($f);
$gl->close();

// Display the scene
header('Refresh:0; url=test.php?f=' . ($x + 0.3));
header('Content-Type:image/bmp');
echo $c;
