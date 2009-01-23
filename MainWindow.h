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

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <qmainwindow.h>
#include <qvalidator.h>
#include <qglobal.h>
#include <qstring.h>

#include "AbstractProcessView.h"
#include "PWatchdogEvents.h"
#include "ui_pwatchdog.h"
#include "Model.h"

class MainWindow: public AbstractProcessView
{
  Q_OBJECT

  public:

    //-----------------------------------------------
    //FUNCTIONS
    //-----------------------------------------------

    MainWindow(Model* _model);
    virtual~ MainWindow();

    inline Ui::window GetUi()
    {
      return ui;
    }
    virtual bool event(QEvent* _event);

    virtual void ProcessAdded(Process* _process);
    virtual void ProcessRemoved(Process* _process);
    virtual void ProcessChanged(Process* _process);
    virtual void ShutdownInitiated();
    virtual void Display();

  private slots:

    //-----------------------------------------------
    //SLOTS
    //-----------------------------------------------

    void DisplayProcess();
    void AddButtonPressed();
    void RemoveButtonPressed();

  private:

    //-----------------------------------------------
    //FUNCTIONS
    //-----------------------------------------------

    QListWidgetItem* GetProcessItem(Process* _process);
    void UpdateProcessStatusIcon(Process* _process);
    void ResetUI();
    //-----------------------------------------------
    //ATTRIBUTES
    //-----------------------------------------------

    QMainWindow* window;
    Ui::window ui;
    Model* model;
};

#endif /* MAINWINDOW_H_ */
