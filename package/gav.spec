Summary: GPL rendition of old Arcade Volleyball game
Name: gav
Version: 0.6.0
Release: 2
URL: gav.sourceforge.net
Source0: %{name}-%{version}.tar.gz
License: GPL
Group: X11/Games/Video
BuildRoot: %{_tmppath}/%{name}-root

%description
An SDL-based rendition of an old favorite CGA game featuring
two characters playing a volleyball-like game. This "revamped"
version is supposed to support theming, multiplayer games,
different input devices and networking (not yet).

%prep
%setup -q

%build
make depend
make

%install
install -d $RPM_BUILD_ROOT/usr/bin
install gav $RPM_BUILD_ROOT/usr/bin/
install -d $RPM_BUILD_ROOT/usr/share/games/gav/themes
install -d $RPM_BUILD_ROOT/usr/share/doc/gav-%{version}
install -d $RPM_BUILD_ROOT/usr/share/applications
install -d $RPM_BUILD_ROOT/usr/share/pixmaps
install package/gav.desktop /usr/share/applications
install package/gav.desktop /etc/X11/applnk/Games
install package/gav.png /usr/share/pixmaps
cp -r themes/{classic,inverted} $RPM_BUILD_ROOT/usr/share/games/gav/themes
install README $RPM_BUILD_ROOT/usr/share/doc/gav-%{version}/
#rm -rf $RPM_BUILD_ROOT

%clean
make clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/gav
/usr/share/games/gav/
/usr/share/doc/gav-%{version}/README
/usr/share/applications/gav.desktop
/etc/X11/applnk/Games/gav.desktop
/usr/share/pixmaps/gav.png
/usr/share/games/gav/themes
/usr/share/games/gav/themes/classic
/usr/share/games/gav/themes/classic/Font.png
/usr/share/games/gav/themes/classic/FontInv.png
/usr/share/games/gav/themes/classic/background.png
/usr/share/games/gav/themes/classic/ball.png
/usr/share/games/gav/themes/classic/plfl.png
/usr/share/games/gav/themes/classic/plfr.png
/usr/share/games/gav/themes/classic/plml.png
/usr/share/games/gav/themes/classic/plmr.png
/usr/share/games/gav/themes/inverted
/usr/share/games/gav/themes/inverted/Font.png
/usr/share/games/gav/themes/inverted/FontInv.png
/usr/share/games/gav/themes/inverted/background.png
/usr/share/games/gav/themes/inverted/ball.png
/usr/share/games/gav/themes/inverted/plfl.png
/usr/share/games/gav/themes/inverted/plfr.png
/usr/share/games/gav/themes/inverted/plml.png
/usr/share/games/gav/themes/inverted/plmr.png

%changelog
* Fri Nov 15 2002 Alessandro Tommasi <tommasi@di.unipi.it>
- Initial build.


