import 'dart:ffi'; // For FFI

final class SoundData extends Struct {
  external Pointer<Double> frequencies;

  @Int32()
  external int frequencySize;
}