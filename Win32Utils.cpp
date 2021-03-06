/*
 * Copyright (c) 2009 David Marques.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <windows.h>
#include <stdlib.h>
#include <Psapi.h>

#include "Win32PowerManager.h"
#include "Utils.h"

void Utils::waitProcess(ProcessWatchDog* watchdog)
{
  Process* proc  = watchdog->getProcess();
  Model*   model = watchdog->getModel();

  HANDLE process = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc->id);
  if (process != NULL)
  {
    WCHAR buffer[Process::MAX_PROCESS_NAME_LENGTH];
    GetModuleFileNameEx(process, NULL, buffer, Process::MAX_PROCESS_NAME_LENGTH);
    wcstombs(proc->name, buffer, Process::MAX_PROCESS_NAME_LENGTH);

    proc->state = Process::ATTACHED;
    model->SendViews(proc, ProcessChangedEvent::PROCESS_CHANGED_EVENT);
    if (WaitForSingleObject(process, INFINITE) != WAIT_FAILED)
    {
      proc->state = Process::FINISHED;
      model->SendViews(proc, ProcessChangedEvent::PROCESS_CHANGED_EVENT);
    }
    else
    {
      proc->state = Process::FAILED_WAIT;
      model->SendViews(proc, ProcessChangedEvent::PROCESS_CHANGED_EVENT);
    }
    CloseHandle(process);
  }
  else
  {
    proc->state = Process::FAILED_ATTACH;
    model->SendViews(proc, ProcessChangedEvent::PROCESS_CHANGED_EVENT);
  }
}

void Utils::shutDown()
{
//    Win32PowerManager::Shutdown();
}


