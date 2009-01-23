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

Process::Process(int _id, State _state)
{
  this->watchdog = NULL;
  this->state    = _state;
  this->name     = NULL;
  this->id       = _id;

  int size = Process::MAX_PROCESS_NAME_LENGTH * sizeof(char);
  this->name = (char*) malloc(size);
  if (this->name)
  {
    memset(this->name, 0x00, size);
  }
}

Process::~Process()
{
  if (this->name)
  {
    free(this->name);
    this->watchdog = NULL;
    this->name     = NULL;
  }
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

void Model::SendViews(Process* process, QEvent::Type type)
{
  QListIterator<AbstractProcessView*> viewsIterator(this->views);
  while (viewsIterator.hasNext())
  {
    AbstractProcessView* view = viewsIterator.next();

    QEvent* event = NULL;
    if (type == ProcessAddedEvent::PROCESS_ADDED_EVENT)
    {
      event = new ProcessAddedEvent(process);
    } else
    if (type == ProcessRemovedEvent::PROCESS_REMOVED_EVENT)
    {
      event = new ProcessRemovedEvent(process);
    } else
    if (type == ProcessChangedEvent::PROCESS_CHANGED_EVENT)
    {
      event = new ProcessChangedEvent(process);
    } else
    if (type == ShutDownEvent::SHUTDOWN_EVENT)
    {
      event = new ShutDownEvent();
    }

    if (event != NULL)
      QApplication::instance()->postEvent(view, event);
  }

  if (type == ProcessChangedEvent::PROCESS_CHANGED_EVENT)
    this->CheckProcessStates();
}

void Model::AddProcess(int _id)
{
  Process* process = this->getProcess(_id);
  if (process == NULL)
  {
    process = new Process(_id, Process::DETACHED);
    this->procs.append(process);
    this->SendViews(process, ProcessAddedEvent::PROCESS_ADDED_EVENT);

    process->watchdog = new ProcessWatchDog(this, process);
    process->watchdog->start();
  }
}

void Model::RemoveProcess(int _id)
{
  Process* process = this->getProcess(_id);
  if (process != NULL)
  {
    this->procs.removeAll(process);
    this->SendViews(process, ProcessRemovedEvent::PROCESS_REMOVED_EVENT);

    ProcessWatchDog* watchdog = process->watchdog;
    if (watchdog != NULL)
      watchdog->stop();
  }
}

void Model::CheckProcessStates()
{
  bool shutdown = this->procs.count() > 0x00;

  QListIterator<Process*> procsIterator(this->procs);
  while (procsIterator.hasNext())
  {
    Process* process = procsIterator.next();
    shutdown &= process->state == Process::FINISHED;
  }

  if (shutdown)
  {
    this->SendViews(NULL, ShutDownEvent::SHUTDOWN_EVENT);
    Utils::shutDown();
  }
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
