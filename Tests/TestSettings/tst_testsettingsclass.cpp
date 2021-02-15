#include <QtTest>
#include "../../PlaylistQt/settings.h"

class TestSettingsClass : public QObject
{
    Q_OBJECT
    Settings &settings = Settings::getInstance();
    const QString id = QStringLiteral("ABC123");
    const QString secret = QStringLiteral("ZFV765");
    const QString redirect = QStringLiteral("http://localhost:8888");
    const QString code = QStringLiteral("456");
    const QString accessToken = QStringLiteral("arhoscdnvowiehgofvo");
    const QString cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).constFirst();

public:
    TestSettingsClass() = default;
    ~TestSettingsClass() = default;

private slots:
    void initTestCase();
    void getClientId();
    void getClientSecret();
    void getRedirect();
    void getCode();
    void getAccessToken();
    void getCacheLocation();
};

void TestSettingsClass::initTestCase()
{
    settings.setClientId(id);
    settings.setClientSecret(secret);
    settings.setRedirect(redirect);
    settings.setCode(code);
    settings.setAccessToken(accessToken);
    settings.setCacheLocation(cacheLocation);
}

void TestSettingsClass::getClientId()
{
    QVERIFY(settings.getClientId() == id);
}

void TestSettingsClass::getClientSecret()
{
    QVERIFY(settings.getClientSecret() == secret);
}

void TestSettingsClass::getRedirect()
{
    QVERIFY(settings.getRedirect() == redirect);
}

void TestSettingsClass::getCode()
{
    QVERIFY(settings.getCode() == code);
}

void TestSettingsClass::getAccessToken()
{
    QVERIFY(settings.getAccessToken() == accessToken);
}

void TestSettingsClass::getCacheLocation()
{
    QVERIFY(settings.getCacheLocation() == cacheLocation);
}


QTEST_APPLESS_MAIN(TestSettingsClass)

#include "tst_testsettingsclass.moc"
