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

#include "MainWindow.h"
#include <QString>
#include <QIntValidator>

MainWindow::MainWindow() :
	QMainWindow(NULL, Qt::Window | Qt::WindowMinimizeButtonHint)
{
	this->ui.setupUi(this);

	QValidator* validator = new QIntValidator(0, 99999, this);
	this->ui.procIdText->setValidator(validator);
	QObject::connect(this->ui.newButton, SIGNAL(released())
			, this, SLOT(addNewProcess()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::addNewProcess()
{
	QString text = this->ui.procIdText->text();
	if(text.length() > 0x00)
	{
		this->ui.procListWidget->addItem(text);
	}
}
