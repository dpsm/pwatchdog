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

#include <qlist.h>
#include "AbstractProcessView.h"
#include "ProcessWatchDog.h"

class Model {
public:
	Model();
	virtual ~Model();

	void processStateChanged(int process_id, AbstractProcessView::ProcessState state);
	void detachFromProcess(int process_id);
	void attachToProcess(int process_id);

	void attachView(AbstractProcessView* view);
	void detachView(AbstractProcessView* view);
	void shutdown();

private:
	QList<AbstractProcessView*> views;
};

#endif /* MODEL_H_ */
