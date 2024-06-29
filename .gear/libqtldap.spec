%define _unpackaged_files_terminate_build 1

Name: libqtldap
Version: 0.0.1
Release: alt1

Summary: Client library for LDAP in Qt
License: GPLv2+
Group: Development
Url: https://github.com/august-alt/libqtldap

BuildRequires: rpm-macros-cmake cmake cmake-modules gcc-c++
BuildRequires: qt5-base-common qt5-base-devel qt5-declarative-devel qt5-tools-devel
BuildRequires: libgtest-devel xorg-xvfb xvfb-run
BuildRequires: doxygen

Requires: cmake

Source0: %name-%version.tar

%description
Client library for LDAP in Qt

%prep
%setup -q

%build
%cmake
%cmake_build

%install
%cmakeinstall_std

%files
%doc README.md
%doc INSTALL.md

%changelog
* Sat Jun 26 2024 Vladimir Rubanov <august@altlinux.org> 0.0.1-alt1
- 0.0.1-alt1
- Initial build
