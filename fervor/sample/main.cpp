#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fvupdater.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Prerequisite for the Fervor updater
	QApplication::setOrganizationName("pypt");
	QApplication::setOrganizationDomain("pypt.lt");

	// Set feed URL before doing anything else
	FvUpdater::sharedUpdater()->SetFeedURL("http://pypt.github.com/fervor/Appcast.xml");

	// Check for updates automatically
	FvUpdater::sharedUpdater()->CheckForUpdatesSilent();

	// Show main window
	MainWindow w;
	w.show();
	
	return a.exec();
}
