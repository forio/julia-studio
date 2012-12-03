#ifndef FVVERSIONCOMPARATORTEST_H
#define FVVERSIONCOMPARATORTEST_H

#include <QtTest/QtTest>


class FvVersionComparatorTest : public QObject
{
	Q_OBJECT

private slots:
	void testNumbers();
	void testPrereleases();
	void testVersionsWithBuildNumbers();

};

#endif // FVVERSIONCOMPARATORTEST_H
