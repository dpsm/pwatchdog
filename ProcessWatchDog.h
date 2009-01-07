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

#ifndef PROCESSWATCHDOG_H_
#define PROCESSWATCHDOG_H_

#include <qthread.h>
#include "Model.h"
class Model;

class ThreadProxy: public QThread {
public:
	static void sleep(unsigned long secs) {
		QThread::sleep(secs);
	}
	static void msleep(unsigned long msecs) {
		QThread::msleep(msecs);
	}
	static void usleep(unsigned long usecs) {
		QThread::usleep(usecs);
	}
};

class ProcessWatchDog {
public:
	ProcessWatchDog(Model* _model, int _process_id);
	virtual ~ProcessWatchDog();

	inline int getProcessId() {return this->process_id;}

	void watch();
	void cancel();
private:
	Model* model;
	int	   process_id;
};

#endif /* PROCESSWATCHDOG_H_ */
