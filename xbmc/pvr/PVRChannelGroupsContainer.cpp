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

#include "PVRChannelGroupsContainer.h"

CPVRChannelGroupsContainer g_PVRChannelGroups;

CPVRChannelGroupsContainer::CPVRChannelGroupsContainer(void)
{
}

CPVRChannelGroupsContainer::~CPVRChannelGroupsContainer(void)
{
  Unload();
}

bool CPVRChannelGroupsContainer::Load(void)
{
  bool bReturn = true;

  Unload();

  m_groupsRadio = new CPVRChannelGroups(true);
  m_groupsTV    = new CPVRChannelGroups(false);

  bReturn = m_groupsRadio->Load() || bReturn;
  bReturn = m_groupsTV->Load() || bReturn;

  return bReturn;
}

void CPVRChannelGroupsContainer::Unload(void)
{
  if (m_groupsRadio)
  {
    delete m_groupsRadio;
    m_groupsRadio = NULL;
  }

  if (m_groupsTV)
  {
    delete m_groupsTV;
    m_groupsTV = NULL;
  }
}

CPVRChannelGroups *CPVRChannelGroupsContainer::Get(bool bRadio)
{
  return bRadio ? m_groupsRadio : m_groupsTV;
}

CPVRChannelGroup *CPVRChannelGroupsContainer::GetGroupAll(bool bRadio)
{
  CPVRChannelGroup *group = NULL;

  CPVRChannelGroups *groups = Get(bRadio);
  if (groups)
    group = groups->GetGroupAll();

  return group;
}
