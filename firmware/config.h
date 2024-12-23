#define TEENSY 0
#define ESP32 1

#define NUMBER_OF_TRACKS 8
int trackPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
int trackNotes[] = {60, 61, 62, 63, 64, 65, 66, 67};
#define MIDI_CHANNEL 10

uint8_t broadcastAddress[6] = { 0xCC, 0x8D, 0xA2, 0x8B, 0x85, 0x1C };

