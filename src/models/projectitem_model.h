#ifndef PITEM_MODEL_H
#define PITEM_MODEL_H

#include <gtkmm.h>

class ProjectItemModel : public Gtk::TreeModelColumnRecord {
    public:
        ProjectItemModel();
        Gtk::TreeModelColumn<int> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
        Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_icon;
};

#endif