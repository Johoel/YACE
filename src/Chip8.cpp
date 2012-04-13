#include "../include/Chip8.h"

namespace YACE
{
  Chip8::Chip8() : cpu(CPU(*this)), cpu_cycles(400), delay_timer(0), sound_timer(0), key_is_pressed(false), last_key_pressed(KEY_0)
  {
    reset();
  }

  /*
   *  Private methods
   */

  /**
   *  Resets video.
   */
  void Chip8::reset_video()
  {
    video_mode = VIDEO_MODES.CHIP8;
    memset(video, 0, 0x2000);
  }

  /**
   *  Setup fonts
   */
  void Chip8::setup_fonts()
  {
  }

  /*
   * Public methods
   */
  /**
   *  Loads a game into memory.
   */
  void Chip8::load_game(const char* file)
  {
    FILE* input = fopen(file, "rb");
    if (input)
    {
      fseek(input, 0, SEEK_END);
      int length = ftell(input);
      fseek(input, 0, SEEK_SET);

      fread(&memory[0x200], 1, length, input);
      fclose(input);
    }
    else
      throw "Couldn't open specified file!";
  }

  /**
    * Resets emulator to known values.
    */
  void Chip8::reset()
  {
    // Reset CPU
    cpu.reset();

    reset_video();

    // Reset memory
    for (int i = 0; i < 0x1000; i++)
      memory[i] = 0;

    // Reset keys
    for (int i = 0; i < 16; i++)
      keys[i] = 0;
  }

  /**
   *  Sets key state of given key
   */
  void Chip8::set_key(EMU_KEYS key, bool pressed)
  {
    keys[key - 1] = pressed;

    if (pressed)
    {
      key_is_pressed = true;
      last_key_pressed = key - 1;
    }
  }

  /**
   * Steps the emulator.
   */
  void Chip8::step()
  {
    cpu.execute(cpu_cycles);

    if (delay_timer > 0)
      delay_timer--;

    if (sound_timer > 0)
      sound_timer--;
  }
}
