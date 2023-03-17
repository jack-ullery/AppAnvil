#
# spec file for appanvil package
#

Name:           appanvil
Version:        0.1.0
Release:        0
License:        GPL-3.0
Group:          Documentation
Summary:        AppArmor GUI
Url:            https://github.com/jack-ullery/AppAnvil
Source:         AppAnvil-%{version}

BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  gtkmm3-devel
BuildRequires:  jsoncpp-devel
BuildRequires:  libapparmor-devel
BuildRequires:  gtest gmock

Requires:  libgtkmm-3_0-1
Requires:  jsoncpp-devel
Requires:  libapparmor-devel
Requires:  polkit

BuildRoot: %{_tmppath}/%{name}-%{version}-build

%description
The AppAnvil project aims to create an intuitive graphical interface for monitoring and configuring AppArmor. In particular, we want it to be easy to monitor and deploy profiles, change a profile's permissions, and to parse system logs>

%prep
#%setup -q -n AppAnvil-%{version}

%build
cmake -DCMAKE_INSTALL_PREFIX=/usr -S %{sources} -B .
make %{?jobs:-j%jobs}

%install
%make_install

%post
%postun

%files
%defattr(-,root,root,-)

# Configuration for polkit (should be improved)
%dir %{_datadir}/polkit-1
%dir %{_datadir}/polkit-1/actions
%{_datadir}/polkit-1/actions/com.github.jack-ullery.AppAnvil.pkexec.policy

# Add the main binaries
%{_bindir}/appanvil
%{_bindir}/aa-caller

%changelog
