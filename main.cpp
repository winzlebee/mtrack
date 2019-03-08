#include <GL/gl.h>
#include <gtk/gtk.h>

#include <iostream>

// New data structure to store widget to prevent memory leaks
// Automatically handles deletion of widget on program completion as per RAII
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

// Called when the OpenGL instance is created
static void on_realize(GtkGLArea *area) {
    // TODO: Error check the GtkGLArea
    
    // Initialize the camera with a focal length
}

static gboolean render(GtkGLArea *area, GdkGLContext *context) {
    
    glClearColor(0.2, 0.2, 0.2, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the scene here
    
    return TRUE;
}

static void open_video(GtkWindow *parent) {
    GtkWidget *chooserDialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    
    chooserDialog = gtk_file_chooser_dialog_new("Open Media File",
                                         parent,
                                         action,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Import", GTK_RESPONSE_ACCEPT,
                                         NULL);
    res = gtk_dialog_run(GTK_DIALOG(chooserDialog));
    
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(chooserDialog);
        filename = gtk_file_chooser_get_filename(chooser);
        
        // We've retrieved the filename, so we're good. Use it here.
        std::cout << "Importing media file: " << filename << std::endl;
        
        g_free(filename);
    }
    
    gtk_widget_destroy(chooserDialog);
}

int main(int argc, char **argv) {
    
    gtk_init(&argc, &argv);
    
    Widget window(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_title(GTK_WINDOW(window.get()), "mtrack v1.0");
    gtk_widget_show_all(window.get());
    
    // Create a layout for the window
    Widget vbox(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    
    // Menus
    Widget menuBar(gtk_menu_bar_new());
    Widget fileMenu(gtk_menu_new());
    
    Widget fileItem(gtk_menu_item_new_with_mnemonic("_ File"));
    Widget importVideoItem(gtk_menu_item_new_with_label("Import Media..."));
    Widget quitItem(gtk_menu_item_new_with_label("Quit"));
    
    // Setup the menus
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileItem.get()), fileMenu.get());
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu.get()), importVideoItem.get());
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu.get()), quitItem.get());
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBar.get()), fileItem.get());
    
    // Setup OpenGL view
    Widget gl_area(gtk_gl_area_new());
    g_signal_connect(gl_area.get(), "realize", G_CALLBACK(on_realize), NULL);
    g_signal_connect(gl_area.get(), "render", G_CALLBACK(render), NULL);
    
    // Add elements to GTK View
    gtk_box_pack_start(GTK_BOX(vbox.get()), menuBar.get(), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox.get()), gl_area.get(), TRUE, TRUE, 5);
    
    gtk_container_add(GTK_CONTAINER(window.get()), vbox.get());
    
    // Setup the signals
    g_signal_connect(G_OBJECT(quitItem.get()), "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(importVideoItem.get()), "activate", G_CALLBACK(open_video), window.get());
    g_signal_connect(G_OBJECT(window.get()), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_widget_show_all(window.get());
    
    // Run the main window
    gtk_main();
    return 0;
}
