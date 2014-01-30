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
