#ifndef CCONS_CONSOLE_H
#define CCONS_CONSOLE_H

//
// Defines the IConsole interface used by ccons.cpp to process user input,
// as well as the concrete Console class, implementing said interface and
// providing C input processing using the clang and llvm libraries.
//
// Part of ccons, the interactive console for the C programming language.
//
// Copyright (c) 2009 Alexei Svitkine. This file is distributed under the
// terms of MIT Open Source License. See file LICENSE for details.
//

#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <llvm/LLVMContext.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/ADT/OwningPtr.h>

#include <clang/Basic/LangOptions.h>

namespace llvm {
	struct GenericValue;
	class ExecutionEngine;
	class Function;
	class Linker;
} // namespace llvm

namespace clang {
	class DeclStmt;
	class Expr;
	class Preprocessor;
	class QualType;
	class SourceManager;
	class Stmt;
	class VarDecl;
} // namespace clang

namespace ccons {

class Parser;
class DiagnosticsProvider;
class MacroDetector;

//
// IConsole interface
//

class IConsole {

public:

	virtual ~IConsole() {}

	// Returns the prompt that should be presented to the user.
	virtual const char * prompt() const = 0;

	// Returns the initial input string that should be prepended.
	virtual const char * input() const = 0;

	// Process the specified line of user input.
	virtual void process(const char *line) = 0;

};

//
// Console implementation
//

class Console : public IConsole {

public:

	Console(bool _debugMode,
	        std::ostream& out = std::cout,
	        std::ostream& err = std::cerr);
	virtual ~Console();

	const char * prompt() const;
	const char * input() const;
	void process(const char *line);

private:

	enum LineType {
		StmtLine,
		DeclLine,
		PrprLine,
	};

	typedef std::pair<std::string, LineType> CodeLine;

	void reportInputError();

	bool shouldPrintCString(const char *p);
	void printGV(const llvm::Function *F,
	             const llvm::GenericValue& GV,
	             const clang::QualType& QT);
	void processVarDecl(const std::string& src,
	                    const clang::VarDecl *VD,
	                    std::vector<std::string> *decls,
	                    std::vector<std::string> *stmts,
	                    std::string *appendix);
	bool handleDeclStmt(const clang::DeclStmt *DS,
	                    const std::string& src,
	                    std::string *appendix,
	                    std::string *funcBody,
	                    std::vector<CodeLine> *moreLines);
	std::string genAppendix(const char *source,
	                        const char *line,
	                        std::string *fName,
	                        clang::QualType& QT,
	                        std::vector<CodeLine> *moreLines,
	                        bool *hadErrors);
	std::string genSource(const std::string& appendix) const;
	int splitInput(const std::string& source,
	               const std::string& input,
	               std::vector<std::string> *statements);
	clang::Stmt * locateStmt(const std::string& line,
	                         std::string *src);

	bool compileLinkAndRun(const std::string& src,
                         const std::string& fName,
                         const clang::QualType& retType);

	bool _debugMode;
	std::ostream& _out;
	std::ostream& _err;
	mutable llvm::raw_os_ostream _raw_err;
	clang::LangOptions _options;
	llvm::OwningPtr<Parser> _parser;
	llvm::LLVMContext _context;
	llvm::OwningPtr<llvm::Linker> _linker;
	llvm::OwningPtr<llvm::ExecutionEngine> _engine;
	llvm::OwningPtr<DiagnosticsProvider> _dp;
	MacroDetector *_macros;
	std::vector<std::string> _prevMacros;
	std::vector<CodeLine> _lines;
	std::string _buffer;
	std::string _prompt;
	std::string _input;
	unsigned _funcNo;
	FILE *_tempFile;

};

} // namespace ccons

#endif // CCONS_CONSOLE_H
