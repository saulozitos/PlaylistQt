#include <QtTest>
#include <QCoreApplication>
#include <QJsonObject>

#include "../../PlaylistQt/settings.h"
#include "../../PlaylistQt/searchengine.h"
#include "../../PlaylistQt/spotify.h"
#include "../../PlaylistQt/auth.h"
#include "../../PlaylistQt/musictrack.h"

class TestSpotifySearch : public QObject
{
    Q_OBJECT
    QJsonObject jsonObject;

public:
    TestSpotifySearch() = default;
    ~TestSpotifySearch() = default;

private slots:
    void initTestCase();
    void getDataTrackFromJson();
    void getPlaylistData();

};

void TestSpotifySearch::initTestCase()
{
    Settings::getInstance().setClientId(QStringLiteral("b782dcea513b41f385613b99a7ad9945"));
    Settings::getInstance().setClientSecret(QStringLiteral("991c9203c980417e9d9335c2ebabb7bd"));
    Settings::getInstance().setRedirect(QStringLiteral("http://localhost:8888"));
    Settings::getInstance().setAccessToken(QStringLiteral("BQCaWQc_FqZH-MCBF08J2i2gs5uKbmi9AKIrOL3CYu7Isub8"
                                                            "C7dU6Y3Bxngysa3yhrcuKZmU5K7APBbreomIy9JHHW-nCkx-abSvr-XXOQnz3UioJldJbU5Iy8WrXa2_natKK2hsYXkCN0L"
                                                            "G5H6GLCLMWaRq_RRtCwQqGLHzAQ3nrO49SEwyCiuSAdfqVsQ4tZbKliqFag7CS5cg7ZlNYNFtMmBn8DKSjzyfuZiv1Z4CVSo"
                                                            "rIuJyXdYiFeVlx9md2pB6wfFG-2rpJpc"));
    Settings::getInstance().setCacheLocation(QStandardPaths::standardLocations(QStandardPaths::CacheLocation).constFirst());
    Auth auth;
    Spotify spotify;
    jsonObject = spotify.search(QStringLiteral("Raul Seixas"));
}

void TestSpotifySearch::getDataTrackFromJson()
{
    SearchEngine se;
    QVERIFY(se.getDataTrackFromSpotifyJson({}).empty());
    QVERIFY(!se.getDataTrackFromSpotifyJson(jsonObject).empty());
}

void TestSpotifySearch::getPlaylistData()
{
    const auto path = QStringLiteral("/home/saulo/.cache/PlaylistQt/playlist/b782dcea513b41f385613b99a7ad9945");
    const auto file = QStringLiteral("pedro.json");
    SearchEngine se;
    QVERIFY(se.loadPlaylistFromJsonFile("", "").first.isEmpty());
    QVERIFY(se.loadPlaylistFromJsonFile("", "").second.empty());
    QVERIFY(!se.loadPlaylistFromJsonFile(path, file).first.isEmpty());
    QVERIFY(!se.loadPlaylistFromJsonFile(path, file).second.empty());
}

QTEST_MAIN(TestSpotifySearch)

#include "tst_testspotifysearch.moc"
