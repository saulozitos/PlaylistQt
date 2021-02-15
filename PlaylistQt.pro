TEMPLATE = subdirs

PlaylistQt.subir = PlaylistQt
Tests.subir = Tests

Tests.depends = PlaylistQt

SUBDIRS += \
    PlaylistQt \
    Tests \
