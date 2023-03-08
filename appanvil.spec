#
# spec file for appanvil package
# This is used by Open Build Service as directions for how to generate packages
#

Name:           appanvil
Version:        0.1.0
Release:        0
License:        GPL-3.0
Group:          Development/Languages/Cpp
Summary:        AppArmor GUI
Url:            https://github.com/jack-ullery/AppAnvil
Source:         AppAnvil
BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  git
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
The AppAnvil project aims to create an intuitive graphical interface for monitoring and configuring AppArmor. In particular, we want it to be easy to monitor and deploy profiles, change a profile's permissions, and to parse system logs>

%prep
%setup -q

%build
#cmake -DCMAKE_INSTALL_PREFIX=/usr .
cmake .
make

%install
cpack -G DEB .

%files
%defattr(-,root,root,-)
%doc README LICENSE *.txt
%{_bindir}/*

%changelog
