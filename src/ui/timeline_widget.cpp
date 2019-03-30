#include "timeline_widget.h"
#include "../project/project.h"

#include <algorithm>
#include <iostream>

TimelineWidget::TimelineWidget(BaseObjectType *drawingArea, Glib::RefPtr<Gtk::Builder> &gladeRef, Project *project) : 
    Gtk::DrawingArea(drawingArea)
    , m_project(project) 
{
    add_events(Gdk::BUTTON_PRESS_MASK);

    m_currentFrame = 0;
    this->signal_button_press_event().connect(sigc::mem_fun(*this, &TimelineWidget::onClick));
    this->signal_size_allocate().connect(sigc::mem_fun(*this, &TimelineWidget::onAllocate));

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
    cr->set_line_width(2.0);
    cr->set_source_rgb(1.0, 0.2, 0.0);
    cr->move_to(getXFromFrame(m_currentFrame), start_y);
    cr->line_to(getXFromFrame(m_currentFrame), start_y + height);
    cr->stroke();

    cr->restore();
    return true;
}

bool TimelineWidget::onClick(GdkEventButton *event) {
    // Button down, Left mouse button
    if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY)
	{
        // Set the current position of the play marker
        m_currentFrame = getFrameFromX(event->x);
        std::cout << m_currentFrame << std::endl;
        queue_draw();
		return true;
    }
    return false;
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