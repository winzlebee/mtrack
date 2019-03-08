#include <string>

void gtk_widget_destroy(GtkWidget*);

class Widget {
    public:
        Widget(GtkWidget *w) {
            this->widget = w;
        }
        
        GtkWidget* get() {
            return this->widget;
        }
        
    private:
        GtkWidget *widget;
};

class Dialog {
    public:
        Dialog() {}
        
        Dialog(GtkWidget *w) {
            this->widget = w;    
        }
        
        void setWidget(GtkWidget *w);
        GtkWidget* getWidget();
        
        ~Dialog() {
            // Destroy the dialog at the end of scope
            gtk_widget_destroy(this->widget);
        }
    private:
        GtkWidget *widget;
};
