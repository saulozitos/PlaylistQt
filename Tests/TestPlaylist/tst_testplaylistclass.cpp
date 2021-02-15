#include <QtTest>
#include <memory>
#include "../../PlaylistQt/playlist.h"

class TestPlaylistClass : public QObject
{
    Q_OBJECT
    std::unique_ptr<Playlist>playlist;
    const QString artistName = QStringLiteral("ArtitaTeste");
    const QString musicaName = QStringLiteral("MusicaTeste");
    const QString urlPreview = QStringLiteral("UrlPreviewTeste");

public:
    TestPlaylistClass() = default;
    ~TestPlaylistClass() = default;


private slots:
    void initTestCase();
    void getPlaylist();
    void addtrack();
    void getPlaylistData();
    void removeTrack();
    void removeAll();

};

void TestPlaylistClass::initTestCase()
{
    playlist = std::make_unique<Playlist>("PlaylistDeTeste");
}

void TestPlaylistClass::getPlaylist()
{
    QVERIFY(playlist->getPlaylist() != nullptr);
}

void TestPlaylistClass::addtrack()
{
    MusicTrack mt;
    mt.artistName = artistName;
    mt.musicaName = musicaName;
    mt.urlPreview = urlPreview;
    playlist->addtrack(mt);
    QVERIFY(!playlist->getPlaylistData().empty());
}

void TestPlaylistClass::getPlaylistData()
{
    QVERIFY(playlist->getPlaylistData().at(0).artistName == artistName);
    QVERIFY(playlist->getPlaylistData().at(0).musicaName == musicaName);
    QVERIFY(playlist->getPlaylistData().at(0).urlPreview == urlPreview);
}

void TestPlaylistClass::removeTrack()
{
    playlist->removeTrack(0);
    QVERIFY(playlist->getPlaylistData().empty());
}

void TestPlaylistClass::removeAll()
{
    addtrack();
    addtrack();
    addtrack();
    addtrack();
    QVERIFY(playlist->getPlaylistData().size() == 4);
    playlist->removeAll();
    QVERIFY(playlist->getPlaylistData().empty());
}


QTEST_APPLESS_MAIN(TestPlaylistClass)

#include "tst_testplaylistclass.moc"
