#
Name:       apitrace
Summary:    The apitrace Opengl API Tracing Tool
Version:    0.0.1
Release:    01
Group:      System/X Hardware Support
License:    Samsung
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  pkgconfig
BuildRequires:  pkgconfig(python)
BuildRequires:  libX11-devel
BuildRequires:  opengl-es-drv-devel

%description
The apitrace Opengl API Tracing Tool
The apitrace Opengl API Tracing Tool

%prep
%setup -q

%build
%ifarch %{arm}
GUIFLAGS="-DENABLE_GUI=FALSE"
#else
GUIFLAGS="-DENABLE_GUI=AUTO"
%endif

cmake -DCMAKE_BUILD_TYPE=Debug ${GUIFLAGS} -H. -Bbuild
make -C build

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/opt/apitrace/bin/wrappers

cp -a build/wrappers/glxtrace.so %{buildroot}/opt/apitrace/bin/wrappers/
cp -a build/wrappers/egltrace.so %{buildroot}/opt/apitrace/bin/wrappers/
cp -a build/apitrace             %{buildroot}/opt/apitrace/bin/
cp -a build/glretrace            %{buildroot}/opt/apitrace/bin/
cp -a build/eglretrace           %{buildroot}/opt/apitrace/bin/
%ifnarch %{arm}
cp -a build/qapitrace            %{buildroot}/opt/apitrace/bin/
%endif

%files
%defattr(-,root,root,-)
/opt/apitrace/bin/wrappers/glxtrace.so
/opt/apitrace/bin/wrappers/egltrace.so
/opt/apitrace/bin/apitrace
/opt/apitrace/bin/glretrace
/opt/apitrace/bin/eglretrace
%ifnarch %{arm}
/opt/apitrace/bin/qapitrace
%endif

