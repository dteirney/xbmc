#pragma once
/*
 * GetNumChannelsCommand.h
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#include "MythXmlCommand.h"

class GetNumOfChannelsCommand: public MythXmlCommand
{
  public:
    GetNumOfChannelsCommand();
    ~GetNumOfChannelsCommand();

    int GetNumberOfChannels();

    bool ParseResponse(CStdString response);
  private:
    int m_number_of_channels;
};