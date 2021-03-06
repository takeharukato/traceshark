#
#
#  Traceshark - a visualizer for visualizing ftrace and perf traces
#  Copyright (C) 2014, 2015, 2016, 2017, 2018  Viktor Rosendahl
#  <viktor.rosendahl@gmail.com>
#
# This file is dual licensed: you can use it either under the terms of
# the GPL, or the BSD license, at your option.
#
#  a) This program is free software; you can redistribute it and/or
#     modify it under the terms of the GNU General Public License as
#     published by the Free Software Foundation; either version 2 of the
#     License, or (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public
#     License along with this library; if not, write to the Free
#     Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
#     MA 02110-1301 USA
#
# Alternatively,
#
#  b) Redistribution and use in source and binary forms, with or
#     without modification, are permitted provided that the following
#     conditions are met:
#
#     1. Redistributions of source code must retain the above
#        copyright notice, this list of conditions and the following
#        disclaimer.
#     2. Redistributions in binary form must reproduce the above
#        copyright notice, this list of conditions and the following
#        disclaimer in the documentation and/or other materials
#        provided with the distribution.
#
#     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
#     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

###############################################################################
# Header files
#

HEADERS       = qcustomplot/qcustomplot.h

HEADERS      +=  ui/cursor.h
HEADERS      +=  ui/cursorinfo.h
HEADERS      +=  ui/errordialog.h
HEADERS      +=  ui/eventinfodialog.h
HEADERS      +=  ui/eventselectdialog.h
HEADERS      +=  ui/eventselectmodel.h
HEADERS      +=  ui/eventselectview.h
HEADERS      +=  ui/eventsmodel.h
HEADERS      +=  ui/eventswidget.h
HEADERS      +=  ui/graphenabledialog.h
HEADERS      +=  ui/infowidget.h
HEADERS      +=  ui/licensedialog.h
HEADERS      +=  ui/mainwindow.h
HEADERS      +=  ui/migrationarrow.h
HEADERS      +=  ui/migrationline.h
HEADERS      +=  ui/taskgraph.h
HEADERS      +=  ui/taskinfo.h
HEADERS      +=  ui/taskmodel.h
HEADERS      +=  ui/taskrangeallocator.h
HEADERS      +=  ui/taskselectdialog.h
HEADERS      +=  ui/taskview.h
HEADERS      +=  ui/tcheckbox.h
HEADERS      +=  ui/traceplot.h
HEADERS      +=  ui/tracesharkstyle.h
HEADERS      +=  ui/yaxisticker.h

HEADERS      +=  analyzer/abstracttask.h
HEADERS      +=  analyzer/cpufreq.h
HEADERS      +=  analyzer/cpu.h
HEADERS      +=  analyzer/cpuidle.h
HEADERS      +=  analyzer/cputask.h
HEADERS      +=  analyzer/filterstate.h
HEADERS      +=  analyzer/migration.h
HEADERS      +=  analyzer/task.h
HEADERS      +=  analyzer/tcolor.h
HEADERS      +=  analyzer/traceanalyzer.h

HEADERS      +=  parser/genericparams.h
HEADERS      +=  parser/paramhelpers.h
HEADERS      +=  parser/traceevent.h
HEADERS      +=  parser/tracefile.h
HEADERS      +=  parser/tracelinedata.h
HEADERS      +=  parser/traceline.h
HEADERS      +=  parser/traceparser.h

HEADERS      +=  parser/ftrace/ftraceparams.h
HEADERS      +=  parser/ftrace/ftracegrammar.h

HEADERS      +=  parser/perf/perfparams.h
HEADERS      +=  parser/perf/perfgrammar.h

HEADERS      +=  threads/indexwatcher.h
HEADERS      +=  threads/loadbuffer.h
HEADERS      +=  threads/loadthread.h
HEADERS      +=  threads/threadbuffer.h
HEADERS      +=  threads/tthread.h
HEADERS      +=  threads/workitem.h
HEADERS      +=  threads/workqueue.h
HEADERS      +=  threads/workthread.h

HEADERS      +=  mm/mempool.h
HEADERS      +=  mm/stringpool.h
HEADERS      +=  mm/stringtree.h

HEADERS      +=  misc/errors.h
HEADERS      +=  misc/resources.h
HEADERS      +=  misc/setting.h
HEADERS      +=  misc/traceshark.h
HEADERS      +=  misc/tstring.h

HEADERS      +=  vtl/avltree.h
HEADERS      +=  vtl/bsdexits.h
HEADERS      +=  vtl/compiler.h
HEADERS      +=  vtl/error.h
HEADERS      +=  vtl/heapsort.h
HEADERS      +=  vtl/tlist.h
HEADERS      +=  vtl/time.h

###############################################################################
# Source files
#

SOURCES       = qcustomplot/qcustomplot.cpp

SOURCES      +=  ui/cursor.cpp
SOURCES      +=  ui/cursorinfo.cpp
SOURCES      +=  ui/errordialog.cpp
SOURCES      +=  ui/eventinfodialog.cpp
SOURCES      +=  ui/eventselectdialog.cpp
SOURCES      +=  ui/eventselectmodel.cpp
SOURCES      +=  ui/eventselectview.cpp
SOURCES      +=  ui/eventsmodel.cpp
SOURCES      +=  ui/eventswidget.cpp
SOURCES      +=  ui/graphenabledialog.cpp
SOURCES      +=  ui/infowidget.cpp
SOURCES      +=  ui/licensedialog.cpp
SOURCES      +=  ui/mainwindow.cpp
SOURCES      +=  ui/migrationarrow.cpp
SOURCES      +=  ui/migrationline.cpp
SOURCES      +=  ui/taskgraph.cpp
SOURCES      +=  ui/taskinfo.cpp
SOURCES      +=  ui/taskmodel.cpp
SOURCES      +=  ui/taskrangeallocator.cpp
SOURCES      +=  ui/taskselectdialog.cpp
SOURCES      +=  ui/taskview.cpp
SOURCES      +=  ui/tcheckbox.cpp
SOURCES      +=  ui/traceplot.cpp
SOURCES      +=  ui/tracesharkstyle.cpp
SOURCES      +=  ui/yaxisticker.cpp


SOURCES      +=  analyzer/abstracttask.cpp
SOURCES      +=  analyzer/cpufreq.cpp
SOURCES      +=  analyzer/cpuidle.cpp
SOURCES      +=  analyzer/cputask.cpp
SOURCES      +=  analyzer/filterstate.cpp
SOURCES      +=  analyzer/task.cpp
SOURCES      +=  analyzer/tcolor.cpp
SOURCES      +=  analyzer/traceanalyzer.cpp

SOURCES      +=  parser/traceevent.cpp
SOURCES      +=  parser/tracefile.cpp
SOURCES      +=  parser/traceparser.cpp

SOURCES      +=  parser/ftrace/ftraceparams.cpp
SOURCES      +=  parser/ftrace/ftracegrammar.cpp

SOURCES      +=  parser/perf/perfparams.cpp
SOURCES      +=  parser/perf/perfgrammar.cpp

SOURCES      +=  threads/indexwatcher.cpp
SOURCES      +=  threads/loadbuffer.cpp
SOURCES      +=  threads/loadthread.cpp
SOURCES      +=  threads/tthread.cpp
SOURCES      +=  threads/workqueue.cpp

SOURCES      +=  mm/mempool.cpp
SOURCES      +=  mm/stringpool.cpp
SOURCES      +=  mm/stringtree.cpp

SOURCES      +=  misc/errors.cpp
SOURCES      +=  misc/main.cpp
SOURCES      +=  misc/setting.cpp

SOURCES      +=  vtl/error.cpp

###############################################################################
# Qt Modules
#

QT           += core
QT           += widgets
QT           += printsupport

###############################################################################
# Directories
#

#DESTDIR=bin #Target file directory
OBJECTS_DIR=obj
MOC_DIR=obj

###############################################################################
# Architecture Flags
#

# Architecture flags, uncomment those that suits your machine best
# NB: Only a few of these have been tested. Could be spelling mistakes in any
# of the commented out flags.

# Automatic detection. Does not necessarily work
# MARCH_FLAG = -march=native
# MTUNE_FLAG = -mtune=native

##### x86-32 Section

# i386
# MARCH_FLAG = -march=i386
# MTUNE_FLAG = -mtune=i386

# i486
# MARCH_FLAG = -march=i486
# MTUNE_FLAG = -mtune=i486

# Pentium
# MARCH_FLAG = -march=pentium
# MTUNE_FLAG = -mtune=pentium

# Lakemont
# MARCH_FLAG = -march=lakemont
# MTUNE_FLAG = -mtune=lakemont

# Pentium MMX
# MARCH_FLAG = -march=pentium-mmx
# MTUNE_FLAG = -mtune=pentium-mmx

# Pentium Pro
# MARCH_FLAG = -march=pentiumpro
# MTUNE_FLAG = -mtune=pentiumpro

# i686
# MARCH_FLAG = -march=i686
# MTUNE_FLAG = -mtune=i686

# Pentium 2
# MARCH_FLAG = -march=pentium2
# MTUNE_FLAG = -mtune=pentium2

# Pentium 3
# MARCH_FLAG = -march=pentium3
# MTUNE_FLAG = -mtune=pentium3

# Pentium M
# MARCH_FLAG = -march=pentium-m
# MTUNE_FLAG = -mtune=pentium-m

# Pentium 4
# MARCH_FLAG = -march=pentium4
# MTUNE_FLAG = -mtune=pentium4

# Prescott
# MARCH_FLAG = -march=prescott
# MTUNE_FLAG = -mtune=prescott

### x86-64 Section

### AMD

# Athlon 64
# MARCH_FLAG = -march=athlon64
# MTUNE_FLAG = -mtune=athlon64

# Athlon 64 SSE3
# MARCH_FLAG = -march=athlon64-sse3
# MTUNE_FLAG = -mtune=athlon64-sse3

# Barcelona
# MARCH_FLAG = -march=barcelona
# MTUNE_FLAG = -mtune=barcelona

# Bulldozer v1
# MARCH_FLAG = -march=bdver1
# MTUNE_FLAG = -mtune=bdver1

# Bulldozer v2
# MARCH_FLAG = -march=bdver2
# MTUNE_FLAG = -mtune=bdver2

# Bulldozer v3
# MARCH_FLAG = -march=bdver3
# MTUNE_FLAG = -mtune=bdver3

# Bulldozer v4
# MARCH_FLAG = -march=bdver4
# MTUNE_FLAG = -mtune=bdver4

# Zen v1
# MARCH_FLAG = -march=znver1
# MTUNE_FLAG = -mtune=znver1

# Bobcat v1
# MARCH_FLAG = -march=btver1
# MTUNE_FLAG = -mtune=btver1

# Bobcat v2
# MARCH_FLAG = -march=btver2
# MTUNE_FLAG = -mtune=btver2

### Intel

# Cocona
# MARCH_FLAG = -march=nocona
# MTUNE_FLAG = -mtune=nocona

# Core2
# MARCH_FLAG = -march=core2
# MTUNE_FLAG = -mtune=core2

# Nehalem
# MARCH_FLAG = -march=nehalem
# MTUNE_FLAG = -mtune=nehalem

# Westmere
# MARCH_FLAG = -march=westmere
# MTUNE_FLAG = -mtune=westmere

# Sandybridge
# MARCH_FLAG = -march=sandybridge
# MTUNE_FLAG = -mtune=sandybridge

# Ivybridge
# MARCH_FLAG = -march=ivybridge
# MTUNE_FLAG = -mtune=ivybridge

# Haswell
# MARCH_FLAG = -march=haswell
# MTUNE_FLAG = -mtune=haswell

# Broadwell
# MARCH_FLAG = -march=broadwell
# MTUNE_FLAG = -mtune=broadwell

# Skylake
# MARCH_FLAG = -march=skylake
# MTUNE_FLAG = -mtune=skylake

# Bonnell
# MARCH_FLAG = -march=bonnell
# MTUNE_FLAG = -mtune=bonnell

# Silvermont
# MARCH_FLAG = -march=silvermont
# MTUNE_FLAG = -mtune=silvermont

# KNL
# MARCH_FLAG = -march=knl
# MTUNE_FLAG = -mtune=nkl

# KNM
# MARCH_FLAG = -march=knm
# MTUNE_FLAG = -mtune=knm

# Skylake-AVX512
# MARCH_FLAG = -march=skylake-avx512
# MTUNE_FLAG = -mtune=skylake-avx512

# Cannonlake
# MARCH_FLAG = -march=cannonlake
# MTUNE_FLAG = -mtune=cannonlake

# Icelake
# MARCH_FLAG = -march=icelake
# MTUNE_FLAG = -mtune=icelake

### Raspberry PI section

# RPI 3 - this does not seem to help much, if at all
# MARCH_FLAG = -mcpu=cortex-a53
# MTUNE_FLAG = -mtune=cortex-a53

###############################################################################
# Generic compiler flags
#

# Uncomment for debug build
# DEBUG_FLAG = -g

# These optimization options do not seem to help, so leave them commented out
# EXTRA_OPTS  = -fpredictive-commoning -fvect-cost-model -fsplit-paths -ftree-vectorize -funswitch-loops -floop-interchange

# EXTRA_OPTS += -funsafe-math-optimizations

HARDENING_CXXFLAGS += -fPIE -pie
HARDENING_CXXFLAGS += -D_FORTIFY_SOURCE=2
HARDENING_CXXFLAGS += -Wformat -Wformat-security -Werror=format-security
HARDENING_CXXFLAGS += -fstack-protector-strong

HARDENING_LFLAGS += -Wl,-z,relro,-z,now

OUR_FLAGS = $${MARCH_FLAG} $${MTUNE_FLAG} $${DEBUG_FLAG} $${EXTRA_OPTS}

# Not really needed, unless browsing data controlled by a non-trusted source
# or for testing purposes.
# OUR_FLAGS += $${HARDENING_CXXFLAGS}

QMAKE_CXXFLAGS_RELEASE += -pedantic -Wall -std=c++11 $${OUR_FLAGS}
QMAKE_CFLAGS_RELEASE += -pedantic -Wall -std=c11 $${OUR_FLAGS}
QMAKE_LFLAGS_RELEASE = -fwhole-program -O2 -std=c++11 $${OUR_FLAGS}

# Not really needed, unless browsing data controlled by a non-trusted source
# or for testing purposes.
# QMAKE_LFLAGS_RELEASE += $${HARDENING_LFLAGS}

# Uncomment this to compile with clang, or change to the C++ compiler of your
# choice
# QMAKE_CXX=clang-6.0
# I have not figured out how to link with clang but here you can specify a
# linker of your choice, g++ is default.
# QMAKE_LINK=g++

# Uncomment for debug build
# CONFIG += DEBUG

# Uncomment the line below to enable OpenGl through the new method. Seems flaky.
# DEFINES += QCUSTOMPLOT_USE_OPENGL

###############################################################################
# Resources
#

RESOURCES     = traceshark.qrc
