## Julia Studio
[Julia Studio is a cross-platform IDE for the Julia language](http://forio.com/julia)
based on Qt Creator and the Qt framework.

![Screenshot of JuliaStudio running on Windows 7](http://i.imgur.com/78ExCHX.png)

[Click here to download Julia Studio](http://forio.com/julia/downloads) for Mac, Windows, or Linux

## Supported Platforms
The binary packages support the following platforms:

Windows 7, Windows XP SP2, Windows Vista
(K)Ubuntu Linux 8.04 (32-bit) or later
Mac OS 10.5 or later

Building the sources requires Qt 4.8.0 or later.



## Compiling Julia Studio
Prerequisites:
   * Qt 4.8.0
   * On Windows:
     -  ActiveState Active Perl
     -  MinGW 4.4 or later, Visual Studio 2008 or later
     -  jom
     The Qt SDK provides you with most of the required software.
   * On Mac: XCode 2.5 or later, compiling on 10.4 requires setting the
     environment variable QTC_TIGER_COMPAT before running qmake

You can build Julia Studio with

    cd $SOURCE_DIRECTORY
    qmake -r
    make (or mingw32-make or nmake or jom, depending on your platform)

Installation ("make install") is not needed. It is however possible, using

    make install INSTALL_ROOT=$INSTALL_DIRECTORY


### Compiling Qt and Julia Studio on Windows

This section provides step by step instructions for compiling the latest
versions of Qt and Julia Studio on Windows. Alternatively, to avoid having to
compile Qt yourself, you can use one of the versions of Qt shipped with the Qt
SDK (release builds of Qt using MinGW and Visual C++ 2008).

   1.  Decide which compiler to use: MinGW or Microsoft Visual Studio. If you
       plan to contribute to Julia Studio, you should compile your changes with
       both compilers.

   2.  Install msysGit from http://code.google.com/p/msysgit/. If you plan to
       use the MinGW compiler suite, do not choose to put git in the
       default path of Windows command prompts. For more information, see
       step 9.

   3.  Create a working directory under which to check out Qt and Julia Studio,
       for example, c:\work. If you plan to use MinGW and Microsoft Visual
       Studio simultaneously or mix different Qt versions, we recommend
       creating a directory structure which reflects that. For example:
       C:\work\qt4.7.1-vs10, C:\work\qt4.7.2-mingw.

   4.  Download and install Perl from http://www.activestate.com/activeperl
       and check that perl.exe is added to the path.

   5.  In the working directory, check out the respective branch of Qt
       (see http://qt.gitorious.org/qt).

   6.  Check out Julia Studio (master branch, see
       http://github.com/forio/julia-studio).
       You should now have the directories qt and julia-studio under your working
       directory.

   7.  Install a compiler:
    - For MinGW (4.4 onwards), see http://www.mingw.org/. Note that gcc 4.5
    produces insufficient debugging information, and therefore, we
    recommend that you use gcc 4.4 for debugging.
    - For Microsoft Visual C++, install the Windows SDK and the "Debugging
    Tools for Windows" from the SDK image. We strongly recommend using the
    64-bit version and 64-bit compilers on 64-bit systems.
    - When using Visual C++ 2010, you must apply a hotfix that is available
    from http://support.microsoft.com/kb/2280741
    (See https://bugreports.qt-project.org/browse/QTBUG-11445).
    - For the Visual C++ compilers, it is recommended to use the tool 'jom'.
    It is a replacement for nmake that utilizes all CPU cores and thus
    speeds up compilation significantly. Download it from
    http://releases.qt-project.org/jom/ and add the executable to the path.

   8.  For convenience, we recommend creating shell prompts with the correct
       environment. This can be done by creating a .bat-file
       (such as, <working_directory>\qtvars.bat) that contains the environment
       variable settings.
       A .bat-file for MinGW looks like:

         set QTDIR=<working_directory>\qt
         set PATH=%QTDIR%\bin;<path_to_mingw>\bin;<working_directory>\creator\bin;%PATH%
         set QMAKESPEC=win32-g++
         
  For the Visual C++ compilers, call the .bat file that sets up the
  environment for the compiler (provided by the Windows SDK or the
  compiler):

         CALL "C:\Program Files (x86)\MSVC10\VC\vcvarsall.bat" amd64
         set QTDIR=<working_directory>\qt
         set PATH=%QTDIR%\bin;<working_directory>\julia-studio\bin;%PATH%
         set QMAKESPEC=win32-msvc2010

  You can create desktop links to the bat files using the working
  directory and specifying

         %SystemRoot%\system32\cmd.exe /E:ON /V:ON  /k <working_directory>\qtvars.bat

   9.  When using MinGW, open the shell prompt and enter:

       `sh.exe`

       That should result in a 'sh is not recognized as internal or external
       command...' error. If a sh.exe is found, the compile process will fail.
       You have to remove it from the path.

   10.  You are now ready to configure and build Qt and Julia Studio.
       To use MinGW, open the the shell prompt and enter:

         cd qt
         configure -debug && mingw32-make -s
         cd ..\julia-studio
         qmake && mingw32-make -s

       To use the Visual C++ compilers, enter:

         cd qt
         configure -debug && jom
         cd ..\julia-studio
         qmake && jom

   11. To launch JuliaStudio, enter:
       `JuliaStudio`

   12. When using  Visual C++ with the "Debugging Tools for Windows" installed,
       the extension library qtcreatorcdbext.dll to be loaded into the
       Windows console debugger (cdb.exe) should have been built under
       lib\qtcreatorcdbext32 or lib\qtcreatorcdbext64.
       When using a 32 bit-build of Julia Studio with the 64 bit version of the
       "Debugging Tools for Windows" the library should also be built with
       a 64 bit compiler (rebuild src\libs\qtcreatorcdbext using a 64 bit
       compiler).



## Third-party Components
Julia Studio includes the following third-party components,
we thank the authors who made this possible:

* Open Source front-end for C++ (license MIT), enhanced for use in Qt Creator

  Roberto Raggi <roberto.raggi@gmail.com>

  JuliaStudio/src/shared/cplusplus

  Copyright 2005 Roberto Raggi <roberto@kdevelop.org>

  Permission to use, copy, modify, distribute, and sell this software and its
  documentation for any purpose is hereby granted without fee, provided that
  the above copyright notice appear in all copies and that both that
  copyright notice and this permission notice appear in supporting
  documentation.

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  KDEVELOP TEAM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


* Botan, a C++ crypto library. Version 1.8.8

  Copyright (C) 1999-2004 The Botan Project. All rights reserved.
  Copyright (C) 1999-2009 Jack Lloyd
  2001 Peter J Jones
  2004-2007 Justin Karneges
  2005 Matthew Gregan
  2005-2006 Matt Johnston
  2006 Luca Piccarreta
  2007 Yves Jerschow
  2007-2008 FlexSecure GmbH
  2007-2008 Technische Universitat Darmstadt
  2007-2008 Falko Strenzke
  2007-2008 Martin Doering
  2007 Manuel Hartl
  2007 Christoph Ludwig
  2007 Patrick Sona
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions, and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions, and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED. IN NO
  EVENT SHALL THE AUTHOR(S) OR CONTRIBUTOR(S) BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  The source code of Botan C++ crypto library can be found in
  JuliaStudio/src/libs/3rdparty


* ClassView and ImageViewer plugins

  Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).

  All rights reserved.
  Copyright (c) 2010 Denis Mingulov.

  Contact: http://www.qt-project.org/

  This file is part of Julia Studio.

  You may use this file under the terms of the BSD license as follows:

  "Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Digia Plc and its Subsidiary(-ies) nor
      the names of its contributors may be used to endorse or promote
      products derived from this software without specific prior written
      permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."



