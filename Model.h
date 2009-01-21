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

#ifndef MODEL_H_
#define MODEL_H_

#include <qapplication.h>
#include <qlist.h>

#include "AbstractProcessView.h"
#include "PWatchdogEvents.h"
#include "ProcessWatchDog.h"
#include "Utils.h"

class ProcessWatchDog;

class Process
{
public:
	static unsigned int MAX_PROCESS_NAME_LENGTH;

	enum State
	{
		DETACHED, ATTACHED, FINISHED, FAILED_ATTACH, FAILED_WAIT
	};
	Process(int _id, State _state);
	~Process();
	ProcessWatchDog* watchdog;
	State state;
	char* name;
	int id;
};

class Model
{
public:
	Model();
	virtual ~Model();

	void processStateChanged(Process* _proc);
	void detachFromProcess(Process* _proc);
	void attachToProcess(Process* _proc);

	Process* addNewProcess(int _id);
	Process* getProcess(int _id);

	void attachView(AbstractProcessView* _view);
	void detachView(AbstractProcessView* _view);
	void shutdown();

private:
	QList<AbstractProcessView*> views;
	QList<Process*> procs;
};

#endif /* MODEL_H_ */
