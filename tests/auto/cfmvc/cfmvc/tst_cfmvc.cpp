#include <QtTest/QtTest>
#include <QtCore/qstandardpaths.h>
#include <QtCore/qdir.h>
#include <QtCore/qtemporarydir.h>
#include <QtCore/qfileinfo.h>

class tst_CFParse : public QObject
{
    Q_OBJECT
public:
    tst_CFParse()

private slots:
    void initTestCase();
};

void tst_CFParse::initTestCase()
{
}

QTEST_MAIN(tst_CFParse)

#include "tst_cfparse.moc"
