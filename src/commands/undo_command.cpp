#include "undo_command.h"

UndoStack::UndoStack()
{
    // Setup signal undo connections
    
}

void UndoStack::push(UndoCommand *command)
{
    command->perform();
    m_undo.push(std::unique_ptr<UndoCommand>(command));
    
}

void UndoStack::clear()
{
    clearUndo();
    clearRedo();
}

void UndoStack::clearUndo()
{
    while (!m_undo.empty()) {
        m_undo.pop();
    }
}

void UndoStack::clearRedo()
{
    while (!m_redo.empty()) {
        m_redo.pop();
    }
}

void UndoStack::redo()
{
    if (m_redo.empty()) {
        return;
    }

    m_redo.top()->perform();
    m_undo.push(std::move(m_redo.top()));
    m_redo.pop();
}

void UndoStack::undo()
{
    if (m_undo.empty()) {
        return;
    }

    m_undo.top()->undo();
    m_redo.push(std::move(m_undo.top()));
    m_undo.pop();

}