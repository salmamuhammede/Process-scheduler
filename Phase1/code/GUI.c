#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include "headers.h" 

typedef struct {
    GtkComboBox *combobox;
    GtkEntry *entry;
    GtkWidget *quantum_label; // Label for quantum value
    GtkWidget *quantum_entry; // Entry field for quantum value
} Widgets;



void create_image_from_text(const char *input_file, const char *output_file) {
    // Open the text file for reading
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open input file\n");
        return;
    }

    // Create a Cairo surface (e.g., PNG file)
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 200);
    cairo_t *cr = cairo_create(surface);

    // Set background color to black
    cairo_set_source_rgb(cr, 0, 0, 0); // Black
    cairo_paint(cr); // Fill the surface with the background color

    // Set font options
    cairo_select_font_face(cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD); // Bold
    cairo_set_font_size(cr, 12.0);

    // Set text color to white
    cairo_set_source_rgb(cr, 1, 1, 1); // White

    // Read text from file and render onto Cairo surface
    char character;
    double y = 20.0;
    double x=10.0;
    while ((character = fgetc(file)) != EOF) {
    // Move the pen to the current position
        cairo_move_to(cr, x, y);

        // Check if the character is a newline
        if (character == '\n') {
            // Move to the next line
            y += 20.0;
            x = 10.0; // Reset x position to start of the line
        }
        // Check if the character is a space
        else if (character == ' ') {
            // Move to the next position
            x += 10.0;
        }
        else if (character == '\t') {
            // Move to the next position
            x += 40.0;
        }
        // For other characters
        else {
            // Convert character to string (assuming it's ASCII)
            char str[2] = {character, '\0'};
            // Show the character on the Cairo surface
            cairo_show_text(cr, str);
            // Move to the next position
            x += 8.0;
        }
    }

    // Finish drawing
    cairo_destroy(cr);

    // Save the surface as a PNG image file
    cairo_surface_write_to_png(surface, output_file);

    // Clean up
    cairo_surface_destroy(surface);
    fclose(file);
}

// Function to handle combobox selection change event
void on_combobox_changed(GtkComboBox *combobox, gpointer data) {
    // Cast the data pointer to Widgets structure
    Widgets *widgets = (Widgets *)data;

    // Get the selected value from the combobox
    gint active_index = gtk_combo_box_get_active(combobox);
    int selected_algorithm = active_index + 1;

    // Show/hide quantum label and entry field based on the selected algorithm
    if (selected_algorithm == 1) { // 1 corresponds to "RR"
        gtk_widget_show(widgets->quantum_label);
        gtk_widget_show(widgets->quantum_entry);
    } else {
        gtk_widget_hide(widgets->quantum_label);
        gtk_widget_hide(widgets->quantum_entry);
    }
}

// Function to handle button click event
void on_button_clicked(GtkWidget *widget, gpointer data) {
    // Cast the data pointer to Widgets structure
    Widgets *widgets = (Widgets *)data;

    // Get the selected value from the combobox
    GtkComboBox *combobox = widgets->combobox;
    gint active_index = gtk_combo_box_get_active(combobox);
    int selected_algorithm = active_index + 1;

    // Get text input from the entry field
    GtkEntry *entry = widgets->entry;
    const char *process_info = gtk_entry_get_text(entry);
int quantum;
    // If RR is selected, also get the quantum value
    if (selected_algorithm == 1) { // 1 corresponds to "RR"
        const char *quantum_str = gtk_entry_get_text(GTK_ENTRY(widgets->quantum_entry));
        quantum = atoi(quantum_str);
        // Now you have the quantum value, you can use it as needed
    }
    char commmand[300];
    snprintf(commmand,sizeof(commmand),"./process_generator.out \"%s\" %d %d",process_info,selected_algorithm,quantum);
    system(commmand);
    // Generate image with selected information
    /*int stat
    key_t p;
    do{
    p=waitpid(-1,&stat,WNOHANG);
    }while(p==0);*/
    sleep(20);

    create_image_from_text("scheduler.log", "image1.png");

    create_image_from_text("scheduler.perf", "image2.png");
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Process Scheduler Image Generator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a label for the combobox
    GtkWidget *label = gtk_label_new("Scheduling Algorithm:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    // Create a combobox with options ("RR", "HPF", "SRTN")
    GtkWidget *combobox = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox), "RR");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox), "HPF");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox), "SRTN");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), 0); // Default to first option
    gtk_box_pack_start(GTK_BOX(vbox), combobox, FALSE, FALSE, 0);

    // Create an entry field for text input
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "File path...");
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    // Create a label and entry field for quantum value
    GtkWidget *quantum_label = gtk_label_new("Quantum:");
    GtkWidget *quantum_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), quantum_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), quantum_entry, FALSE, FALSE, 0);

    // Create a button to generate the image
    GtkWidget *button = gtk_button_new_with_label("RUN");

    // Create an instance of the Widgets structure and initialize its members
    Widgets widgets;
    widgets.combobox = GTK_COMBO_BOX(combobox);
    widgets.entry = GTK_ENTRY(entry);
    widgets.quantum_label = quantum_label;
    widgets.quantum_entry = quantum_entry;

    // Connect the combobox changed signal, passing the Widgets instance as data
    g_signal_connect(combobox, "changed", G_CALLBACK(on_combobox_changed), &widgets);

    // Connect the button click signal, passing the Widgets instance as data
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), &widgets);

    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
