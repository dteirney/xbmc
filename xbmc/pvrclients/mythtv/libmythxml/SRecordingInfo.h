#pragma once

#include "StdString.h"

struct SRecording
{
  int          index; // TODO: Remove and have incremented in the client class instead.
  int          duration;
  int          priority;
  CStdString   title;
  CStdString   subtitle;
  CStdString   description;
  CStdString   channel_name; // TODO: rename callsign if that's what it is.
  CStdString   stream_url;
  time_t       recording_time; // TODO: rename to recstart
  // TODO: Add chanid.
  // TODO: Add recend.

  SRecording() {
    index           = -1;
    duration        = 0;
    priority      	= 0;
    recording_time 	= 0; 
  }
};
