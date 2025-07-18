#include "audio.h"

#include "FS.h"
#include "config.h"
#include "project.h"
#include "sfx.h"

/*
This function starts the audio with the given audio name.
*/
void StartAudio(const char *audioname) {
  if (strcmp(audioname, DEFAULT_AUDIO) == 0) {
    // If the first audio is selected, play the default SFX
    AudioSampleISR_StartAudioFlash(SFX_RAW, SFX_RAW_LEN);
  } else {
    // Otherwise, fetch audio data from the SD card
    AudioSampleISR_StartAudioSD(audioname);
  }
}

/*
This function stops the audio with the given audio name.
*/
void StopAudio(const char *audioname) {
  if (strcmp(audioname, DEFAULT_AUDIO) == 0) {
    // If default audio was playing, stop regularly
    AudioSampleISR_StopAudio();
  } else {
    // Otherwise, also close the file
    AudioSampleISR_StopAudioSD(audioname);
  }
}

/*
This function populates the audionames array with the
list of audios available and also the number of files.
*/
void GetAudioNames(char audionames[MAX_FILES][FILENAME_BUF], int *numFiles) {
  // Calculate number of files
  int num = 0;

  // Add a default SFX entry to audionames
  strcpy(audionames[num++], DEFAULT_AUDIO);

  // Iterate through files in the sd card
  FS_FIND_DATA fd;
  char acFilename[FILENAME_BUF];
  if (FS_FindFirstFile(&fd, "", acFilename, sizeof(acFilename)) == 0) {
    do {
      // Truncate the last four characters
      size_t len = strlen(acFilename);
      if (len > 4)
        acFilename[len - 4] = '\0';

      // Compute the number of files and add audio file names to array
      if (!(fd.Attributes & FS_ATTR_DIRECTORY) && num < MAX_FILES) {
        strcpy(audionames[num++], acFilename);
      }
    } while (FS_FindNextFile(&fd));
  }
  FS_FindClose(&fd);

  // Update the value at pointer location
  *numFiles = num;
}

/*
This function shuts down the PAM8302.
*/
void ShutdownAmp() { AudioSD_Write(0); }

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
  ShutdownAmp();
  CyDelay(100);
  SendWav1();
  CyDelay(200);
  ShutdownAmp();
  CyDelay(200);
}

/* [] END OF FILE */