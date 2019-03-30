#include "timeline_widget.h"
#include "../project/project.h"

TimelineWidget::TimelineWidget(BaseObjectType *drawingArea, Glib::RefPtr<Gtk::Builder> &gladeRef, Project *project) : 
    Gtk::DrawingArea(drawingArea)
    , m_project(project) 
{
    
}

TimelineWidget::~TimelineWidget() {}

bool TimelineWidget::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->save();

    // Allocation defines the area that Gtk uses for the timeline, and we can use this to allow cairo to draw
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    cr->set_line_width(5.0);

    // Draw the separator line that designates track markers from the timeline
    // Default at 1/4 the width of the timeline widget
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(width/4, 0);
    cr->line_to(width/4, height);
    cr->stroke();

    // Draw a line at the current frame position on the timeline.

    cr->restore();
    return true;
}