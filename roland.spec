# Specfile

#%define debug	full
%define debug	0

%define	name	roland
%define	version	0.5
%define release	1


Name:      %{name}
Version:   %{version}
Release:   %{release}
Vendor:    Fred Klaus <frednet@web.de>
License:   GPL
Summary:   Amstrad Emulator based on Caprice Source rewritten in C++
Group:     Emulators
Packager:  Fred Klaus <frednet@web.de>
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Source:    %{name}-%{version}.tar.bz2

%description
Amstrad Emulator based on Caprice Source rewritten in C++.


%prep
%setup

%ifarch athlon
export RPM_OPT_FLAGS="%{optflags} -ffast-math -mfpmath=sse -mmmx -msse -m3dnow"
%define optflags $RPM_OPT_FLAGS
%endif

%if %{debug}
	CFLAGS="$RPM_OPT_FLAGS -O0 -Wall -g3" \
	CXXFLAGS="$RPM_OPT_FLAGS -O0 -Wall -g3" \
	./configure --prefix=/usr --enable-debug=%{debug}
%else
	CFLAGS="$RPM_OPT_FLAGS" \
	CXXFLAGS="$RPM_OPT_FLAGS" \
	./configure --prefix=/usr  --enable-mmx --disable-debug
%endif

%build
make

%install
rm -rf %{buildroot}

%if %{debug}
make DESTDIR=%{buildroot} install
%else
make DESTDIR=%{buildroot} install-strip
%endif

%clean
rm -rf %{buildroot}


%files
%defattr(-, root, root)
%doc AUTHORS COPYING ChangeLog NEWS README TODO
%{_bindir}/roland
