size = $(notdir $(PWD))
icondir = $(datadir)/icons/hicolor/$(size)/apps/
icon_DATA = $(PACKAGE).png

EXTRA_DIST = $(icon_DATA)

devices_icondir = $(datadir)/icons/hicolor/$(size)/devices/
devices_icon_DATA = preference.png send-key.png home.png leave-fullscreen.png disconnect.png file.png
EXTRA_DIST += $(devices_icon_DATA)

-include $(top_srcdir)/git.mk
