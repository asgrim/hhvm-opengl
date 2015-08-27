<?hh

<<__NativeData("OpenGL")>>
class OpenGL {
    <<__Native>>
    public function __construct(int $width, int $height): void;

    <<__Native>>
    public function render(): void;

    <<__Native>>
    public function setCameraSettings(array<float> $cameraSettings): void;

    <<__Native>>
    public function setVertexBuffer(array<float> $vertices): void;

    <<__Native>>
    public function setColourBuffer(array<float> $colours): void;

    <<__Native>>
    public function setBackgroundColour(float $r, float $g, float $b, float $a): void;

    <<__Native>>
    public function getBackBufferContent(): string;

    <<__Native>>
    public function close(): void;
}

class OpenGLColour {
    private int $r;
    private int $g;
    private int $b;

    public function __construct(int $r, int $b, int $g): void
    {
        $this->r = $this->checkRgbRange($r);
        $this->g = $this->checkRgbRange($g);
        $this->b = $this->checkRgbRange($b);
    }

    private function checkRgbRange(int $value): int
    {
        if ($value > 255 || $value < 0) {
            throw new \InvalidArgumentException('Value must be between 0-255');
        }
        return $value;
    }

    public function getFloats(): array<float>
    {
        return [
            $this->r * (1/255),
            $this->b * (1/255),
            $this->g * (1/255),
        ];
    }
}

class OpenGLVertex {
    private float $x;
    private float $y;
    private float $z;
    private OpenGLColour $colour;

    public function __construct(mixed $x, mixed $y, mixed $z, OpenGLColour $colour = null): void
    {
        $this->x = (float)$x;
        $this->y = (float)$y;
        $this->z = (float)$z;

        if ($colour !== null) {
            $this->colour = $colour;
        } else {
            $this->colour = new OpenGLColour(rand(0,255), rand(0,255), rand(0,255));
        }
    }

    public function getX()
    {
        return $this->x;
    }

    public function getY()
    {
        return $this->y;
    }

    public function getZ()
    {
        return $this->z;
    }

    public function setColour(OpenGLColour $colour): void
    {
        return $this->colour;
    }

    public function getColour(): OpenGLColour
    {
        return $this->colour;
    }
}

class OpenGLPolygon {
    private array<OpenGLVertex> $vertices;

    public function __construct(array $vertices): void
    {
        $this->addVertices($vertices);
    }

    public function addVertices(array<OpenGLVertex> $vertices): void
    {
        foreach ($vertices as $v) {
            if ($v instanceof OpenGLPolygon) {
                $this->addVertices($v->getVertices());
            } elseif ($v instanceof OpenGLVertex) {
                $this->vertices[] = $v;
            } else {
                throw new \InvalidArgumentException("Unable to add vertices");
            }
        }
    }

    public function getVertices(): array<OpenGLVertex>
    {
        return $this->vertices;
    }

    public function getVertexBuffer(): array<float>
    {
        $vertices = [];
        foreach ($this->vertices as $vertex) {
            $vertices[] = $vertex->getX();
            $vertices[] = $vertex->getY();
            $vertices[] = $vertex->getZ();
        }
        return $vertices;
    }

    public function getColourBuffer(): array<float>
    {
        $colours = [];
        foreach ($this->vertices as $vertex) {
            $colours[] = $vertex->getColour()->getFloats()[0]; //r
            $colours[] = $vertex->getColour()->getFloats()[1]; //g
            $colours[] = $vertex->getColour()->getFloats()[2]; //b
        }
        return $colours;
    }
}

class OpenGLTriangle extends OpenGLPolygon
{
    public function __construct(OpenGLVertex $a, OpenGLVertex $b, OpenGLVertex $c): void
    {
        $this->addVertices([$a, $b, $c]);
    }
}

class OpenGLCuboid extends OpenGLPolygon
{
    public function __construct(OpenGLVertex $origin, int $w, int $h, int $d): void
    {
        $oX = $origin->getX();
        $oY = $origin->getY();
        $oZ = $origin->getZ();
        $nX = $oX + $w;
        $nY = $oY + $h;
        $nZ = $oZ + $d;

        $this->addVertices([
            new OpenGLTriangle(new OpenGLVertex($oX, $oY, $oZ), new OpenGLVertex($oX, $oY, $nZ), new OpenGLVertex($oX, $nY, $nZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $nY, $oZ), new OpenGLVertex($oX, $oY, $oZ), new OpenGLVertex($oX, $nY, $oZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $oY, $nZ), new OpenGLVertex($oX, $oY, $oZ), new OpenGLVertex($nX, $oY, $oZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $nY, $oZ), new OpenGLVertex($nX, $oY, $oZ), new OpenGLVertex($oX, $oY, $oZ)),
            new OpenGLTriangle(new OpenGLVertex($oX, $oY, $oZ), new OpenGLVertex($oX, $nY, $nZ), new OpenGLVertex($oX, $nY, $oZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $oY, $nZ), new OpenGLVertex($oX, $oY, $nZ), new OpenGLVertex($oX, $oY, $oZ)),
            new OpenGLTriangle(new OpenGLVertex($oX, $nY, $nZ), new OpenGLVertex($oX, $oY, $nZ), new OpenGLVertex($nX, $oY, $nZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $nY, $nZ), new OpenGLVertex($nX, $oY, $oZ), new OpenGLVertex($nX, $nY, $oZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $oY, $oZ), new OpenGLVertex($nX, $nY, $nZ), new OpenGLVertex($nX, $oY, $nZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $nY, $nZ), new OpenGLVertex($nX, $nY, $oZ), new OpenGLVertex($oX, $nY, $oZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $nY, $nZ), new OpenGLVertex($oX, $nY, $oZ), new OpenGLVertex($oX, $nY, $nZ)),
            new OpenGLTriangle(new OpenGLVertex($nX, $nY, $nZ), new OpenGLVertex($oX, $nY, $nZ), new OpenGLVertex($nX, $oY, $nZ)),
        ]);
    }
}

class OpenGLCamera
{
    private OpenGLVertex $cameraPosition;
    private OpenGLVertex $cameraTarget;
    private bool $yInvert = false;

    private float $fovy;
    private float $aspectX;
    private float $aspectY;
    private float $zNear;
    private float $zFar;

    public function __construct(float $aspectX = 4.0, float $aspectY = 3.0, float $fovy = 45.0, float $zNear = 0.1, float $zFar = 100.0): void
    {
        $this->fovy = $fovy;
        $this->aspectX = $aspectX;
        $this->aspectY = $aspectY;
        $this->zNear = $zNear;
        $this->zFar = $zFar;

        $this->cameraPosition = new OpenGLVertex(4, 3, 3);
        $this->cameraTarget = new OpenGLVertex(0, 0, 0);
    }

    public function setCameraPosition(OpenGLVertex $cameraPosition): void
    {
        $this->cameraPosition = $cameraPosition;
    }

    public function setCameraTarget(OpenGLVertex $cameraTarget): void
    {
        $this->cameraTarget = $cameraTarget;
    }

    public function setYInvert(bool $yInvert): void
    {
        $this->yInvert = $yInvert;
    }

    public function getCameraSettings()
    {
        return [
            $this->fovy,
            $this->aspectX,
            $this->aspectY,
            $this->zNear,
            $this->zFar,
            $this->cameraPosition->getX(),
            $this->cameraPosition->getY(),
            $this->cameraPosition->getZ(),
            $this->cameraTarget->getX(),
            $this->cameraTarget->getY(),
            $this->cameraTarget->getZ(),
            $this->yInvert ? -1 : 1,
        ];
    }
}