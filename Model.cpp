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
#include "Model.h"

unsigned int Process::MAX_PROCESS_NAME_LENGTH = 100;

Process::Process(int _id, State _state) :
	id(_id), state(_state), watchdog(NULL)
{
	this->name = NULL;
	int size = Process::MAX_PROCESS_NAME_LENGTH * sizeof(char);
	this->name = (char*)malloc(size);
	if (this->name)
		memset(this->name, 0x00, size);
}

Process::~Process()
{
	if (this->name)
	{
		free(this->name);
		this->name = NULL;
	}
}

Model::Model()
{
}

Model::~Model()
{
}

void Model::attachView(AbstractProcessView* view)
{
	if (!this->views.contains(view))
	{
		this->views.append(view);
	}
}

void Model::detachView(AbstractProcessView* view)
{
	if (this->views.contains(view))
	{
		this->views.removeAll(view);
	}
}

void Model::processStateChanged(Process* _proc)
{
	QListIterator<AbstractProcessView*> viewsIterator(this->views);
	while (viewsIterator.hasNext())
	{
		ProcessChangedEvent* event = new ProcessChangedEvent(_proc);
		AbstractProcessView* view = viewsIterator.next();
		QApplication::instance()->postEvent(view, event);
	}

	bool shutdown = true;
	QListIterator<Process*> procsIterator(this->procs);
	while (procsIterator.hasNext())
	{
		Process* process = procsIterator.next();
		shutdown &= process->state == Process::FINISHED
				 || process->state == Process::DETACHED;
	}

	if (shutdown)
	{
		this->shutdown();
	}
}

void Model::detachFromProcess(Process* _proc)
{
	if (_proc->watchdog != NULL && _proc->state == Process::ATTACHED)
	{
		_proc->watchdog->stop();
	}
}

void Model::attachToProcess(Process* _proc)
{
	ProcessWatchDog* watchdog = new ProcessWatchDog(this, _proc);
	watchdog->start();
}

Process* Model::addNewProcess(int _id)
{
	Process* process = this->getProcess(_id);
	if (process == NULL)
	{
		process = new Process(_id, Process::DETACHED);
		this->procs.append(process);
	}
	return process;
}

Process* Model::getProcess(int _id)
{
	Process* process = NULL;

	QListIterator<Process*> iterator(this->procs);
	while (iterator.hasNext())
	{
		Process* current = iterator.next();
		if (current->id == _id)
		{
			process = current;
			break;
		}
	}
	return process;
}

void Model::shutdown()
{
	QListIterator<AbstractProcessView*> viewsIterator(this->views);
	while (viewsIterator.hasNext())
	{
		AbstractProcessView* view = viewsIterator.next();
		QApplication::instance()->postEvent(view, new ShutDownEvent());
	}

        Utils::shutDown();
}
