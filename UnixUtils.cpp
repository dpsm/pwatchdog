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
#include <iostream>
#include <stdlib.h>

#include "Utils.h"

using std::cerr;
using std::endl;

void Utils::waitProcess(ProcessWatchDog* watchdog)
{
	Process* proc  = watchdog->getProcess();
	Model* 	 model = watchdog->getModel();

	proc->state = Process::ATTACHED;
	model->processStateChanged(proc);

	QString command = QString("bash -c \"while ps -p %1 > /dev/null; do sleep 1; done\"")
		.arg(QString::number(proc->id));

	const char* cmd = command.toAscii();
	int exit = system(cmd);
	if (exit >= 0)
	{
		proc->state = Process::FINISHED;
		model->processStateChanged(proc);
	}
	else
	{
		proc->state = Process::FAILED_WAIT;
		model->processStateChanged(proc);
	}
}

void Utils::shutDown()
{
	const char* cmd = "bash -c \"sudo shutdown -h +1\"";
	int exit = system(cmd);
	if (exit < 0)
	{
		cerr << "Unable to shutdown machine. Exit code " << exit << "." << endl;	
	}
}

