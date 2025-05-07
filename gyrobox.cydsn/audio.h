#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

void StopAudio();
void SendAudioSample(uint8_t sample);
void SendWav1();
void SendWav2();
void Beep();

#endif // AUDIO_H

/* [] END OF FILE */