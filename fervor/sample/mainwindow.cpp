#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fvupdater.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// Connect the "check for updates manually" button with the autoupdater
	connect(ui->updateButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(CheckForUpdatesNotSilent()));
}

MainWindow::~MainWindow()
{
	delete ui;
}
