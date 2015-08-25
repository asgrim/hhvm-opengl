<?hh

<<__NativeData("OpenGL")>>
class OpenGL {
    <<__Native>>
    public function __construct(int $width, int $height): void;

    <<__Native>>
    public function render(): void;

    <<__Native>>
    public function setVertexBuffer(mixed $v): void;

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

    public function __construct(array $vertices)
    {
        $this->addVertices($vertices);
    }

    public function addVertices(array<OpenGLVertex> $vertices)
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

    public function toVerticesArray()
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
    public function __construct(OpenGLVertex $a, OpenGLVertex $b, OpenGLVertex $c)
    {
        $this->addVertices([$a, $b, $c]);
    }
}
