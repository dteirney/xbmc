#include "ScheduleCommands1254.h"

#include <mysql/mysql.h>

#include "MythSqlResult.h"

#include "../client.h"

ScheduleCommands1254::ScheduleCommands1254()
{
}

ScheduleCommands1254::~ScheduleCommands1254()
{
}

int ScheduleCommands1254::GetNumberOfSchedules(MYSQL* conn)
{
  if (mysql_query(conn, "SELECT count(*) FROM record")) // 0 is successful
  {
    XBMC->Log(LOG_ERROR, "%s - Error querying for number of schedules. ERROR %u: %s",
              __FUNCTION__, mysql_errno(conn), mysql_error(conn));
    return 0;
  }

  MYSQL_RES* res = mysql_use_result(conn);

  int num_schedules = 0;
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(res)) != NULL)
  {
    num_schedules = atoi(row[0]);
  }
  mysql_free_result(res);
  return num_schedules;
}

bool ScheduleCommands1254::GetAllSchedules(std::vector<MythSchedule>& schedules, MYSQL* conn)
{
  if (mysql_query(conn, "SELECT r.recordid, r.type, r.inactive, r.title, r.storagegroup, r.chanid, c.channum, r.startdate, r.starttime, r.enddate, r.endtime, r.recpriority, r.autoexpire FROM record AS r INNER JOIN channel AS c ON r.chanid = c.chanid"))
  {
    XBMC->Log(LOG_ERROR, "%s - Error querying for all schedules. ERROR %u: %s",
              __FUNCTION__, mysql_errno(conn), mysql_error(conn));
    return false;
  }
  MYSQL_RES* res = mysql_use_result(conn);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(res)) != NULL)
  {
    MythSchedule sched;
    sched.m_recordid      = atoi(row[0]); // recordid
    sched.m_type          = (MythRecordingTypes)atoi(row[1]); // type. TODO: safe conversion to ENUM
    sched.m_inactive      = atoi(row[2]) != 0; // inactive
    sched.m_title         = row[3]; // title
    sched.m_storagegroup  = row[4]; // storagegroup
    sched.m_chanid        = atoi(row[5]); // chanid
    sched.m_channum       = atoi(row[6]); // channum
    sched.m_starttime     = MythSqlResult::ToDateTime(row[7], row[8]); // startdate, starttime
    sched.m_endtime       = MythSqlResult::ToDateTime(row[9], row[10]); // enddate, endtime
    sched.m_priority      = atoi(row[11]); // recpriority
    sched.m_firstday      = 0; // TODO: fill this missing field in.
    sched.m_lifetime      = atoi(row[12]); // TODO: check this is correct for this field.
    sched.m_recording     = false; // TODO: fill this missing field in. Possible?
    sched.m_repeat        = 0; // TODO: fill this missing field in. Possible?
    sched.m_repeatflags   = kAll; // TODO: determine repeat flags from type?
    schedules.push_back(sched);
  }
  mysql_free_result(res);
  return true;
}

bool ScheduleCommands1254::AddSchedule(const MythSchedule& schedule, MYSQL* conn)
{
  return false;
}

bool ScheduleCommands1254::DeleteSchedule(const MythSchedule& schedule, MYSQL* conn)
{
  return false;
}

bool ScheduleCommands1254::RenameSchedule(const MythSchedule& schedule, const CStdString& newname, MYSQL* conn)
{
  return false;
}
