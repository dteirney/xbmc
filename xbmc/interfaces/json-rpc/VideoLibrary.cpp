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

#include "VideoLibrary.h"
#include "JSONUtils.h"
#include "video/VideoDatabase.h"
#include "Util.h"
#include "Application.h"

using namespace Json;
using namespace JSONRPC;

JSON_STATUS CVideoLibrary::GetMovies(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  JSON_STATUS ret = OK;
  if (videodatabase.GetMoviesByWhere("videodb://", "", "", items))
    ret = GetAdditionalMovieDetails(parameterObject, items, result);

  videodatabase.Close();
  return ret;
}

JSON_STATUS CVideoLibrary::GetMovieDetails(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  int id = parameterObject["movieid"].asInt();

  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CVideoInfoTag infos;
  videodatabase.GetMovieInfo("", infos, id);
  if (infos.m_iDbId <= 0)
  {
    videodatabase.Close();
    return InvalidParams;
  }

  HandleFileItem("movieid", true, "moviedetails", CFileItemPtr(new CFileItem(infos)), parameterObject, parameterObject["fields"], result, false);

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetMovieSets(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  if (videodatabase.GetSetsNav("videodb://1/7/", items, VIDEODB_CONTENT_MOVIES))
    HandleFileItemList("setid", false, "sets", items, parameterObject, result);

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetMovieSetDetails(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  int id = parameterObject["setid"].asInt();

  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  // Get movie set details
  CVideoInfoTag infos;
  videodatabase.GetSetInfo(id, infos);
  if (infos.m_iDbId <= 0)
  {
    videodatabase.Close();
    return InvalidParams;
  }

  HandleFileItem("setid", false, "setdetails", CFileItemPtr(new CFileItem(infos)), parameterObject, parameterObject["fields"], result, false);

  // Get movies from the set
  CFileItemList items;
  JSON_STATUS ret = OK;
  if (videodatabase.GetMoviesNav("", items, -1, -1, -1, -1, -1, -1, id))
  {
    ret = GetAdditionalMovieDetails(parameterObject["movies"], items, result["setdetails"]["items"]);
  }

  videodatabase.Close();
  return ret;
}

JSON_STATUS CVideoLibrary::GetTVShows(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  if (videodatabase.GetTvShowsNav("videodb://", items))
  {
    bool additionalInfo = false;
    Json::ValueConstIterator it;
    for (it = parameterObject["fields"].begin(); it != parameterObject["fields"].end(); it++)
    {
      CStdString fieldValue = parameterObject["fields"][it.index()].asString();
      if (fieldValue == "cast")
        additionalInfo = true;
    }

    if (additionalInfo)
    {
      for (int index = 0; index < items.Size(); index++)
      {
        videodatabase.GetTvShowInfo("", *(items[index]->GetVideoInfoTag()), items[index]->GetVideoInfoTag()->m_iDbId);
      }
    }
    HandleFileItemList("tvshowid", false, "tvshows", items, parameterObject, result);
  }

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetTVShowDetails(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  int id = parameterObject["tvshowid"].asInt();

  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CVideoInfoTag infos;
  videodatabase.GetTvShowInfo("", infos, id);
  if (infos.m_iDbId <= 0)
  {
    videodatabase.Close();
    return InvalidParams;
  }

  HandleFileItem("tvshowid", true, "tvshowdetails", CFileItemPtr(new CFileItem(infos)), parameterObject, parameterObject["fields"], result, false);

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetSeasons(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  int tvshowID = parameterObject["tvshowid"].asInt();

  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  if (videodatabase.GetSeasonsNav("videodb://", items, -1, -1, -1, -1, tvshowID))
    HandleFileItemList(NULL, false, "seasons", items, parameterObject, result);

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetEpisodes(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  int tvshowID = parameterObject["tvshowid"].asInt();
  int season   = parameterObject["season"].asInt();

  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  if (videodatabase.GetEpisodesNav("videodb://", items, -1, -1, -1, -1, tvshowID, season))
  {
    bool additionalInfo = false;
    Json::ValueConstIterator it;
    for (it = parameterObject["fields"].begin(); it != parameterObject["fields"].end(); it++)
    {
      CStdString fieldValue = parameterObject["fields"][it.index()].asString();
      if (fieldValue == "cast")
        additionalInfo = true;
    }

    if (additionalInfo)
    {
      for (int index = 0; index < items.Size(); index++)
      {
        videodatabase.GetEpisodeInfo("", *(items[index]->GetVideoInfoTag()), items[index]->GetVideoInfoTag()->m_iDbId);
      }
    }
    HandleFileItemList("episodeid", true, "episodes", items, parameterObject, result);
  }

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetEpisodeDetails(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  int id = parameterObject["episodeid"].asInt();

  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CVideoInfoTag infos;
  videodatabase.GetEpisodeInfo("", infos, id);
  if (infos.m_iDbId <= 0)
  {
    videodatabase.Close();
    return InvalidParams;
  }

  HandleFileItem("episodeid", true, "episodedetails", CFileItemPtr(new CFileItem(infos)), parameterObject, parameterObject["fields"], result, false);

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetMusicVideos(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  int artistID = parameterObject["artistid"].asInt();
  int albumID  = parameterObject["albumid"].asInt();

  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  if (videodatabase.GetMusicVideosNav("videodb://", items, -1, -1, artistID, -1, -1, albumID))
    HandleFileItemList("musicvideoid", true, "musicvideos", items, parameterObject, result);

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetMusicVideoDetails(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  int id = parameterObject["musicvideoid"].asInt();

  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CVideoInfoTag infos;
  videodatabase.GetMusicVideoInfo("", infos, id);
  if (infos.m_iDbId <= 0)
  {
    videodatabase.Close();
    return InvalidParams;
  }

  HandleFileItem("musicvideoid", true, "musicvideodetails", CFileItemPtr(new CFileItem(infos)), parameterObject, parameterObject["fields"], result, false);

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetRecentlyAddedMovies(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  if (videodatabase.GetRecentlyAddedMoviesNav("videodb://", items))
  {
    bool additionalInfo = false;
    Json::ValueConstIterator it;
    for (it = parameterObject["fields"].begin(); it != parameterObject["fields"].end(); it++)
    {
      CStdString fieldValue = parameterObject["fields"][it.index()].asString();
      if (fieldValue == "cast" || fieldValue == "set" || fieldValue == "showlink")
        additionalInfo = true;
    }

    if (additionalInfo)
    {
      for (int index = 0; index < items.Size(); index++)
      {
        videodatabase.GetMovieInfo("", *(items[index]->GetVideoInfoTag()), items[index]->GetVideoInfoTag()->m_iDbId);
      }
    }
    HandleFileItemList("movieid", true, "movies", items, parameterObject, result);
  }

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetRecentlyAddedEpisodes(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  if (videodatabase.GetRecentlyAddedEpisodesNav("videodb://", items))
  {
    bool additionalInfo = false;
    Json::ValueConstIterator it;
    for (it = parameterObject["fields"].begin(); it != parameterObject["fields"].end(); it++)
    {
      CStdString fieldValue = parameterObject["fields"][it.index()].asString();
      if (fieldValue == "cast")
        additionalInfo = true;
    }

    if (additionalInfo)
    {
      for (int index = 0; index < items.Size(); index++)
      {
        videodatabase.GetEpisodeInfo("", *(items[index]->GetVideoInfoTag()), items[index]->GetVideoInfoTag()->m_iDbId);
      }
    }
    HandleFileItemList("episodeid", true, "episodes", items, parameterObject, result);
  }

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::GetRecentlyAddedMusicVideos(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  CFileItemList items;
  if (videodatabase.GetRecentlyAddedMusicVideosNav("videodb://", items))
    HandleFileItemList("musicvideoid", true, "musicvideos", items, parameterObject, result);

  videodatabase.Close();
  return OK;
}

JSON_STATUS CVideoLibrary::ScanForContent(const CStdString &method, ITransportLayer *transport, IClient *client, const Value &parameterObject, Value &result)
{
  g_application.getApplicationMessenger().ExecBuiltIn("updatelibrary(video)");
  return ACK;
}

bool CVideoLibrary::FillFileItem(const CStdString &strFilename, CFileItem &item)
{
  CVideoDatabase videodatabase;
  bool status = false;
  if (!strFilename.empty() && videodatabase.Open())
  {
    CVideoInfoTag details;
    if (videodatabase.LoadVideoInfo(strFilename, details))
    {
      item = CFileItem(details);
      status = true;
    }

    videodatabase.Close();
  }

  return status;
}

bool CVideoLibrary::FillFileItemList(const Value &parameterObject, CFileItemList &list)
{
  CVideoDatabase videodatabase;
  if (videodatabase.Open())
  {
    CStdString file   = parameterObject["file"].asString();
    int movieID       = parameterObject["movieid"].asInt();
    int episodeID     = parameterObject["episodeid"].asInt();
    int musicVideoID  = parameterObject["musicvideoid"].asInt();

    bool success = false;
    CFileItem fileItem;
    if (FillFileItem(file, fileItem))
    {
      success = true;
      list.Add(CFileItemPtr(new CFileItem(fileItem)));
    }

    if (movieID > 0)
    {
      CVideoInfoTag details;
      videodatabase.GetMovieInfo("", details, movieID);
      if (!details.IsEmpty())
      {
        list.Add(CFileItemPtr(new CFileItem(details)));
        success = true;
      }
    }
    if (episodeID > 0)
    {
      CVideoInfoTag details;
      if (videodatabase.GetEpisodeInfo("", details, episodeID) && !details.IsEmpty())
      {
        list.Add(CFileItemPtr(new CFileItem(details)));
        success = true;
      }
    }
    if (musicVideoID > 0)
    {
      CVideoInfoTag details;
      videodatabase.GetMusicVideoInfo("", details, musicVideoID);
      if (!details.IsEmpty())
      {
        list.Add(CFileItemPtr(new CFileItem(details)));
        success = true;
      }
    }

    videodatabase.Close();
    return success;
  }

  return false;
}

JSON_STATUS CVideoLibrary::GetAdditionalMovieDetails(const Value &parameterObject, CFileItemList &items, Value &result)
{
  CVideoDatabase videodatabase;
  if (!videodatabase.Open())
    return InternalError;

  bool additionalInfo = false;
  Json::ValueConstIterator it;
  for (it = parameterObject["fields"].begin(); it != parameterObject["fields"].end(); it++)
  {
    CStdString fieldValue = parameterObject["fields"][it.index()].asString();
    if (fieldValue == "cast" || fieldValue == "set" || fieldValue == "showlink")
      additionalInfo = true;
  }

  if (additionalInfo)
  {
    for (int index = 0; index < items.Size(); index++)
    {
      videodatabase.GetMovieInfo("", *(items[index]->GetVideoInfoTag()), items[index]->GetVideoInfoTag()->m_iDbId);
    }
  }
  HandleFileItemList("movieid", true, "movies", items, parameterObject, result);

  return OK;
}
