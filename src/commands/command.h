#pragma once


// A command is representative of anything that happens to the project. Use for all reversible actions.
class Command {
public:
	virtual void undo() = 0;
	virtual void perform() = 0;
};