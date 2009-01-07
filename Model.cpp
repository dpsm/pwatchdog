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

Model::Model()
{
}

Model::~Model()
{
}

void Model::processStateChanged(int process_id, AbstractProcessView::ProcessState state)
{
	QListIterator<AbstractProcessView*> i(this->views);
	while (i.hasNext())
	{
		i.next()->processStateChanged(process_id, state);
	}
}

void Model::attachView(AbstractProcessView* view)
{
	if(!this->views.contains(view))
	{
		this->views.append(view);
	}
}

void Model::detachView(AbstractProcessView* view)
{
	if(this->views.contains(view))
	{
		this->views.removeAll(view);
	}
}

void Model::detachFromProcess(int process_id)
{

}

void Model::attachToProcess(int process_id)
{
	ProcessWatchDog* watchdog = new ProcessWatchDog(this, process_id);
	watchdog->watch();
}

void Model::shutdown()
{
}
