#include "audio.h"

#include "project.h"

/*
This function stops both audio DACs.
*/
void StopAudio() {
  WaveDAC_Stop();
  AudioDAC_Stop();
}

/*
This function plays waveform1 from the WaveDAC.
*/
void SendWav1() {
  // Start and select waveform1 on the WaveDAC
  WaveSelect_Write(0);
  AudioMux_Select(0);
  WaveDAC_Start();
}

/*
This function plays waveform2 from the WaveDAC.
*/
void SendWav2() {
  // Start and select waveform2 on the WaveDAC
  WaveSelect_Write(1);
  AudioMux_Select(0);
  WaveDAC_Start();
}

/* [] END OF FILE */