#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "../include/utils.h"
#include <gtk/gtk.h>

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File opening failed");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    if (content) {
        fread(content, 1, length, file);
        content[length] = '\0';
    }

    fclose(file);
    return content;
}

void print_problem_details(cJSON* problem) {
    cJSON* id = cJSON_GetObjectItemCaseSensitive(problem, "id");
    cJSON* title = cJSON_GetObjectItemCaseSensitive(problem, "title");
    cJSON* description = cJSON_GetObjectItemCaseSensitive(problem, "description");
    cJSON* expected_output = cJSON_GetObjectItemCaseSensitive(problem, "expected_output");

    if (cJSON_IsString(id) && (id->valuestring != NULL)) {
        printf("ID: %s\n", id->valuestring);
    }
    if (cJSON_IsString(title) && (title->valuestring != NULL)) {
        printf("Title: %s\n", title->valuestring);
    }
    if (cJSON_IsString(description) && (description->valuestring != NULL)) {
        printf("Description: %s\n", description->valuestring);
    }
    if (cJSON_IsString(expected_output) && (expected_output->valuestring != NULL)) {
        printf("Expected Output: %s\n", expected_output->valuestring);
    }
}

void display_json_in_window(const char* filename, const char* difficulty) {
    // Read JSON file and parse it
    char* json_content = read_file(filename);
    if (json_content == NULL) {
        return;
    }

    cJSON* json = cJSON_Parse(json_content);
    free(json_content);

    if (json == NULL) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return;
    }

    cJSON* problems_array = cJSON_GetObjectItemCaseSensitive(json, difficulty);
    if (!cJSON_IsArray(problems_array)) {
        printf("Invalid JSON format for difficulty: %s\n", difficulty);
        cJSON_Delete(json);
        return;
    }

    // Initialize GTK
    gtk_init(NULL, NULL);

    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "JSON Problems");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create a text view widget
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    // Add text view to the window
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    // Create a string to hold the JSON output
    GString *output = g_string_new("");

    // Iterate over JSON array and append to output string
    cJSON* problem = NULL;
    cJSON_ArrayForEach(problem, problems_array) {
        cJSON* id = cJSON_GetObjectItemCaseSensitive(problem, "id");
        cJSON* title = cJSON_GetObjectItemCaseSensitive(problem, "title");
        cJSON* description = cJSON_GetObjectItemCaseSensitive(problem, "description");
        cJSON* expected_output = cJSON_GetObjectItemCaseSensitive(problem, "expected_output");

        if (cJSON_IsString(id) && (id->valuestring != NULL)) {
            g_string_append_printf(output, "ID: %s\n", id->valuestring);
        }
        if (cJSON_IsString(title) && (title->valuestring != NULL)) {
            g_string_append_printf(output, "Title: %s\n", title->valuestring);
        }
        if (cJSON_IsString(description) && (description->valuestring != NULL)) {
            g_string_append_printf(output, "Description: %s\n", description->valuestring);
        }
        if (cJSON_IsString(expected_output) && (expected_output->valuestring != NULL)) {
            g_string_append_printf(output, "Expected Output: %s\n\n", expected_output->valuestring);
        }
    }

    // Set the text in the text view
    gtk_text_buffer_set_text(buffer, output->str, -1);

    // Free the output string
    g_string_free(output, TRUE);

    // Show all widgets
    gtk_widget_show_all(window);

    // Connect the destroy signal of the window to gtk_main_quit to ensure
    // the application quits when the window is closed
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Run the GTK main loop
    gtk_main();

    cJSON_Delete(json);
}
