<?hh

<<__NativeData("OpenGL")>>
class OpenGL {
    <<__Native>>
    public function __construct(): void;

    <<__Native>>
    public function render(): void;

    <<__Native>>
    public function setBackgroundColour(float $r, float $g, float $b, float $a): void;

    <<__Native>>
    public function getBackBufferContent(): string;

    <<__Native>>
    public function close(): void;
}
