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
#ifndef PWATCHDOGEVENTS_H_
#define PWATCHDOGEVENTS_H_

#include <qevent.h>

class Process;
class ProcessEvent : public QEvent
{
  public:

    ProcessEvent(Process* _process, QEvent::Type _type)
    : QEvent(_type), process(_process)
    {
    }

    virtual ~ProcessEvent()
    {
      this->process = NULL;
    };

    inline Process* getSource()
    {
      return this->process;
    };

  protected:

    Process* process;
};

class ProcessAddedEvent : public ProcessEvent
{
  public:

    static QEvent::Type PROCESS_ADDED_EVENT;

    ProcessAddedEvent(Process* _process) :
      ProcessEvent(_process, PROCESS_ADDED_EVENT)
    {
    }
};

class ProcessRemovedEvent : public ProcessEvent
{
  public:

    static QEvent::Type PROCESS_REMOVED_EVENT;

    ProcessRemovedEvent(Process* _process) :
      ProcessEvent(_process, PROCESS_REMOVED_EVENT)
    {
    }
};

class ProcessChangedEvent : public ProcessEvent
{
  public:

    static QEvent::Type PROCESS_CHANGED_EVENT;

    ProcessChangedEvent(Process* _process) :
      ProcessEvent(_process, PROCESS_CHANGED_EVENT)
    {
    }
};

class ShutDownEvent : public QEvent
{
public:
  static QEvent::Type SHUTDOWN_EVENT;

  ShutDownEvent() : QEvent(SHUTDOWN_EVENT)
  {

  }
};

#endif /* PWATCHDOGEVENTS_H_ */
