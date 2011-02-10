#pragma once

/*
 *      Copyright (C) 2005-2010 Team XBMC
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

#include "windows/GUIMediaWindow.h"
#include "pvr/epg/PVREpgSearchFilter.h"
#include "pvr/epg/PVREpgContainer.h"

class CGUIEPGGridContainer;

enum PVRWindow
{
  PVR_WINDOW_UNKNOWN         = 0,
  PVR_WINDOW_EPG             = 1,
  PVR_WINDOW_CHANNELS_TV     = 2,
  PVR_WINDOW_CHANNELS_RADIO  = 3,
  PVR_WINDOW_RECORDINGS      = 4,
  PVR_WINDOW_TIMERS          = 5,
  PVR_WINDOW_SEARCH          = 6
};

class CGUIWindowPVR : public CGUIMediaWindow
{
public:
  CGUIWindowPVR(void);
  virtual ~CGUIWindowPVR(void);
  virtual bool OnMessage(CGUIMessage& message);
  virtual bool OnAction(const CAction &action);
  virtual void OnWindowLoaded();
  virtual void OnWindowUnload();
  virtual void OnInitWindow();

  void UpdateData(PVRWindow update);

protected:
  virtual void GetContextButtons(int itemNumber, CContextButtons &buttons);
  virtual bool OnContextButton(int itemNumber, CONTEXT_BUTTON button);
  virtual void UpdateButtons();

private:
  bool OnMessageFocus(CGUIMessage &message);
  bool OnMessageClick(CGUIMessage &message);

  bool OnClickButton(CGUIMessage &message);
  bool OnClickListEpg(CGUIMessage &message);
  bool OnClickListChannels(CGUIMessage &message);
  bool OnClickListRecordings(CGUIMessage &message);
  bool OnClickListTimers(CGUIMessage &message);
  bool OnClickListSearch(CGUIMessage &message);

  bool ActionDeleteChannel(CFileItem *item);
  bool ActionDeleteRecording(CFileItem *item);
  bool ActionDeleteTimer(CFileItem *timer);

  bool ActionPlayChannel(unsigned int iControl, CFileItem *item);
  bool ActionPlayEpg(CFileItem *item);

  bool ActionRecord(CFileItem *item);

  bool ActionShowSearch(CFileItem *item);
  bool ActionShowTimer(CFileItem *item);

  PVRWindow m_iCurrSubTVWindow;    /* Active subwindow */
  PVRWindow m_iSavedSubTVWindow;   /* Last subwindow, required if main window is shown again */
  bool m_bShowHiddenChannels;
  bool m_bSearchStarted;
  bool m_bSearchConfirmed;
  PVREpgSearchFilter m_searchfilter;

  /* Selected item in associated list, required for subwindow change */
  int m_iSelected_GUIDE;
  int m_iSelected_CHANNELS_TV;
  int m_iSelected_CHANNELS_RADIO;
  int m_iSelected_RECORDINGS;
  CStdString m_iSelected_RECORDINGS_Path;
  int m_iSelected_TIMERS;
  int m_iSelected_SEARCH;

  SORT_ORDER m_iSortOrder_TIMERS;
  SORT_METHOD m_iSortMethod_TIMERS;
  SORT_ORDER m_iSortOrder_SEARCH;
  SORT_METHOD m_iSortMethod_SEARCH;

  int m_iGuideView;
  int m_iCurrentTVGroup;
  int m_iCurrentRadioGroup;

  void ShowEPGInfo(CFileItem *item);
  void ShowRecordingInfo(CFileItem *item);
  bool ShowTimerSettings(CFileItem *item);
  void ShowGroupManager(bool IsRadio);
  void ShowSearchResults();

  void UpdateGuide();
  void UpdateChannelsTV();
  void UpdateChannelsRadio();
  void UpdateRecordings();
  void UpdateTimers();
  void UpdateSearch();

  bool PlayFile(CFileItem *item, bool playMinimized = false);

  CGUIEPGGridContainer   *m_guideGrid;
};
