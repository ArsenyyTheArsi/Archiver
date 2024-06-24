#include "functions.h"

void create_archive(const char* archive_path, const char* files_to_archive, int compression_level) {
    FILE* archive_file = fopen(archive_path, "wb");
    if (archive_file == NULL) {
        printf("Error: cannot create archive\n");
        return;
    }

    // Split the list of files into separate lines
    char* token = strtok((char*)files_to_archive, " ");
    while (token != NULL) {
        if (verbose) {
            printf("Adding file: %s\n", token);
        }

        // Open a file for reading
        FILE* file = fopen(token, "rb");
        if (file == NULL) {
            printf("Error: cannot open file %s\n", token);
            continue; // Skip this file
        }

        // To archive information about a file (without compression)
        char header[30] = { 0 }; // The title of the file in the archive
        sprintf(header, "File: %s\n", token); // Write the file name
        fwrite(header, strlen(header), 1, archive_file);

        // To archive the contents of the file
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            fwrite(buffer, 1, bytes_read, archive_file);
        }

        fclose(file);
        token = strtok(NULL, " ");
    }

    fclose(archive_file);
}

void add_files_to_archive(const char* archive_path, const char* files_to_archive, int compression_level) { 
    FILE* archive_file = fopen(archive_path, "ab"); // Open the archive in the add mode 
    if (archive_file == NULL) { 
        printf("Error: cannot open archive\n"); 
        return; 
    } 
 
    // Split the list of files into separate lines 
    char* token = strtok((char*)files_to_archive, " "); 
    while (token != NULL) { 
        if (verbose) { 
            printf("Adding file: %s\n", token); 
        } 
 
        // Open a file for reading 
        FILE* file = fopen(token, "rb"); 
        if (file == NULL) { 
            printf("Error: cannot open file %s\n", token); 
            continue; // Skip this file 
        } 
 
        // To archive information about a file (without compression) 
        char header[30] = { 0 }; // The title of the file in the archive 
        sprintf(header, "File: %s\n", token); // Write the file name 
        fwrite(header, strlen(header), 1, archive_file); 
 
        // To archive the contents of the file 
        char buffer[1024]; 
        size_t bytes_read; 
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) { 
            fwrite(buffer, 1, bytes_read, archive_file); 
        } 
 
        fclose(file); 
        token = strtok(NULL, " "); 
    } 
 
    fclose(archive_file); 
}
