<?hh

<<__NativeData("OpenGL")>>
class OpenGL {
    <<__Native>>
    public function __construct(int $width, int $height): void;

    <<__Native>>
    public function render(): void;

    <<__Native>>
    public function setVertexBuffer(array<float> $v): void;

    <<__Native>>
    public function setColourBuffer(): void;

    <<__Native>>
    public function setBackgroundColour(float $r, float $g, float $b, float $a): void;

    <<__Native>>
    public function getBackBufferContent(): string;

    <<__Native>>
    public function close(): void;
}

class OpenGLVertex {
    private float $x;
    private float $y;
    private float $z;

    public function __construct(mixed $x, mixed $y, mixed $z): void
    {
        $this->x = (float)$x;
        $this->y = (float)$y;
        $this->z = (float)$z;
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
