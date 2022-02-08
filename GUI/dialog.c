#include <gtk/gtk.h>

typedef struct {
    GtkWidget *container;
    GtkWindow *window;
    gboolean dialogs_created;
} window_ctx_t;

typedef struct {
    GtkEntry *entry;
    window_ctx_t *window_ctx;
} first_dialog_data_t;

static void first_dialog_create(gpointer context);

static void another_dialog_response(GtkWidget *dialog, gint response_id,
    gpointer ctx)
{
    window_ctx_t *win_ctx = ctx;

    gtk_widget_destroy(dialog);
    win_ctx->dialogs_created = FALSE;
}

static void cleanup_first_dialog(GtkWidget *widget, gpointer data)
{
    g_free(data);
}

/* response callback of the first dialog */
static void add_new_button(GtkWidget *dialog, gint response_id,
    gpointer ctx)
{
    GtkWidget *button;
    GtkWidget *button_text_dialog;
    first_dialog_data_t *data = ctx;

    if (response_id != GTK_RESPONSE_OK) {
        data->window_ctx->dialogs_created = FALSE;
        goto Exit;
    }

    /* Create a new button with the label from text entry */
    button = gtk_button_new_with_label(gtk_entry_get_text(data->entry));
    /* When the button receives the "clicked" signal, it will again open
     * "new button" dialog */
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(first_dialog_create),
        data->window_ctx);
    /* add button to the container */
    gtk_container_add(GTK_CONTAINER(data->window_ctx->container), button);
    /* show button */
    gtk_widget_show(button);

    /* create another dialog */
    button_text_dialog = gtk_dialog_new_with_buttons("new button",
        data->window_ctx->window, GTK_DIALOG_DESTROY_WITH_PARENT, "OK",
        GTK_RESPONSE_OK, NULL);
    g_signal_connect(button_text_dialog, "response",
        G_CALLBACK(another_dialog_response), (gpointer)data->window_ctx);

    gtk_widget_show_all(button_text_dialog);

Exit:
    gtk_widget_destroy(dialog);
}

static void first_dialog_create(gpointer context)
{
    first_dialog_data_t *data;
    window_ctx_t *win_ctx = context;

    if (win_ctx->dialogs_created)
        return;

    /* lock buttons handler */
    win_ctx->dialogs_created = TRUE;

    /* allocate structure for dialog context */
    data = g_new0(first_dialog_data_t, 1);

    /* create first dialog, its label and text entry */
    GtkWidget *new_button_dialog = gtk_dialog_new_with_buttons("new button",
        win_ctx->window, GTK_DIALOG_DESTROY_WITH_PARENT,"OK",
        GTK_RESPONSE_OK, NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(
    GTK_DIALOG(new_button_dialog));
    GtkWidget *button_name_label = gtk_label_new("Press escape at any time to "
        "cancel.\ntype in the button name");
    gtk_container_add(GTK_CONTAINER(content_area), button_name_label);

    GtkWidget * button_name_entry = gtk_entry_new();
    gtk_entry_set_activates_default((GtkEntry*)button_name_entry,TRUE);
    gtk_container_add(GTK_CONTAINER(content_area), button_name_entry);

    /* provide data to response handler */
    data->entry = (GtkEntry *)button_name_entry;
    data->window_ctx = win_ctx;
    g_signal_connect(new_button_dialog, "response", G_CALLBACK(add_new_button),
        data);

    /* handler to free dialog context 'data' when destroed */
    g_signal_connect(new_button_dialog, "destroy",
        G_CALLBACK(cleanup_first_dialog), data);

    /* show first dialog */
    gtk_widget_show_all(new_button_dialog);
}

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *box1;
    GtkWidget *button;
    window_ctx_t win_ctx;

    gtk_init (&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    box1 = gtk_hbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), box1);

    /* set window context */
    win_ctx.window = (GtkWindow *)window;
    win_ctx.container = box1;
    win_ctx.dialogs_created = FALSE;

    g_signal_connect (window, "destroy", G_CALLBACK(destroy), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    button = gtk_button_new_with_label("Start");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(first_dialog_create),
        &win_ctx);
    gtk_container_add(GTK_CONTAINER(box1), button);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
