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

MainWindow::MainWindow(Model* _model)
{
  this->window = new QMainWindow(NULL, Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint);
  this->ui.setupUi(this->window);

  this->ui.procListWidget->setMouseTracking(true);
  QValidator* validator = new QIntValidator(0, 99999, this->window);
  this->ui.procIdText->setValidator(validator);

  QObject::connect(this->ui.addButton, SIGNAL(released())
      , this, SLOT(AddButtonPressed()));
  QObject::connect(this->ui.remButton, SIGNAL(released())
      , this, SLOT(RemoveButtonPressed()));
  QObject::connect(this->ui.procListWidget, SIGNAL(itemSelectionChanged())
      , this, SLOT(DisplayProcess()));

  this->model = _model;
  this->model->attachView(this);
}

MainWindow::~MainWindow()
{
  free(this->window);
  this->window = NULL;
  this->model  = NULL;
}

void MainWindow::Display()
{
  this->window->show();
}

void MainWindow::UpdateProcessStatusIcon(Process* _proc)
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

void MainWindow::DisplayProcess()
{
  QListWidget* list = this->ui.procListWidget;

  QListWidgetItem* current = list->currentItem();
  if (current == NULL)
    return;

  this->ui.procIdText->setText(current->text());

  Process* process = this->model->getProcess(current->text().toInt());
  if (process != NULL)
  {
    if(strlen(process->name) == 0x00)
    {
      this->window->setWindowTitle(QApplication::translate("window",
              "Process Watch Dog", 0, QApplication::UnicodeUTF8));
    }
    else
    {
      this->window->setWindowTitle(QString("Application: [%1]").arg(process->name));
    }
    UpdateProcessStatusIcon(process);
  }
}

void MainWindow::RemoveButtonPressed()
{
  QListWidgetItem* item = this->ui.procListWidget->currentItem();
  if (item == NULL)
    return;

  int id = item->text().toInt();
  this->model->RemoveProcess(id);
}

void MainWindow::AddButtonPressed()
{
  QString text = this->ui.procIdText->text();
  if (text.length() > 0x00)
  {
    int id = text.toInt();
    this->model->AddProcess(id);
  }
}

bool MainWindow::event(QEvent* _event)
{
  QEvent::Type type = _event->type();
  if (type == ProcessAddedEvent::PROCESS_ADDED_EVENT)
  {
    ProcessAddedEvent* event =
        dynamic_cast<ProcessAddedEvent*> (_event);
    this->ProcessAdded(event->getSource());
  }
  else
  if (type == ProcessRemovedEvent::PROCESS_REMOVED_EVENT)
  {
    ProcessRemovedEvent* event =
        dynamic_cast<ProcessRemovedEvent*> (_event);
    this->ProcessRemoved(event->getSource());
  }
  else
  if (type == ProcessChangedEvent::PROCESS_CHANGED_EVENT)
  {
    ProcessChangedEvent* event =
        dynamic_cast<ProcessChangedEvent*> (_event);
    this->ProcessChanged(event->getSource());
  }
  else
  if (type == ShutDownEvent::SHUTDOWN_EVENT)
  {
    this->ShutdownInitiated();
  }
  else
  {
    return AbstractProcessView::event(_event);
  }
  return true;
}

void MainWindow::ProcessAdded(Process* _process)
{
  QListWidget* list = this->ui.procListWidget;
  list->addItem(QString::number(_process->id, 10));

  QListWidgetItem* item = this->GetProcessItem(_process);
  if(item != NULL)
  {
    int row = list->row(item);
    list->setCurrentRow(row);
  }
}

void MainWindow::ProcessRemoved(Process* _process)
{
  QListWidgetItem* item = this->GetProcessItem(_process);
  QListWidget*     list = this->ui.procListWidget;
  if(item != NULL)
      delete list->takeItem(list->row(item));

  if (list->count() == 0x00)
    this->ResetUI();
  else
    list->setCurrentRow(0x00);
}

void MainWindow::ProcessChanged(Process* _process)
{
  QListWidgetItem* item = this->GetProcessItem(_process);
  QListWidget*     list = this->ui.procListWidget;
  if(item != NULL);
  {
    list->setCurrentItem(item);
    this->DisplayProcess();
  }
}

void MainWindow::ShutdownInitiated()
{
  this->window->setWindowTitle("Initializing shutdown process...");
}

void MainWindow::ResetUI()
{
  this->window->setWindowTitle(QApplication::translate("window",
      "Process Watch Dog", 0, QApplication::UnicodeUTF8));
  this->ui.toolButton->setIcon(QIcon());
  this->ui.toolButton->setToolTip("");
  this->ui.procIdText->clear();
}

QListWidgetItem* MainWindow::GetProcessItem(Process* _process)
{
  QListWidgetItem* target = NULL;
  QListWidget*     list   = this->ui.procListWidget;
  QString id = QString::number(_process->id);
  for (int index = 0x00; index < list->count(); ++index)
  {
    QListWidgetItem* item = list->item(index);
    if(item != NULL && item->text() == id)
    {
      target = item;
      break;
    }
  }
  return target;
}

