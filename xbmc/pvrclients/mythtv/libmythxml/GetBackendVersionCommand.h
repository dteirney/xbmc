#pragma once

#include "MythXmlCommand.h"

class GetBackendVersionCommand: public MythXmlCommand
{
  public:
    GetBackendVersionCommand();
    ~GetBackendVersionCommand();

    CStdString GetVersion();
    bool ParseResponse(CStdString response);

  private:
    CStdString m_version;
};