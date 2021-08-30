#include <gtk/gtk.h>

#include "exampleapp.h"
#include "exampleappwin.h"

struct _ExampleAppWindow {
    GtkApplicationWindow parent;    
};

struct _ExampleAppWindowPrivate
{
  GtkWidget *stack;
};

typedef struct _ExampleAppWindowPrivate ExampleAppWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(ExampleAppWindow, example_app_window, GTK_TYPE_APPLICATION_WINDOW);

// G_DEFINE_TYPE (ExampleAppWindow, example_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void example_app_window_init (ExampleAppWindow *win) {
    gtk_widget_init_template (GTK_WIDGET (win));
}

static void example_app_window_class_init (ExampleAppWindowClass *class) {
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class),
                                               "/org/gtk/exampleapp/window.ui");
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), ExampleAppWindow, stack);
}

ExampleAppWindow *example_app_window_new (ExampleApp *app) {
    ExampleAppWindow *ptr =  g_object_new (EXAMPLE_APP_WINDOW_TYPE, "application", app, NULL);
    gtk_window_set_default_size (GTK_WINDOW (ptr), 400, 400);
    gtk_window_set_title (GTK_WINDOW (ptr), "Sample App");
    return ptr;
}

void example_app_window_open (ExampleAppWindow *win, GFile *file) {
  ExampleAppWindowPrivate *priv;
  gchar *basename; // std::string basename
  GtkWidget *scrolled, *view;
  gchar *contents;
  gsize length;

  priv = example_app_window_get_instance_private (win);
  basename = g_file_get_basename (file);

  scrolled = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolled);
  gtk_widget_set_hexpand (scrolled, TRUE);
  gtk_widget_set_vexpand (scrolled, TRUE);
  view = gtk_text_view_new ();
  gtk_text_view_set_editable (GTK_TEXT_VIEW (view), TRUE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (view), TRUE);
  gtk_widget_show (view);
  gtk_container_add (GTK_CONTAINER (scrolled), view);
  gtk_stack_add_titled (GTK_STACK (priv->stack), scrolled, basename, basename);

  if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
    {
      GtkTextBuffer *buffer;

      buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
      gtk_text_buffer_set_text (buffer, contents, length);
      g_free (contents);
    }

  g_free (basename);
}