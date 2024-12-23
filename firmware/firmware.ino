// !!!! select usb type: serial + midi

#include <Wire.h>
#include <SPI.h>
#include <AceButton.h>
#include "./config.h"
using namespace ace_button;

#if TEENSY == 1
#include <Audio.h>
#include <MIDI.h>
#endif

#if ESP32 == 1
#include <esp_now.h>
#include <WiFi.h>
#include <esp_now_midi.h>
esp_now_midi ESP_NOW_MIDI;
void customOnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  Serial.print("Custom Callback - Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failure");
}
#endif

AceButton _buttons[NUMBER_OF_TRACKS];
bool _buttonStates[] = { false, false, false, false, false, false, false, false };



void handleEvent(AceButton*, uint8_t, uint8_t);
void onNoteOn(byte channel, byte note, byte velocity);
void onNoteOff(byte channel, byte note, byte velocity);
void onControlChange(byte channel, byte controller, byte value);

void setup() {
  Serial.begin(115200);
  Wire.setClock(400000);  // Set I2C to 400 kHz

#if ESP32 == 1
  WiFi.mode(WIFI_STA);
  ESP_NOW_MIDI.setup(broadcastAddress, customOnDataSent);
#endif

// midi
#if TEENSY == 1
  Serial.println("setup midi engine");
  usbMIDI.setHandleNoteOff(onNoteOff);
  usbMIDI.setHandleNoteOn(onNoteOn);
  usbMIDI.setHandleControlChange(onControlChange);
#endif

  for (auto i = 0; i < NUMBER_OF_TRACKS; i++) {
    pinMode(trackPins[i], INPUT_PULLUP);

    _buttons[i].init(trackPins[i], HIGH, i);
  }
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
}

void loop() {
  auto timestamp = millis();
  usbMIDI.read();
  for (auto i = 0; i < NUMBER_OF_TRACKS; i++) {
    _buttons[i].check();
    if (digitalRead(trackPins[i]) != _buttonStates[i]) {
      _buttonStates[i] = !_buttonStates[i];
    }
  }
}


// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  // Print out a message for all events.
  // Serial.print(F("handleEvent(): eventType: "));
  // Serial.print(AceButton::eventName(eventType));
  // Serial.print(F("; buttonState: "));
  // Serial.println(buttonState);

  // Get the LED pin
  uint8_t id = button->getId();
  Serial.println(id);

  switch (eventType) {
    case AceButton::kEventPressed:
      // TODO: send note on
      Serial.println("button pressed");
#if TEENSY == 1
      usbMIDI.sendNoteOn(trackNotes[id], 127, MIDI_CHANNEL);
#endif
#if ESP32 == 1
      esp_err_t result = ESP_NOW_MIDI.sendNoteOn(trackNotes[id], 127, MIDI_CHANNEL);
#endif
      break;
    case AceButton::kEventReleased:
#if TEENSY
      usbMIDI.sendNoteOff(trackNotes[id], 127, MIDI_CHANNEL);
#endif
#if ESP32 == 1
      esp_err_t result = ESP_NOW_MIDI.sendNoteOff(trackNotes[id], 0, MIDI_CHANNEL);
#endif
      break;
  }
}


void onNoteOn(byte channel, byte note, byte velocity) {
  Serial.println("got note on");
}
void onNoteOff(byte channel, byte note, byte velocity) {
}

void onControlChange(byte channel, byte controller, byte value) {
  switch (controller) {
    case 1:
      {
        break;
      }
    case 2:
      {
        break;
      }
  }
}
