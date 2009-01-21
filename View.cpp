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
	AbstractProcessView(NULL, Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint)
{
	model.attachView(this);
	this->ui.setupUi(this);

	this->ui.procListWidget->setMouseTracking(true);

	QValidator* validator = new QIntValidator(0, 99999, this);
	this->ui.procIdText->setValidator(validator);

	QObject::connect(this->ui.addButton, SIGNAL(released()), this, SLOT(addNewProcess()));
	QObject::connect(this->ui.remButton, SIGNAL(released()), this, SLOT(removeProcess()));
	QObject::connect(this->ui.procListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(currentProcessChanged()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::currentProcessChanged()
{
	QListWidgetItem* item = this->ui.procListWidget->currentItem();
	if (item == NULL)
		return;
	this->ui.procIdText->setText(item->text());

	Process* process = this->model.getProcess(item->text().toInt());
	if (process != NULL) {
		if(strlen(process->name) == 0x00)
			this->setWindowTitle(QApplication::translate("window", "Process Watch Dog", 0, QApplication::UnicodeUTF8));
		else
			this->setWindowTitle(QString("Application: [%1]").arg(process->name));
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

void MainWindow::removeProcess()
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
		int row = this->ui.procListWidget->row(item);
		delete this->ui.procListWidget->takeItem(row);
	}
	this->ui.toolButton->setIcon(QIcon());
	this->ui.toolButton->setIconSize(QSize(80, 80));
	this->ui.toolButton->setToolTip("");
	this->ui.procIdText->clear();
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
			this->model.attachToProcess(process);
		}
		this->ui.procIdText->clear();
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
				dynamic_cast<ProcessChangedEvent*> (_event);
		this->processStateChanged(procEvent);
	}
	else if (type == ShutDownEvent::SHUTDOWN_EVENT)
	{
		ShutDownEvent* procEvent = dynamic_cast<ShutDownEvent*> (_event);
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
	if (isProcessRegistered(QString::number(process->id, 10))) {
		this->updateProcessStatusIcon(process);
	}
}

void MainWindow::shutdown(ShutDownEvent* _event)
{
	this->setWindowTitle("Initializing shutdown process...");
}
