/*
   MIT License

   Copyright (c) 2018 gdsports625@gmail.com

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   */

/*
 * Convert qwerty USB keyboard to MIDI button controller. No hardware
 * modifications to the keyboard are needed.
 */

/* https://github.com/FortySevenEffects/arduino_midi_library/ */
#include <MIDI.h>

// 1 turns on debug, 0 off
#define DBGSERIAL if (0) SERIAL_PORT_MONITOR

#ifdef USBCON
#define MIDI_SERIAL_PORT Serial1
#else
#define MIDI_SERIAL_PORT Serial
#endif

struct MySettings : public midi::DefaultSettings
{
  static const bool Use1ByteParsing = false;
  static const unsigned SysExMaxSize = 1026; // Accept SysEx messages up to 1024 bytes long.
  static const long BaudRate = 31250;
  //static const long BaudRate = 115200;
};

//MIDI_CREATE_DEFAULT_INSTANCE();
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, MIDI_SERIAL_PORT, MIDIUART, MySettings);

// Require keyboard control library
#include <KeyboardController.h>

// Initialize USB Controller
USBHost usb;

// Attach keyboard controller to USB
KeyboardController keyboard(usb);

const byte SYSEX1[] = {
  0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xF7
};
const byte SYSEX2[] = {
  0xF0, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0xF7
};
const byte SYSEX3[] = {
  0xF0, 0x33, 0x44, 0x55, 0x66, 0x77, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x22, 0xF7,
  0xF0, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x33, 0x44, 0x55, 0x66, 0x77, 0x08, 0x09, 0x11, 0x22, 0xF7
};
const byte SYSEX4[] = {
  0xF0, 0x44, 0x55, 0x66, 0x77, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x22, 0x33, 0xF7
};
const byte SYSEX5[] = {
  0xF0, 0x55, 0x66, 0x77, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x22, 0x33, 0x44, 0xF7
};
const byte SYSEX6[] = {
  0xF0, 0x66, 0x77, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x22, 0x33, 0x44, 0x55, 0xF7
};

// This function intercepts USB key press.
// Send any MIDI message.
void keyPressed() {
  uint8_t keycode;
  DBGSERIAL.print(F("Pressed key: "));
  keycode = keyboard.getOemKey();
  DBGSERIAL.println(keycode);
  switch (keycode) {
    // Top row
    case 41:    // ESC
      MIDIUART.sendNoteOn(60, 127, 1);
      break;
      // The key with the calculator icon does not send a code.
    case 43:    // TAB
      MIDIUART.sendNoteOn(61, 127, 1);
      break;
    case 46:    // =
      MIDIUART.sendNoteOn(62, 127, 1);
      break;
      // Row 2
    case 83:    // Num Lock
      MIDIUART.sendProgramChange(1, 1);
      break;
    case 84:    // /
      MIDIUART.sendProgramChange(4, 2);
      break;
    case 85:    // *
      MIDIUART.sendProgramChange(8, 8);
      break;
    case 42:    // <- Backspace
      MIDIUART.sendProgramChange(16, 16);
      break;
      // Row 3
    case 95:    // 7
      MIDIUART.sendSongSelect(1);
      break;
    case 96:    // 8
      MIDIUART.sendSongSelect(4);
      break;
    case 97:    // 9
      MIDIUART.sendSongSelect(16);
      break;
    case 86:    // -
      MIDIUART.sendSongSelect(127);
      break;
      // Row 4
    case 92:    // 4
      MIDIUART.sendRealTime(midi::Start);
      break;
    case 93:    // 5
      MIDIUART.sendRealTime(midi::Stop);
      break;
    case 94:    // 6
      MIDIUART.sendRealTime(midi::Continue);
      break;
    case 87:    // +
      MIDIUART.sendRealTime(midi::SystemReset);
      break;
      // Row 5
    case 89:    // 1
      MIDIUART.sendSysEx(sizeof(SYSEX1), SYSEX1, true);
      break;
    case 90:    // 2
      MIDIUART.sendSysEx(sizeof(SYSEX2), SYSEX2, true);
      break;
    case 91:    // 3
      MIDIUART.sendSysEx(sizeof(SYSEX3), SYSEX3, true);
      break;
    case 88:    // Enter
      MIDIUART.sendSysEx(sizeof(SYSEX4), SYSEX4, true);
      delay(10);
      MIDIUART.sendSysEx(sizeof(SYSEX5), SYSEX5, true);
      delay(10);
      MIDIUART.sendSysEx(sizeof(SYSEX6), SYSEX6, true);
      break;
      // Bottom row 6
    case 98:    // 0
      break;
      // Double zero key sends the same code as the zero key but twice
    case 99:    // .
      break;
    default:
      DBGSERIAL.print(F("unknown keycode="));
      DBGSERIAL.println(keycode);
      break;
  }
}

// This function intercepts USB key release
// Send any MIDI message.
void keyReleased() {
  uint8_t keycode;
  DBGSERIAL.print(F("Released key: "));
  keycode = keyboard.getOemKey();
  DBGSERIAL.println(keycode);
  switch (keycode) {
    // Top row
    case 41:    // ESC
      MIDIUART.sendNoteOff(60, 0, 1);
      break;
      // The key with the calculator icon does not send a code.
    case 43:    // TAB
      MIDIUART.sendNoteOff(61, 0, 1);
      break;
    case 46:    // =
      MIDIUART.sendNoteOff(62, 0, 1);
      break;
      // Row 2
    case 83:    // Num Lock
      break;
    case 84:    // /
      break;
    case 85:    // *
      break;
    case 42:    // <- Backspace
      break;
      // Row 3
    case 95:    // 7
      break;
    case 96:    // 8
      break;
    case 97:    // 9
      break;
    case 86:    // -
      break;
      // Row 4
    case 92:    // 4
      break;
    case 93:    // 5
      break;
    case 94:    // 6
      break;
    case 87:    // +
      break;
      // Row 5
    case 89:    // 1
      break;
    case 90:    // 2
      break;
    case 91:    // 3
      break;
    case 88:    // Enter
      break;
      // Bottom row 6
    case 98:    // 0
      break;
      // Double zero key sends the same code as the zero key but twice
    case 99:    // .
      break;
    default:
      DBGSERIAL.print(F("unknown keycode="));
      DBGSERIAL.println(keycode);
      break;
  }
}

void setup()
{
  DBGSERIAL.begin( 115200 );
  DBGSERIAL.println(F("Keyboard Controller Program started"));

  if (usb.Init() == -1)
    DBGSERIAL.println(F("USB host did not start."));

  delay( 20 );
  MIDIUART.begin();           // Launch MIDI, by default listening to channel 1.
}

void loop()
{
  // Process USB tasks
  usb.Task();

  // Process MIDI input -- discard
  MIDIUART.read();
}
