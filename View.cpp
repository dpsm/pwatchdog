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

#include "View.h"

MainWindow::MainWindow() :
	QMainWindow(NULL, Qt::Window | Qt::WindowMinimizeButtonHint),
	AbstractProcessView()
{
	model.attachView(this);
	this->ui.setupUi(this);

	QValidator* validator = new QIntValidator(0, 99999, this);
	this->ui.procIdText->setValidator(validator);

	QObject::connect(this->ui.detachButton, SIGNAL(released())
			, this, SLOT(detachFromProcess()));
	QObject::connect(this->ui.attachButton, SIGNAL(released())
			, this, SLOT(attachToProcess()));
	QObject::connect(this->ui.newButton, SIGNAL(released())
			, this, SLOT(addNewProcess()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::detachFromProcess()
{
	QList<QListWidgetItem*> selected = this->ui.procListWidget->selectedItems();
	if (selected.size() > 0)
	{
		QListWidgetItem* item = selected.first();
		int id = item->text().toInt();
		this->model.detachFromProcess(id);
	}
}

void MainWindow::attachToProcess()
{
	QList<QListWidgetItem*> selected = this->ui.procListWidget->selectedItems();
	if (selected.size() > 0)
	{
		QListWidgetItem* item = selected.first();
		int id = item->text().toInt();
		this->model.attachToProcess(id);
	}
}

void MainWindow::addNewProcess()
{
	QString text = this->ui.procIdText->text();
	if(text.length() > 0x00)
	{
		this->ui.procListWidget->addItem(text);
	}
}

void MainWindow::processStateChanged(int process_id, AbstractProcessView::ProcessState state)
{
	QString title;
	title.append("Process [");
	title.append(QString::number(process_id));
	title.append("] State = ");

	switch(state)
	{
		case RUNNNING:
			title.append("RUNNNING");
		break;
		case FINISHED:
			title.append("FINISHED");
		break;
		case ERROR:
			title.append("ERROR");
		break;
	}
	this->setWindowTitle(title);
}
