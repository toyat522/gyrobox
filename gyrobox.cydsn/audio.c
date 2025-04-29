#include "audio.h"

#include "project.h"

/*
This function shuts down the PAM8302.
*/
void StopAudio() {
  AudioSD_Write(0);
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

/* [] END OF FILE */