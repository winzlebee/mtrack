#ifndef PITEM_MODEL_H
#define PITEM_MODEL_H

#include <gtkmm.h>

class ProjectItemModel : public Gtk::TreeModelColumnRecord {
    public:
        ProjectItemModel();
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
};

#endif