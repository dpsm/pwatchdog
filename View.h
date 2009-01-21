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

#ifndef VIEW_H_
#define VIEW_H_

#include <qglobal.h>
#include <qstring.h>
#include <qvalidator.h>

#include "AbstractProcessView.h"
#include "PWatchdogEvents.h"
#include "ui_pwatchdog.h"
#include "Model.h"

class MainWindow: public AbstractProcessView
{
Q_OBJECT

public:
	MainWindow()	;
	virtual~ MainWindow();
	inline Ui::window getUi(){return ui;}
	virtual bool event(QEvent* _event);

private slots:
	void currentProcessChanged();
	void addNewProcess();
	void removeProcess();

private:
	void processStateChanged(ProcessChangedEvent* _event);
	void updateProcessStatusIcon(Process* _proc);
	bool isProcessRegistered(QString _text);
	void shutdown(ShutDownEvent* _event);
	Ui::window ui;
	Model model;
};

#endif /* VIEW_H_ */
