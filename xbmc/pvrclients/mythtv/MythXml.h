/*
 * MythXml.h
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#ifndef XBMC_PVRCLIENTS_MYTHTV_MYTHXML_H_
#define XBMC_PVRCLIENTS_MYTHTV_MYTHXML_H_

#include "libmythxml/MythXmlCommand.h"
#include "libmythxml/MythXmlResponse.h"
#include "libmythxml/SRecording.h"
#include "libmythxml/SChannel.h"
#include "client.h"

/*! \class MythXml
  \brief Acts as the glue between the PVR Addon world and the mythXML world.
 */
class MythXml {
public:
  MythXml(CStdString& hostname, int port, CStdString& user, CStdString& pass, int pin, long timeout);
  ~MythXml();

  void Init();
  void Cleanup();
  int getNumChannels();
  PVR_ERROR requestChannelList(PVRHANDLE handle, int radio);
  PVR_ERROR requestEPGForChannel(PVRHANDLE handle, const PVR_CHANNEL &channel, time_t start, time_t end);
  int getNumRecordings();
  PVR_ERROR requestRecordingsList(PVRHANDLE handle);
  PVR_ERROR getBackendTime(time_t *localTime, int *gmtOffset);
  CStdString GetBackendName();
  CStdString GetBackendVersion();
  CStdString GetConnectionString();
  PVR_ERROR GetDriveSpace(long long *total, long long *used);

private:
  bool ExecuteCommand(MythXmlCommand& command);
  bool checkConnection();
  CStdString GetRecordingPath(const SRecording &recording);
  CStdString GetLiveTvPath(const SChannel &channel);
  CStdString GetChannelIconPath(const SChannel &channel);
  CStdString GetUrlPrefix();

  CStdString hostname_;
  CStdString username_;
  CStdString password_;
  int port_;
  int timeout_;
  int pin_;
};

#endif /* XBMC_PVRCLIENTS_MYTHTV_MYTHXML_H_ */