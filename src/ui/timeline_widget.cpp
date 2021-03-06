#include "timeline_widget.h"
#include "../project/project.h"

#include <algorithm>
#include <vector>
#include <iostream>

TimelineWidget::TimelineWidget(BaseObjectType *drawingArea, Glib::RefPtr<Gtk::Builder> &gladeRef, Project *project) : 
    Gtk::DrawingArea(drawingArea)
    , m_project(project) 
{
    add_events(Gdk::BUTTON_PRESS_MASK);

    m_currentFrame = 0;
    this->signal_button_press_event().connect(sigc::mem_fun(*this, &TimelineWidget::onClick));
    this->signal_size_allocate().connect(sigc::mem_fun(*this, &TimelineWidget::onAllocate));
    this->signal_drag_data_received().connect(sigc::mem_fun(*this, &TimelineWidget::onDragReceived));
    this->signal_drag_motion().connect(sigc::mem_fun(*this, &TimelineWidget::onDragMotion));

    m_separatorPosition = get_allocated_width() / 4;
}

TimelineWidget::~TimelineWidget() {}

bool TimelineWidget::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->save();

    // Allocation defines the area that Gtk uses for the timeline, and we can use this to allow cairo to draw
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    const int start_x = allocation.get_x();
    const int start_y = allocation.get_y();

    cr->set_line_width(5.0);

    // Draw the separator line that designates track markers from the timeline
    // Default at 1/4 the width of the timeline widget
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(m_separatorPosition, start_y);
    cr->line_to(m_separatorPosition, start_y + height);
    cr->stroke();

    // Draw a line at the current frame position on the timeline.
    int markerPosition = getXFromFrame(m_currentFrame);
    cr->set_line_width(2.0);
    cr->set_source_rgb(1.0, 0.2, 0.0);
    cr->move_to(markerPosition, start_y);
    cr->line_to(markerPosition, start_y + height);
    cr->stroke();

    // Draw an arrow for the line that sits above the timeline
    int arrow_size = 20;
    cr->set_source_rgb(0.0, 0.5, 0.5);
    cr->move_to(markerPosition-(arrow_size/2), start_y);
    cr->line_to(markerPosition+(arrow_size/2), start_y);
    cr->line_to(markerPosition, start_y + arrow_size);
    cr->fill();

    // Draw a clip that is currently being added to the timeline
    if (m_currentMediaDrag > 0) {
        cr->set_source_rgb(0.0, 0.0, 0.0);
        cr->move_to(m_dragLoc.x, m_dragLoc.y);
        // TODO: Replace 48 with length in frames, calculated. Replace 12 with desired zoom height
        cr->line_to(m_dragLoc.x + 48, m_dragLoc.y);
        cr->line_to(m_dragLoc.x + 48, m_dragLoc.y-12);
        cr->line_to(m_dragLoc.x, m_dragLoc.y-12);
        cr->close_path();
    }

    cr->restore();
    return true;
}

bool TimelineWidget::onClick(GdkEventButton *event) {
    // Button down, Left mouse button
    if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY)
	{
        // Set the current position of the play marker
        m_currentFrame = getFrameFromX(event->x);
        queue_draw();
		return true;
    }
    return false;
}

void TimelineWidget::onDragReceived(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y,
                                    const Gtk::SelectionData &selection_data, guint info, guint time)
{
    if (selection_data.get_length() <= 0) {
        context->drag_finish(false, false, time);
        return;
    }

    if (selection_data.get_target() == "PROJECT_ITEM") {
        // Cast the data to the underlying ProjectItem
        ProjectItem *draggedItem = m_project->getMediaById(m_currentMediaDrag);
        std::cout << "Dragged Item:" << draggedItem->getName() << std::endl;

        m_currentMediaDrag = -1;
    }

    context->drag_finish(true, false, time);
}

bool TimelineWidget::onDragMotion(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, guint time) {
    // TODO: Place the marker on the timeline dependently
    this->drag_highlight();
    m_dragLoc.x = x;
    m_dragLoc.y = y;

    queue_draw();

    return true;
}

void TimelineWidget::setCurrentDragMedia(int media) {
    m_currentMediaDrag = media;
}

void TimelineWidget::onAllocate(Gtk::Allocation &alloc) {
    // All things that need dynamic sizing go here
}

int TimelineWidget::getFrameFromX(int xpos) {
    float interval = std::max(0.0f, ((float) (xpos - m_separatorPosition) / (float) (get_allocated_width() - m_separatorPosition)));
    return interval * m_project->getSettings().length;
}

int TimelineWidget::getXFromFrame(int frame) {
    return ((float) frame / m_project->getSettings().length) * (get_allocated_width() - m_separatorPosition) + m_separatorPosition;
}