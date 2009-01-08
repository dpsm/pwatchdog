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

#include <qthread.h>
#include "ProcessWatchDog.h"
#include "ProcessHandler.h"

ProcessWatchDog::ProcessWatchDog(Model* _model, Process* _proc) :
	model(_model), proc(_proc)
{
	this->proc->watchdog = this;
}

ProcessWatchDog::~ProcessWatchDog()
{
}

void ProcessWatchDog::run()
{
	ProcessHandler::handleProcess(this->proc, this->model);
}

void ProcessWatchDog::stop()
{
	this->terminate();
	this->wait();

	this->proc->state = Process::DETACHED;
	model->processStateChanged(this->proc);
}
