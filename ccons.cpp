//
// Defines main() function and command-line arguments used by ccons.
//
// Part of ccons, the interactive console for the C programming language.
//
// Copyright (c) 2009 Alexei Svitkine. This file is distributed under the
// terms of MIT Open Source License. See file LICENSE for details.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include <llvm/ADT/OwningPtr.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Signals.h>
#include <llvm-c/Target.h>

#include "Console.h"
#include "EditLineReader.h"
#include "InternalCommands.h"
#include "RemoteConsole.h"

using std::string;
using ccons::Console;
using ccons::IConsole;
using ccons::RemoteConsole;
using ccons::SerializedOutputConsole;
using ccons::LineReader;
using ccons::EditLineReader;
using ccons::StdInLineReader;

static llvm::cl::opt<bool>
	DebugMode("ccons-debug",
			llvm::cl::desc("Print debugging information"));
static llvm::cl::opt<bool>
	UseStdIo("ccons-use-std-io",
			llvm::cl::desc("Use standard IO for input and output"));
static llvm::cl::opt<bool>
	SerializedOutput("ccons-serialized-output",
			llvm::cl::desc("Output will be serialized"));
static llvm::cl::opt<bool>
	MultiProcess("ccons-multi-process",
			llvm::cl::desc("Run in multi-process mode"));

static IConsole * createConsole(const char * command)
{
	if (MultiProcess)
		return new RemoteConsole(command, DebugMode);
	else if (SerializedOutput)
		return new SerializedOutputConsole(DebugMode);		
	else
		return new Console(DebugMode);
}

static LineReader * createReader()
{
	if (UseStdIo)
		return new StdInLineReader;
	else
		return new EditLineReader;
}

extern "C" void LLVMInitializeX86TargetMC();

int main(const int argc, char **argv)
{
	llvm::cl::SetVersionPrinter(ccons::PrintVersionInformation);
	llvm::cl::ParseCommandLineOptions(argc, argv);

	if (DebugMode && !SerializedOutput) {
		std::cerr << "NOTE: Debugging information will be displayed.\n";
		llvm::sys::PrintStackTraceOnErrorSignal();
	}

	LLVMInitializeNativeTarget();

	llvm::OwningPtr<IConsole> console(createConsole(argv[0]));
	llvm::OwningPtr<LineReader> reader(createReader());

	const char *line = reader->readLine(console->prompt(), console->input());
	while (line) {
		console->process(line);
		line = reader->readLine(console->prompt(), console->input());
	}

	return 0;
}

