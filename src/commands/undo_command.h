#pragma once

#include <memory>
#include <stack>

#include <gtkmm.h>

// A command is representative of anything that happens to the project. Use for all reversible actions.
class UndoCommand {
public:
	virtual void undo() = 0;
	virtual void perform() = 0;
};

class UndoStack {

public:

	UndoStack();

	void push(UndoCommand *command);

	void undo();
	void redo();

	void clear();

private:
	void clearUndo();
	void clearRedo();

	std::stack<std::unique_ptr<UndoCommand>> m_redo;
	std::stack<std::unique_ptr<UndoCommand>> m_undo;
};