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

#ifndef ABSTRACTPROCESSVIEW_H_
#define ABSTRACTPROCESSVIEW_H_

#import <QObject.h>

class Process;

/*
 * AbstractProcessView class defines a set of virtual
 * functions that must be handled by any process view.
 */
class AbstractProcessView : public QObject
{
  public:
    virtual void ProcessAdded(Process* _process)   = 0x00;
    virtual void ProcessRemoved(Process* _process) = 0x00;
    virtual void ProcessChanged(Process* _process) = 0x00;
    virtual void ShutdownInitiated()               = 0x00;
    virtual void Display()                         = 0x00;
};

#endif /* ABSTRACTPROCESSVIEW_H_ */
