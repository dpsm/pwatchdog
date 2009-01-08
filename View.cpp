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
	AbstractProcessView(NULL, Qt::Window | Qt::WindowMinimizeButtonHint)
{
	model.attachView(this);
	this->ui.setupUi(this);

	QValidator* validator = new QIntValidator(0, 99999, this);
	this->ui.procIdText->setValidator(validator);

	QObject::connect(this->ui.detachButton, SIGNAL(released()), this, SLOT(detachFromProcess()));
	QObject::connect(this->ui.attachButton, SIGNAL(released()), this, SLOT(attachToProcess()));
	QObject::connect(this->ui.newButton, SIGNAL(released()), this, SLOT(addNewProcess()));
	QObject::connect(this->ui.procListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(currentProcessChanged()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::currentProcessChanged()
{
	QListWidgetItem* item = this->ui.procListWidget->currentItem();
	this->ui.procIdText->setText(item->text());

	Process* process = this->model.getProcess(item->text().toInt());
	if (process != NULL) {
		updateProcessStatusIcon(process);
	}
}

void MainWindow::updateProcessStatusIcon(Process* _proc)
{
	QString tooltip;
	QString image;
	switch (_proc->state) {
		case Process::ATTACHED:
			tooltip = "Attached";
			image = QString::fromUtf8(":/images/images/attached.png");
		break;
		case Process::DETACHED:
			tooltip = "Detached";
			image = QString::fromUtf8(":/images/images/detached.png");
		break;
		case Process::FAILED_ATTACH:
		case Process::FAILED_WAIT:
			tooltip = "Failed";
			image = QString::fromUtf8(":/images/images/error.png");
		break;
		case Process::FINISHED:
			tooltip = "Finished";
			image = QString::fromUtf8(":/images/images/finished.png");
		break;
	}
	QIcon icon;
	icon.addPixmap(QPixmap(image), QIcon::Normal, QIcon::Off);
	this->ui.toolButton->setIcon(icon);
	this->ui.toolButton->setIconSize(QSize(80, 80));
	this->ui.toolButton->setToolTip(tooltip);
}

void MainWindow::detachFromProcess()
{
	QList<QListWidgetItem*> selected = this->ui.procListWidget->selectedItems();
	if (selected.size() > 0)
	{
		QListWidgetItem* item = selected.first();
		int id = item->text().toInt();
		Process* process = this->model.getProcess(id);
		if (process != NULL && process->state != Process::DETACHED)
		{
			this->model.detachFromProcess(process);
		}
	}
}

void MainWindow::attachToProcess()
{
	QList<QListWidgetItem*> selected = this->ui.procListWidget->selectedItems();
	if (selected.size() > 0)
	{
		QListWidgetItem* item = selected.first();
		int id = item->text().toInt();
		Process* process = this->model.getProcess(id);
		if (process != NULL && process->state != Process::ATTACHED)
		{
			this->model.attachToProcess(process);
		}
	}
}

void MainWindow::addNewProcess()
{
	QString text = this->ui.procIdText->text();
	if (text.length() > 0x00 && !isProcessRegistered(text))
	{
		Process* process = this->model.addNewProcess(text.toInt());
		if (process != NULL)
		{
			this->ui.procListWidget->addItem(QString::number(process->id));
		}
	}
}

bool MainWindow::isProcessRegistered(QString _text)
{
	int index = 0x00;
	int size = 0x00;

	size = this->ui.procListWidget->count();
	for (index = 0; index < size; ++index)
	{
		QListWidgetItem* item = this->ui.procListWidget->item(index);
		if (item->text() == _text)
		{
			return true;
		}
	}
	return false;
}

bool MainWindow::event(QEvent* _event)
{
	QEvent::Type type = _event->type();
	if (type == ProcessChangedEvent::PROCESS_CHANGED_EVENT)
	{
		ProcessChangedEvent* procEvent =
				static_cast<ProcessChangedEvent*> (_event);
		this->processStateChanged(procEvent);
	}
	else if (type == ShutDownEvent::SHUTDOWN_EVENT)
	{
		ShutDownEvent* procEvent = static_cast<ShutDownEvent*> (_event);
		this->shutdown(procEvent);
	}
	else
	{
		return AbstractProcessView::event(_event);
	}
	return true;
}

void MainWindow::processStateChanged(ProcessChangedEvent* _event)
{
	Process* process = _event->getSource();

	QString title("Process [%1]; State [%2].");
	QString state;
	title = title.arg(QString::number(process->id));
	switch (process->state)
	{
	case Process::DETACHED:
		state = "DETACHED";
		break;
	case Process::ATTACHED:
		state = "ATTACHED";
		break;
	case Process::FINISHED:
		state = "FINISHED";
		break;
	case Process::FAILED_ATTACH:
	case Process::FAILED_WAIT:
		state = "ERROR";
		break;
	}
	this->setWindowTitle(title.arg(state));
	this->updateProcessStatusIcon(process);
}

void MainWindow::shutdown(ShutDownEvent* _event)
{
	this->setWindowTitle("Initializing shutdown process...");
}
