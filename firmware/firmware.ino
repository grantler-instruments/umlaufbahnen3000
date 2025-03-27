#include "config.h"
#include <esp_now.h>
#include <WiFi.h>
#include "esp_now_midi.h"

#include <AceButton.h>
using namespace ace_button;
AceButton _buttons[NUMBER_OF_TRACKS];


esp_now_midi ESP_NOW_MIDI;
void customOnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  // Serial.print("Custom Callback - Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failure");
}

void onNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("Note On - Channel: %d, Note: %d, Velocity: %d\n", channel, note, velocity);
}

void onNoteOff(byte channel, byte note, byte velocity) {
  Serial.printf("Note Off - Channel: %d, Note: %d, Velocity: %d\n", channel, note, velocity);
}

void onControlChange(byte channel, byte control, byte value) {
  Serial.printf("Control Change - Channel: %d, Control: %d, Value: %d\n", channel, control, value);
}

void onProgramChange(byte channel, byte program) {
  Serial.printf("Program Change - Channel: %d, Program: %d\n", channel, program);
}

void onPitchBend(byte channel, int value) {
  Serial.printf("Pitch Bend - Channel: %d, Value: %d\n", channel, value);
}
void onAfterTouch(byte channel, byte value) {
  Serial.printf("After Touch - Channel: %d, Value: %d\n", channel, value);
}
void onPolyAfterTouch(byte channel, byte note, byte value) {
  Serial.printf("Poly After Touch - Channel: %d, note: %d, Value: %d\n", channel, note, value);
}
void onStart() {
  Serial.printf("Start");
}
void onStop() {
  Serial.printf("Stop");
}
void onContinue() {
  Serial.printf("Continue");
}
void onClock() {
  Serial.printf("Clock");
}

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i < NUMBER_OF_TRACKS; i++) {
    pinMode(trackPins[i], INPUT_PULLUP);
    _buttons[i].init(trackPins[i], HIGH, i);
  }
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  // buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  // buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  // buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);

  WiFi.mode(WIFI_STA);
  ESP_NOW_MIDI.setup(broadcastAddress, customOnDataSent);
  // ESP_NOW_MIDI.setup(broadcastAddress); //or get rid of the custom send function and use the default one

  // all of these midi handlers are optional, depends on the usecase, very often you just wanna send data and not receive
  // e.g. this can be used for calibration, or maybe you wanna connect an amp via i2s and render some sound
  ESP_NOW_MIDI.setHandleNoteOn(onNoteOn);
  ESP_NOW_MIDI.setHandleNoteOff(onNoteOff);
  ESP_NOW_MIDI.setHandleControlChange(onControlChange);
  ESP_NOW_MIDI.setHandleProgramChange(onProgramChange);
  ESP_NOW_MIDI.setHandlePitchBend(onPitchBend);
  ESP_NOW_MIDI.setHandleAfterTouchChannel(onAfterTouch);
  ESP_NOW_MIDI.setHandleAfterTouchPoly(onPolyAfterTouch);
  ESP_NOW_MIDI.setHandleStart(onStart);
  ESP_NOW_MIDI.setHandleStop(onStop);
  ESP_NOW_MIDI.setHandleContinue(onContinue);
  ESP_NOW_MIDI.setHandleClock(onClock);
}

void loop() {
  for (uint8_t i = 0; i < NUMBER_OF_TRACKS; i++) {
    _buttons[i].check();
  }


  // esp_err_t result = ESP_NOW_MIDI.sendNoteOn(60, 127, 1);

  // if (result != ESP_OK) {
  //   Serial.println("Error sending the data");
  // }
  // delay(100);
  // result = ESP_NOW_MIDI.sendNoteOff(60, 0, 1);
  // delay(100);
  // result = ESP_NOW_MIDI.sendControlChange(1, 127, 1);
  // delay(100);
  // result = ESP_NOW_MIDI.sendControlChange(1, 0, 1);
  // delay(100);
  // result = ESP_NOW_MIDI.sendPitchBend(16383, 1);
  // delay(100);
  // result = ESP_NOW_MIDI.sendPitchBend(0, 1);

  // delay(2000);
}

// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events.
  auto index = button->getId();


  switch (eventType) {
    case AceButton::kEventPressed:
      {
        Serial.print("pressed: ");
        Serial.println(index);
        esp_err_t result = ESP_NOW_MIDI.sendNoteOn(trackNotes[index], 127, MIDI_CHANNEL);
        break;
      }
    case AceButton::kEventReleased:
      {
        Serial.print("released: ");
        Serial.println(index);
        esp_err_t result = ESP_NOW_MIDI.sendNoteOff(trackNotes[index], 0, MIDI_CHANNEL);
        break;
      }
  }
}