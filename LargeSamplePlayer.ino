// Sample player which uses a large amount of flash memory!

#include <Audio.h>
#include <Bounce.h>

// WAV files converted to code by wav2sketch
#include "AudioSampleThunder_strike.h"  // http://soundbible.com/2016-Thunder-Strike-2.html
#include "AudioSampleBell.h"            // http://soundbible.com/2185-Old-School-Bell.html
#include "AudioSampleEvil_laugh.h"      // http://soundbible.com/2055-Evil-Laugh-Male-6.html
#include "AudioSampleOld_car_engine.h"  // http://soundbible.com/2214-Old-Car-Starting.html
#include "AudioSampleGong.h"            // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleCash_register.h"   // http://www.freesound.org/people/kiddpark/sounds/201159/

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlayMemory    sound0;
AudioPlayMemory    sound1;  // six memory players, so we can play
AudioPlayMemory    sound2;  // all six sounds simultaneously
AudioPlayMemory    sound3;
AudioPlayMemory    sound4;
AudioPlayMemory    sound5;
AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioMixer4        mix2;    // tandem to combine 6 audio sources
AudioOutputI2S     headphones;
AudioOutputAnalog  dac;     // play to both I2S audio board and on-chip DAC

// Create Audio connections between the components
//
AudioConnection c1(sound0, 0, mix1, 0);
AudioConnection c2(sound1, 0, mix1, 1);
AudioConnection c3(sound2, 0, mix1, 2);
AudioConnection c4(sound3, 0, mix1, 3);
AudioConnection c5(mix1, 0, mix2, 0);   // output of mix1 into 1st input on mix2
AudioConnection c6(sound4, 0, mix2, 1);
AudioConnection c7(sound5, 0, mix2, 2);
AudioConnection c8(mix2, 0, headphones, 0);
AudioConnection c9(mix2, 0, headphones, 1);
AudioConnection c10(mix2, 0, dac, 0);

// Create an object to control the audio shield.
// 
AudioControlSGTL5000 audioShield;

// Bounce objects to read six pushbuttons (pins 0-5)
//
Bounce button0 = Bounce(0, 5);
Bounce button1 = Bounce(1, 5);  // 5 ms debounce time
Bounce button2 = Bounce(2, 5);
Bounce button3 = Bounce(3, 5);
Bounce button4 = Bounce(4, 5);
Bounce button5 = Bounce(5, 5);


void setup() {
  // Configure the pushbutton pins for pullups.
  // Each button should connect from the pin to GND.
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  while (!Serial && millis() < 4000) ; // wait
  Serial.println("Large Sample Player");

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(20);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);
  mix1.gain(2, 0.4);
  mix1.gain(3, 0.4);
  mix2.gain(1, 0.4);
  mix2.gain(2, 0.4);
}

void loop() {
  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();

  // When the buttons are pressed, just start a sound playing.
  // The audio library will play each sound through the mixers
  // so any combination can play simultaneously.
  //
  if (button0.fallingEdge()) {
    Serial.println("play Thunder Strike");
    sound0.play(AudioSampleThunder_strike);
  }
  if (button1.fallingEdge()) {
    Serial.println("play Bell");
    sound1.play(AudioSampleBell);
  }
  if (button2.fallingEdge()) {
    Serial.println("play Evil Laugh");
    sound2.play(AudioSampleEvil_laugh);
  }
  if (button3.fallingEdge()) {
    Serial.println("play Old Car Engine");
    sound3.play(AudioSampleOld_car_engine);
  }
  if (button4.fallingEdge()) {
    Serial.println("play Gong");
    sound4.play(AudioSampleGong);
  }
  if (button5.fallingEdge()) {
    Serial.println("play Cash Register");
    sound5.play(AudioSampleCash_register);
  }

}
