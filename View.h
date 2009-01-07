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

#include <qmainwindow.h>
#include <qglobal.h>
#include <qstring.h>
#include <qvalidator.h>

#include "Ui_pwatchdog.h"

#include "AbstractProcessView.h"
#include "Model.h"

#ifndef VIEW_H_
#define VIEW_H_

class MainWindow : public QMainWindow, public AbstractProcessView{
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

	inline Ui::window getUi() {return ui;}
	virtual void processStateChanged(int process_id, AbstractProcessView::ProcessState state);

private slots:
	void detachFromProcess();
	void attachToProcess();
	void addNewProcess();

private:
	Ui::window ui;
	Model model;
};

#endif /* VIEW_H_ */
