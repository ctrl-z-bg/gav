Summary: GPL rendition of old Arcade Volleyball game
Name: gav
Version: 0.5.1
Release: 1
URL: gav.sourceforge.net
Source0: %{name}-%{version}.tar.gz
License: GPL
Group: X11/Games/Video
BuildRoot: %{_tmppath}/%{name}-root

%description
An SDL-based rendition of an old favorite CGA game featuring
two characters playing a volleyball-like game. This "revamped"
is supposed to support theming, multiplayer games, different
input devices and networking (not yet).

%prep
%setup -q

%build
make

%install
install -d $RPM_BUILD_ROOT/usr/bin
install gav $RPM_BUILD_ROOT/usr/bin/
install -d $RPM_BUILD_ROOT/usr/share/games/gav/themes
cp -r themes/* $RPM_BUILD_ROOT/usr/share/games/gav/themes
#rm -rf $RPM_BUILD_ROOT

%clean
make clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/gav
/usr/share/games/gav/
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

%changelog
* Fri Nov 15 2002 Alessandro Tommasi <tommasi@di.unipi.it>
- Initial build.


