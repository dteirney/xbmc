#pragma once
/*
 *      Copyright (C) 2010 Alwin Esch (Team XBMC)
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "VNSISession.h"
#include "client.h"
#include <string>

class cResponsePacket;

struct SQuality
{
  std::string fe_name;
  std::string fe_status;
  uint32_t    fe_snr;
  uint32_t    fe_signal;
  uint32_t    fe_ber;
  uint32_t    fe_unc;
};

class cVNSIDemux
{
public:
  cVNSIDemux();
  ~cVNSIDemux();

  bool Open(const PVR_CHANNEL &channelinfo);
  void Close();
  bool GetStreamProperties(PVR_STREAM_PROPERTIES* props);
  void Abort();
  DemuxPacket* Read();
  bool SwitchChannel(const PVR_CHANNEL &channelinfo);
  int CurrentChannel() { return m_channel; }
  bool GetSignalStatus(PVR_SIGNAL_STATUS &qualityinfo);

protected:
  void StreamChange(cResponsePacket *resp);
  void StreamStatus(cResponsePacket *resp);
  void StreamSignalInfo(cResponsePacket *resp);
  void StreamContentInfo(cResponsePacket *resp);

private:
  bool            m_startup;
  cVNSISession    m_session;
  int             m_channel;
  int             m_StatusCount;
  std::string     m_Status;
  PVR_STREAM_PROPERTIES m_Streams;
  SQuality        m_Quality;
};
