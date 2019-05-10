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
        void setCurrentDragMedia(int media);
    protected:
        //Overrides
        bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
        
        Project *m_project;
    private:
        struct vec2 {
            vec2() {};
            vec2(int xpos, int ypos) : x(xpos), y(ypos) {};
            int x;
            int y;
        };

        bool onClick(GdkEventButton *event);
        void onAllocate(Gtk::Allocation &alloc);
        void onDragRecieved(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y,
                            const Gtk::SelectionData &selection_data, guint info, guint time);
        bool onDragMotion(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, guint time); 	

	protected:
        int getFrameFromX(int xpos);
        int getXFromFrame(int frame);

        int m_separatorPosition;
        int m_currentFrame;
        vec2 m_dragLoc;
        int m_currentMediaDrag = -1;
};  


