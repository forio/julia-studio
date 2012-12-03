#include "fvversioncomparatortest.h"
#include "fvversioncomparator.h"

void FvVersionComparatorTest::testNumbers()
{
	QVERIFY(FvVersionComparator::CompareVersions("1.0", "1.1") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0", "1.0") == FvVersionComparator::kSame);
	QVERIFY(FvVersionComparator::CompareVersions("2.0", "1.1") == FvVersionComparator::kDescending);
	QVERIFY(FvVersionComparator::CompareVersions("0.1", "0.0.1") == FvVersionComparator::kDescending);
	QVERIFY(FvVersionComparator::CompareVersions("0.1", "0.1.2") == FvVersionComparator::kAscending);

	QVERIFY(FvVersionComparator::CompareVersions("1.0.0", "1.0.0") == FvVersionComparator::kSame);
	QVERIFY(FvVersionComparator::CompareVersions("1.0.0", "1.0.1") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0.0", "1.1.0") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0.0", "2.0.0") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0.0", "0.0.1") == FvVersionComparator::kDescending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0.0", "0.1.0") == FvVersionComparator::kDescending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0.0", "0.9.9") == FvVersionComparator::kDescending);
	QVERIFY(FvVersionComparator::CompareVersions("0.0.1", "0.0.2") == FvVersionComparator::kAscending);
}

void FvVersionComparatorTest::testPrereleases()
{
	QVERIFY(FvVersionComparator::CompareVersions("1.0a1", "1.0b1") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0b1", "1.0") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("0.9", "1.0a1") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0b", "1.0b2") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0b10", "1.0b11") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0b9", "1.0b10") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0rc", "1.0") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0b", "1.0") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0pre1", "1.0") == FvVersionComparator::kAscending);

	QVERIFY(FvVersionComparator::CompareVersions("1.0", "1.0pre1") == FvVersionComparator::kDescending);
}

void FvVersionComparatorTest::testVersionsWithBuildNumbers()
{
	QVERIFY(FvVersionComparator::CompareVersions("1.0 (1234)", "1.0 (1235)") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0b1 (1234)", "1.0 (1234)") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0b5 (1234)", "1.0b5 (1235)") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0b5 (1234)", "1.0.1b5 (1234)") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("1.0.1b5 (1234)", "1.0.1b6 (1234)") == FvVersionComparator::kAscending);
	QVERIFY(FvVersionComparator::CompareVersions("3.3 (5847)", "3.3.1b1 (5902)") == FvVersionComparator::kAscending);
}

QTEST_MAIN(FvVersionComparatorTest)