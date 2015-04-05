/*
 * Traceshark - a visualizer for visualizing ftrace traces
 * Copyright (C) 2014-2015  Viktor Rosendahl <viktor.rosendahl@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGlobal>
#include <QString>
#include <QTextStream>
#include "ftraceparams.h"
#include "ftraceparser.h"
#include "tracefile.h"
#include "grammarroot.h"
#include "namepidnode.h"
#include "cpunode.h"
#include "timenode.h"
#include "eventnode.h"
#include "argnode.h"
#include "traceshark.h"

using namespace TraceShark;

#define FAKE_DELTA ((double) 0.0000005)

bool FtraceParser::open(const QString &fileName)
{
	unsigned long long nr = 0;
	bool ok = false;

	if (traceFile != NULL)
		return ok;

	traceFile = new TraceFile(fileName.toLocal8Bit().data(), ok, 1024*1024);

	if (!ok) {
		delete traceFile;
		traceFile = NULL;
		return ok;
	}

	lines.reserve(80000000);

	while(!traceFile->atEnd()) {
		TraceLine line;
		quint32 n = traceFile->ReadLine(&line);
		lines.append(line);
		nr += n;
	}
	QTextStream(stdout) << nr << "\n";
	return true;
}

bool FtraceParser::isOpen()
{
	return (traceFile != NULL);
}

void FtraceParser::close()
{
	if (traceFile != NULL) {
		lines.resize(0);
		lines.reserve(1);
		events.resize(0);
		events.reserve(1);
		delete traceFile;
		traceFile = NULL;
	}
}

FtraceParser::FtraceParser()
	: cpuTaskMaps(NULL)
{
	NamePidNode *namePidNode;
	CpuNode *cpuNode;
	TimeNode *timeNode;
	EventNode *eventNode;
	ArgNode *argNode;

	traceFile = NULL;
	ptrPool = new MemPool(2048, sizeof(TString*));
	taskNamePool = new MemPool(16, sizeof(char));

	argNode = new ArgNode("argnode");
	argNode->nChildren = 1;
	argNode->children[0] = argNode;
	argNode->isLeaf = true;

	eventNode = new EventNode("eventnode");
	eventNode->nChildren = 1;
	eventNode->children[0] = argNode;
	eventNode->isLeaf = true;

	timeNode = new TimeNode("timenode");
	timeNode->nChildren = 1;
	timeNode->children[0] = eventNode;
	timeNode->isLeaf = false;

	cpuNode = new CpuNode("cpunode");
	cpuNode->nChildren = 1;
	cpuNode->children[0] = timeNode;
	cpuNode->isLeaf = false;

	namePidNode = new NamePidNode("namepidnode");
	namePidNode->nChildren = 1;
	namePidNode->children[0] = cpuNode;
	namePidNode->isLeaf = false;

	grammarRoot = new GrammarRoot("rootnode");
	grammarRoot->nChildren = 1;
	grammarRoot->children[0] = namePidNode;
	grammarRoot->isLeaf = false;
}

FtraceParser::~FtraceParser()
{
	DeleteGrammarTree(grammarRoot);
	if (traceFile != NULL)
		delete traceFile;
	delete ptrPool;
}

void FtraceParser::DeleteGrammarTree(GrammarNode* node) {
	unsigned int i;
	for (i = 0; i < node->nChildren; i++) {
		/* Delete subtree unless it's a node being it's own child */
		if (node->children[i] != node)
			DeleteGrammarTree(node->children[i]);
	}
	delete node;
}

bool FtraceParser::parse(void)
{
	quint32 s = lines.size();
	quint32 i;

	events.resize(0);
	events.reserve(s);

	for(i = 0; i < s; i++) {
		TraceLine &line = lines[i];
		TraceEvent event;
		event.argc = 0;
		event.argv = (TString**) ptrPool->preallocN(256);
		if (parseLine(&line, &event)) {
			ptrPool->commitN(event.argc);
			events.push_back(event);
		}
	}

	return true;
}

void FtraceParser::preScan()
{
	unsigned long i;

	nrEvents = events.size();
	lastEvent = nrEvents - 1;
	maxCPU = 0;
	startTime = 0;
	endTime = 0;
	minFreq = 2147483647;
	maxFreq = 0;
	minIdleState = 31000;
	maxIdleState = -31000;
	nrMigrateEvents = 0;

	for (i = 0; i < nrEvents; i++) {
		TraceEvent &event = events[i];
		if (event.cpu > maxCPU)
			maxCPU = event.cpu;
		if (cpuidle_event(event)) {
			int state = cpuidle_state(event);
			unsigned int cpu = cpuidle_cpu(event);
			if (cpu > maxCPU)
				maxCPU = cpu;
			if (state < minIdleState)
				minIdleState = state;
			if (state > maxIdleState)
				maxIdleState = state;
		} else if (cpufreq_event(event)) {
			unsigned int cpu = cpufreq_cpu(event);
			unsigned int freq = cpufreq_freq(event);
			if (freq > maxFreq)
				maxFreq = freq;
			if (freq < minFreq)
				minFreq = freq;
			if (cpu > maxCPU)
				maxCPU = cpu;
		} else if (sched_migrate(event)) {
			unsigned int dest = sched_migrate_destCPU(event);
			unsigned int orig = sched_migrate_origCPU(event);
			if (dest > maxCPU)
				maxCPU = dest;
			if (orig > maxCPU)
				maxCPU = dest;
			nrMigrateEvents++;
		}
	}

	if (nrEvents >= 2) {
		startTime = events[0].time;
		endTime = events[lastEvent].time;
	}

	if (cpuTaskMaps != NULL)
		delete[] cpuTaskMaps;

	cpuTaskMaps = new QMap<unsigned int, Task>[maxCPU];
}

void FtraceParser::processMigration()
{
}

static __always_inline void processSwitchEvent(TraceEvent &event,
					       QMap<unsigned int, Task>
					       *taskMaps, double &startTime,
					       MemPool *pool)
{
	unsigned int cpu = event.cpu;
	double oldtime = event.time - FAKE_DELTA;
	double newtime = event.time + FAKE_DELTA;
	unsigned int oldpid = sched_switch_oldpid(event);
	unsigned int newpid = sched_switch_newpid(event);

	/* Handle the outgoing task */
	Task &oldtask = taskMaps[cpu][oldpid]; /* Modifiable reference */
	if (oldtask.lastT == 0) { /* 0 means task is newly constructed above */
		double lastT = oldtask.lastT;
		oldtask.pid = oldpid;

		/* Apparenly this task was on CPU when we started tracing */
		oldtask.timev.push_back(startTime);
		oldtask.data.push_back(1);
		oldtask.t.push_back(lastT);
		lastT += 1;

		oldtask.timev.push_back(oldtime);
		oldtask.data.push_back(1);
		oldtask.t.push_back(lastT);
		lastT += 1;

		oldtask.timev.push_back(oldtime);
		oldtask.data.push_back(0);
		oldtask.t.push_back(lastT);
		lastT += 1;

		oldtask.lastT = lastT;
		oldtask.name = sched_switch_oldname_strdup(event, pool);
	} else {
		double lastT = oldtask.lastT;

		oldtask.timev.push_back(oldtime);
		oldtask.data.push_back(1);
		oldtask.t.push_back(lastT);
		lastT += 1;

		oldtask.timev.push_back(oldtime);
		oldtask.data.push_back(0);
		oldtask.t.push_back(lastT);
		lastT += 1;

		oldtask.lastT = lastT;
	}

	/* Handle the incoming task */
	Task &newtask = taskMaps[cpu][newpid]; /* Modifiable reference */
	if (newtask.lastT == 0) { /* 0 means task is newly constructed above */
		double lastT = newtask.lastT;
		newtask.pid = newpid;

		newtask.timev.push_back(startTime);
		newtask.data.push_back(0);
		newtask.t.push_back(lastT);
		lastT += 1;

		newtask.timev.push_back(newtime);
		newtask.data.push_back(0);
		newtask.t.push_back(lastT);
		lastT += 1;

		newtask.timev.push_back(newtime);
		newtask.data.push_back(0);
		newtask.t.push_back(lastT);
		lastT += 1;

		newtask.lastT = lastT;
		newtask.name = sched_switch_newname_strdup(event, pool);
	} else {
		double lastT = newtask.lastT;

		newtask.timev.push_back(newtime);
		newtask.data.push_back(0);
		newtask.t.push_back(lastT);
		lastT += 1;

		newtask.timev.push_back(newtime);
		newtask.data.push_back(1);
		newtask.t.push_back(lastT);
		lastT += 1;

		newtask.lastT = lastT;
	}
}

void FtraceParser::processSched()
{
	unsigned long i;
	for (i = 0; i < nrEvents; i++) {
		TraceEvent &event = events[i];
		if (sched_switch(event)) {
			processSwitchEvent(event, cpuTaskMaps, startTime,
					   taskNamePool);
		}
	}
}

void FtraceParser::processCPUfreq()
{
}
