
DEST = $${LITERAL_HASH}"!/usr/bin/env xdg-open" "[Desktop Entry]" Name=test Name[ru]=test Comment[ru]=test Exec=$$absolute_path($${DESTDIR}/$$TARGET)  Icon=$$absolute_path($${DESTDIR}/$${TARGET}.png) Terminal=false Type=Application Categories=GNOME; StartupNotify=true
# если файл существует, то происходит замена
write_file($$DESTDIR/$${TARGET}.desktop, DEST)
