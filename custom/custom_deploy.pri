QMAKE_POST_LINK += && $$QMAKE_COPY $$PWD/deploy/dropstrike-start.sh $$DESTDIR
QMAKE_POST_LINK += && $$QMAKE_COPY $$PWD/deploy/dropstrike.desktop $$DESTDIR
QMAKE_POST_LINK += && $$QMAKE_COPY $$PWD/res/icons/dropstrike.png $$DESTDIR