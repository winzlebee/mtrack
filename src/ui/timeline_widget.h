#pragma once


#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

class Project;

/*
 * Timeline Widget. Responsible for displaying the clips in the sequence and their relations to each other.
 */
class TimelineWidget : public Gtk::DrawingArea {
    public:
        TimelineWidget(BaseObjectType *drawingArea, Glib::RefPtr<Gtk::Builder> &gladeRef, Project *project);
        virtual ~TimelineWidget();

        // Get the current position of the trackbar, in frames
        int getFramePosition();
    protected:
        //Overrides
        bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
        
        Project *m_project;
    private:
        bool onClick(GdkEventButton *event);
        void onAllocate(Gtk::Allocation &alloc);

        int getFrameFromX(int xpos);
        int getXFromFrame(int frame);

        int m_separatorPosition;
        int m_currentFrame;
};  


