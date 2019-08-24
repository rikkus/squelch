bin_PROGRAMS = squelch

INCLUDES = -I$(QTDIR)/include @VORBIS_CFLAGS@ @AO_CFLAGS@

squelch_LDADD = @VORBISFILE_LIBS@ @VORBIS_LIBS@ @AO_LIBS@ @QT_LIBS@

squelch_SOURCES = \
	AlbumItem.cpp \
	ArtistItem.cpp \
	BufferItem.cpp \
	BufferItemPool.cpp \
	DirWatcher.cpp \
	IOController.cpp \
	InfoReader.cpp \
	Input.cpp \
	OptionsDialog.cpp \
	OptionsDialogBase.cpp \
	Output.cpp \
	PlayList.cpp \
	PlayListEditWidget.cpp \
	PreviewItem.cpp \
	PreviewWidget.cpp \
	RemoteControl.cpp \
	RikStyle.cpp \
	ScrollingLabel.cpp \
	Seeker.cpp \
	SquelchItem.cpp \
	Static.cpp \
	TimeDisplay.cpp \
	TopItem.cpp \
	Track.cpp \
	TrackAddNotifyDialog.cpp \
	TitleItem.cpp \
	TrackList.cpp \
	VorbisInput.cpp \
	VorbisHelpers.cpp \
	AddArtistDialog.cpp \
	AddExtraDialog.cpp \
	HelpDialog.cpp \
	VorbisTagEditor.cpp \
	VorbisTagEditDialog.cpp \
	VorbisTagEditWidget.cpp \
	vcedit.c \
	netwm.cpp \
	main.cpp

# Qt designer files

DESIGNERSOURCES = \
	AddArtistDialog.h \
	AddArtistDialog.cpp \
	AddExtraDialog.h \
	AddExtraDialog.cpp \
	HelpDialog.h \
	HelpDialog.cpp \
	OptionsDialogBase.h \
	OptionsDialogBase.cpp \
	VorbisTagEditWidget.h \
	VorbisTagEditWidget.cpp

VorbisTagEditDialog.cpp: \
	VorbisTagEditWidget.h \
	AddArtistDialog.h \
	AddExtraDialog.h \
	HelpDialog.h

AddArtistDialog.h:	$(srcdir)/AddArtistDialog.ui
	@UIC@ -tr '' $(srcdir)/AddArtistDialog.ui -o AddArtistDialog.h

AddExtraDialog.h:	$(srcdir)/AddExtraDialog.ui
	@UIC@ -tr '' $(srcdir)/AddExtraDialog.ui -o AddExtraDialog.h

HelpDialog.h:	$(srcdir)/HelpDialog.ui
	@UIC@ -tr '' $(srcdir)/HelpDialog.ui -o HelpDialog.h

OptionsDialogBase.h:	$(srcdir)/OptionsDialogBase.ui
	@UIC@ -tr '' $(srcdir)/OptionsDialogBase.ui -o OptionsDialogBase.h

VorbisTagEditWidget.h:	$(srcdir)/VorbisTagEditWidget.ui
	@UIC@ -tr '' $(srcdir)/VorbisTagEditWidget.ui -o VorbisTagEditWidget.h

AddArtistDialog.cpp: AddArtistDialog.h AddArtistDialog.moc
	@UIC@ -impl AddArtistDialog.h $(srcdir)/AddArtistDialog.ui \
	-o AddArtistDialog.cpp
	echo '#include "AddArtistDialog.moc"' >> AddArtistDialog.cpp

AddExtraDialog.cpp: AddExtraDialog.h AddExtraDialog.moc
	@UIC@ -impl AddExtraDialog.h $(srcdir)/AddExtraDialog.ui \
	-o AddExtraDialog.cpp
	echo '#include "AddExtraDialog.moc"' >> AddExtraDialog.cpp

HelpDialog.cpp: HelpDialog.h HelpDialog.moc
	@UIC@ -impl HelpDialog.h $(srcdir)/HelpDialog.ui \
	-o HelpDialog.cpp
	echo '#include "HelpDialog.moc"' >> HelpDialog.cpp

OptionsDialogBase.cpp : OptionsDialogBase.h OptionsDialogBase.moc
	@UIC@ -impl OptionsDialogBase.h $(srcdir)/OptionsDialogBase.ui \
	-o OptionsDialogBase.cpp
	echo '#include "OptionsDialogBase.moc"' >> OptionsDialogBase.cpp

VorbisTagEditWidget.cpp: VorbisTagEditWidget.h VorbisTagEditWidget.moc
	@UIC@ -impl VorbisTagEditWidget.h $(srcdir)/VorbisTagEditWidget.ui \
	-o VorbisTagEditWidget.cpp
	echo '#include "VorbisTagEditWidget.moc"' >> VorbisTagEditWidget.cpp

# Qt meta-object files

METASOURCES = \
	DirWatcher.moc \
	IOController.moc \
	OptionsDialog.moc \
	OptionsDialogBase.moc \
	PlayList.moc \
	PlayListEditWidget.moc \
	PreviewWidget.moc \
	RemoteControl.moc \
	ScrollingLabel.moc \
	Seeker.moc \
	TimeDisplay.moc \
	Track.moc \
	TrackAddNotifyDialog.moc \
	AddArtistDialog.moc \
	AddExtraDialog.moc \
	HelpDialog.moc \
	VorbisTagEditWidget.moc

DirWatcher.cpp				: DirWatcher.moc
IOController.cpp			: IOController.moc
OptionsDialog.cpp			: OptionsDialog.moc OptionsDialogBase.h
OptionsDialogBase.cpp			: OptionsDialogBase.moc
PlayList.cpp				: PlayList.moc
PlayListEditWidget.cpp			: PlayListEditWidget.moc
PreviewWidget.cpp			: PreviewWidget.moc
RemoteControl.cpp			: RemoteControl.moc
ScrollingLabel.cpp			: ScrollingLabel.moc
Seeker.cpp				: Seeker.moc
TimeDisplay.cpp				: TimeDisplay.moc
Track.cpp				: Track.moc
TrackAddNotifyDialog.cpp		: TrackAddNotifyDialog.moc
VorbisTagEditor.cpp			: VorbisTagEditor.moc
VorbisTagEditWidget.cpp			: VorbisTagEditWidget.moc
VorbisTagEditDialog.cpp			: VorbisTagEditDialog.moc
	
DirWatcher.moc: $(srcdir)/DirWatcher.h
	@MOC@ $(srcdir)/DirWatcher.h -o DirWatcher.moc

IOController.moc: $(srcdir)/IOController.h
	@MOC@ $(srcdir)/IOController.h -o IOController.moc

OptionsDialog.moc: $(srcdir)/OptionsDialog.h
	@MOC@ $(srcdir)/OptionsDialog.h -o OptionsDialog.moc

PlayList.moc: $(srcdir)/PlayList.h
	@MOC@ $(srcdir)/PlayList.h -o PlayList.moc

PlayListEditWidget.moc: $(srcdir)/PlayListEditWidget.h
	@MOC@ $(srcdir)/PlayListEditWidget.h -o PlayListEditWidget.moc

PreviewWidget.moc: $(srcdir)/PreviewWidget.h
	@MOC@ $(srcdir)/PreviewWidget.h -o PreviewWidget.moc

RemoteControl.moc: $(srcdir)/RemoteControl.h
	@MOC@ $(srcdir)/RemoteControl.h -o RemoteControl.moc

ScrollingLabel.moc: $(srcdir)/ScrollingLabel.h
	@MOC@ $(srcdir)/ScrollingLabel.h -o ScrollingLabel.moc

Seeker.moc: $(srcdir)/Seeker.h
	@MOC@ $(srcdir)/Seeker.h -o Seeker.moc

TimeDisplay.moc: $(srcdir)/TimeDisplay.h
	@MOC@ $(srcdir)/TimeDisplay.h -o TimeDisplay.moc

Track.moc: $(srcdir)/Track.h
	@MOC@ $(srcdir)/Track.h -o Track.moc

TrackAddNotifyDialog.moc: $(srcdir)/TrackAddNotifyDialog.h
	@MOC@ $(srcdir)/TrackAddNotifyDialog.h -o TrackAddNotifyDialog.moc

AddArtistDialog.moc: AddArtistDialog.h
	@MOC@ AddArtistDialog.h -o AddArtistDialog.moc

AddExtraDialog.moc: AddExtraDialog.h
	@MOC@ AddExtraDialog.h -o AddExtraDialog.moc

HelpDialog.moc: HelpDialog.h
	@MOC@ HelpDialog.h -o HelpDialog.moc

OptionsDialogBase.moc: OptionsDialogBase.h
	@MOC@ OptionsDialogBase.h -o OptionsDialogBase.moc

VorbisTagEditWidget.moc: VorbisTagEditWidget.h
	@MOC@ VorbisTagEditWidget.h -o VorbisTagEditWidget.moc

VorbisTagEditor.moc: $(srcdir)/VorbisTagEditor.h
	@MOC@ $(srcdir)/VorbisTagEditor.h -o VorbisTagEditor.moc

VorbisTagEditDialog.moc: VorbisTagEditDialog.h
	@MOC@ VorbisTagEditDialog.h -o VorbisTagEditDialog.moc

DISTCLEANFILES = $(METASOURCES) $(DESIGNERSOURCES)
