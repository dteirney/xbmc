#pragma once

#include "MythXmlCommand.h"

#include <vector>

#include "SRecording.h"

class GetRecordedCommand: public MythXmlCommand
{
  public:
    GetRecordedCommand();
    ~GetRecordedCommand();

    const std::vector<SRecording>& GetRecordings();

    bool ParseResponse(const TiXmlHandle& handle);

  private:
    std::vector<SRecording> m_recordings;
};
