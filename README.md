<a name="logo"/>
<div align="center">
<a href="http://forio.com/products/julia-studio/" target="_blank">
<img src="http://github.com/forio/julia-studio/blob/master/src/plugins/juliaeditor/images/js-by-forio.png?raw=true" alt="Julia Logo" width="525" height="128"></img>
</a>
</div>

## Julia Studio
[Julia Studio](http://forio.com/products/julia-studio/) is a cross-platform IDE for the [Julia language](http://julialang.org/)
based on [Qt Creator](http://qt.gitorious.org/qt-creator) and the [Qt framework](http://qt.gitorious.org/qt).

## Supported Platforms
[Binary packages](http://forio.com/products/julia-studio/download) are available for the following platforms:

   * Mac OSX 10.6 and later
   * Windows XP SP2 and later
   * (K)Ubuntu Linux 12.04 and later

Julia Studio binaries for Windows and Mac install the Julia programming language.  Linux users will need to [install Julia](http://julialang.org/downloads/) separately.

![Screenshot of JuliaStudio running on Mac](http://forio.com/img/julia-studio/js-plot.png)

## Compiling Julia Studio

#### Prerequisites

   * [Qt 5.1.0 or later](http://qt-project.org/downloads)
   * [Julia 0.2 or later](http://julialang.org/downloads/)

#### OSX
```
cd $SOURCE_DIRECTORY
qmake -r
make
```
Note: Installation ("make install") is not needed. It is however possible, using
```
make install INSTALL_ROOT=$INSTALL_DIRECTORY
```

#### Windows
See instructions in [windows-build.md](./windows-build.md)

#### Linux
See instructions in [linux-build.md](./linux-build.md)


## Third-party Components
Julia Studio includes the following third-party components,
we thank the authors who made this possible:

##### Open Source front-end for C++ (license MIT), enhanced for use in Qt Creator

```
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
```

##### Botan, a C++ crypto library. Version 1.8.8

```
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
```

##### ClassView and ImageViewer plugins

```
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
```


