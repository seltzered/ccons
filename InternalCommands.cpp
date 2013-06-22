//
// Functions to process built-in commands for ccons.
//
// Part of ccons, the interactive console for the C programming language.
//
// Copyright (c) 2009 Alexei Svitkine. This file is distributed under the
// terms of MIT Open Source License. See file LICENSE for details.
//

#include "InternalCommands.h"
#include "StringUtils.h"

#include <string.h>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/PathV1.h>

namespace ccons {

// Prints the help text.
static void HandleHelpCommand(const char *arg, bool debugMode,
                              std::ostream& out, std::ostream& err)
{
	oprintf(out, "The following commands are available:\n");
	oprintf(out, "  :help - displays this message\n");
	oprintf(out, "  :load <library path> - dynamically loads specified library\n");
	oprintf(out, "  :version - displays ccons version information\n");
}

// Prints the version text.
static void HandleVersionCommand(const char *arg, bool debugMode,
                                std::ostream& out, std::ostream& err)
{
	PrintVersionInformation(out);
}															

// Loads the library that was specified. 
static void HandleLoadCommand(const char *arg, bool debugMode,
                              std::ostream& out, std::ostream& err)
{
	if (debugMode)
		oprintf(err, "Attempting to load external library '%s'.\n", arg);

	llvm::sys::Path path(arg);
	if (path.isDynamicLibrary()) {
		std::string errMsg;
		llvm::sys::DynamicLibrary::LoadLibraryPermanently(arg, &errMsg);
		if (!errMsg.empty()) {
			oprintf(err, "Error: %s\n", errMsg.c_str());
		} else {
			oprintf(out, "Dynamic library loaded.\n");
		}
	} else {
		oprintf(out, "No dynamic library found at the specified path.\n");
	}
}

// Handle an internal command if it was specified. If handled, returns
// true; otherwise the input did not correspond to an internal command.
bool HandleInternalCommand(const char *input, bool debugMode,
                           std::ostream& out, std::ostream& err)
{
	while (isspace(*input)) input++;

	if (*input == ':') {
		struct {
			const char *name;
			void (*handler)(const char *arg, bool debugMode,
			                std::ostream& out, std::ostream& err);
		}	commands[] = {
			{ "help",    HandleHelpCommand    },
			{ "version", HandleVersionCommand },
			{ "load",    HandleLoadCommand    },
		};
		const unsigned commandCount = sizeof(commands)/sizeof(commands[0]);
		input++;
		for (unsigned i = 0; i < commandCount; i++) {
			const unsigned nameLength = strlen(commands[i].name);
			if (!strncmp(input, commands[i].name, nameLength) &&
			    (input[nameLength] == '\0' || isspace(input[nameLength]))) {
				int index = nameLength + 1;
				while (isspace(input[index])) index++;
				int length = strlen(input + index);
				while (length > 0 && isspace(input[index + length - 1]))
					length--;
				std::string args(&input[index], length);
				commands[i].handler(args.c_str(), debugMode, out, err);
				return true;
			}
		}
	}
	return false;
}

// Prints ccons version information to the specified ostream.
void PrintVersionInformation(std::ostream& out)
{
	oprintf(out, "ccons version 0.1 by Alexei Svitkine\n");
	oprintf(out, "Interactive Console for the C Programming Language\n");
	oprintf(out, "http://code.google.com/p/ccons\n");
}

// Prints ccons version information to std::out.
void PrintVersionInformation()
{
	PrintVersionInformation(std::cout);
}

} // namespace ccons
