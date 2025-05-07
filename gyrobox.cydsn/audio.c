#include "audio.h"

#include "project.h"

/*
This function shuts down the PAM8302.
*/
void StopAudio() {
  AudioSD_Write(0);
}

/*
This function sends an audio sample to the VDAC.
*/
void SendAudioSample(uint8_t sample) {
  // Wake up PAM8302
  AudioSD_Write(1);

  // Select VDAC and set its value
  AudioDAC_SetValue(sample);
  AudioMux_Select(1);
}

/*
This function plays waveform1 from the WaveDAC.
*/
void SendWav1() {
  // Wake up PAM8302
  AudioSD_Write(1);

  // Select waveform1 on the WaveDAC
  WaveSelect_Write(0);
  AudioMux_Select(0);
}

/*
This function plays waveform2 from the WaveDAC.
*/
void SendWav2() {
  // Wake up PAM8302
  AudioSD_Write(1);

  // Select waveform2 on the WaveDAC
  WaveSelect_Write(1);
  AudioMux_Select(0);
}

/*
This function sends a "beep" to the speaker.
*/
void Beep() {
  SendWav1();
  CyDelay(200);
  StopAudio();
  CyDelay(100);
  SendWav1();
  CyDelay(200);
  StopAudio();
  CyDelay(200);
}

/* [] END OF FILE */