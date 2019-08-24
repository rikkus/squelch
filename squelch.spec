Summary: Ogg Vorbis audio player
Name: squelch
Version: 1.0
Release: 1
Source: http://www.geoid.clara.net/rik/arch/squelch-1.0.tar.gz
Copyright: as XFree86, without advertising clause
Group: Applications/Multimedia
Prefix: /usr
Autoreq: 0
Requires: libvorbisfile.so.0,libao.so.0,libqt-mt.so.2


%description
Ogg Vorbis audio player

%prep
%setup

%build
./configure --prefix=/usr
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
make install

%files
%doc AUTHORS README COPYING ChangeLog

/usr/bin/squelch
/usr/man/man1/squelch.1
