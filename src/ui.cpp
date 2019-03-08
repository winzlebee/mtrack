#include <gtk/gtk.h>
#include <iostream>

#include "ui.h"

void Dialog::setWidget(GtkWidget *w) {
    this->widget = w;
}

GtkWidget* Dialog::getWidget() {
    return this->widget;
}

