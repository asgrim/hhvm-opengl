<?hh

<<__NativeData("OpenGL")>>
class OpenGL {
    <<__Native>>
    public function __construct(int $width, int $height): void;

    <<__Native>>
    public function render(): void;

    <<__Native>>
    public function setVertexBuffer(): void;

    <<__Native>>
    public function setColourBuffer(): void;

    <<__Native>>
    public function setBackgroundColour(float $r, float $g, float $b, float $a): void;

    <<__Native>>
    public function getBackBufferContent(): string;

    <<__Native>>
    public function close(): void;
}
