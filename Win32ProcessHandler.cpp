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
#include "ProcessHandler.h"
#include <windows.h>

void ProcessHandler::handleProcess(ProcessWatchDog* watchdog)
{
	Process* proc  = watchdog->getProcess();
	Model* 	 model = watchdog->getModel();

	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, proc->id);
	if (process != NULL)
	{
		proc->state = Process::ATTACHED;
		model->processStateChanged(proc);
		if (WaitForSingleObject(process, INFINITE) != WAIT_FAILED)
		{
			proc->state = Process::FINISHED;
			model->processStateChanged(proc);
		}
		else
		{
			proc->state = Process::FAILED_WAIT;
			model->processStateChanged(proc);
		}
		CloseHandle(process);
	}
	else
	{
		proc->state = Process::FAILED_ATTACH;
		model->processStateChanged(proc);
	}
}
