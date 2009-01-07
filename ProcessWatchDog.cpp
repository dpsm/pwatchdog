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
#include "AbstractProcessView.h"
#include "ProcessWatchDog.h"

ProcessWatchDog::ProcessWatchDog(Model* _model, int _process_id)
	: model(_model), process_id(_process_id)
{
}

ProcessWatchDog::~ProcessWatchDog()
{
}

void ProcessWatchDog::watch()
{
	AbstractProcessView::ProcessState state;

	state = AbstractProcessView::RUNNNING;
	model->processStateChanged(this->process_id, state);
	ThreadProxy::sleep(3);
	state = AbstractProcessView::FINISHED;
	model->processStateChanged(this->process_id, state);
}

void ProcessWatchDog::cancel()
{

}

