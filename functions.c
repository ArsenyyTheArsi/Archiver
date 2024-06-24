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

void delete_files_from_archive(const char* archive_path, const char* files_to_archive) { 
    FILE* archive_file = fopen(archive_path, "rb"); 
    if (archive_file == NULL) { 
        printf("Error: cannot open archive\n"); 
        return; 
    } 
 
    // Temporary file for storing the archive with deleted files 
    char temp_filename[] = "temp_archive.tmp"; 
    FILE* temp_file = fopen(temp_filename, "wb"); 
    if (temp_file == NULL) { 
        printf("Error: cannot create temporary file\n"); 
        fclose(archive_file); 
        return; 
    } 
 
    // Split the list of files into separate lines 
    char* token = strtok((char*)files_to_archive, " "); 
    // Create an array to store the names of files to be deleted 
    char** files_to_delete = malloc(100 * sizeof(char*)); // Assume a maximum of 100 files 
    int file_count = 0; 
    while (token != NULL) { 
        if (verbose) { 
            printf("Deleting file: %s\n", token); 
        } 
        files_to_delete[file_count] = strdup(token); // Duplicate the line to avoid changing the original list 
        file_count++; 
        token = strtok(NULL, " "); 
    } 
 
    // Read data from the source archive and write it to a temporary file 
    char buffer[1024]; 
    char filename[256]; 
    while (fgets(buffer, sizeof(buffer), archive_file) != NULL) { 
        if (strncmp(buffer, "File: ", 6) == 0) { 
            sscanf(buffer, "File: %s\n", filename); 
 
            // Check if this file needs to be deleted 
            int delete_file = 0; 
            for (int i = 0; i < file_count; i++) { 
                if (strcmp(files_to_delete[i], filename) == 0) { 
                    delete_file = 1; 
                    break; 
                } 
            } 
 
            if (delete_file) { 
                // Skip writing file data to the temporary file 
                // Continue reading until the next "File: " 
                while (fgets(buffer, sizeof(buffer), archive_file) != NULL) { 
                    if (strncmp(buffer, "File: ", 6) == 0) { 
                        break; // Reached the next file 
                    } 
                } 
            } else { 
                // Write file header to the temporary file 
                fwrite(buffer, strlen(buffer), 1, temp_file); 
 
                // Write the contents of the file to the temporary file 
                while (fgets(buffer, sizeof(buffer), archive_file) != NULL) { 
                    if (strncmp(buffer, "File: ", 6) == 0) { 
                        break; // Reached the next file 
                    } 
                    fwrite(buffer, 1, strlen(buffer), temp_file); 
                } 
            } 
        } else { 
            // Write data to the temporary file (this is not file data) 
            fwrite(buffer, strlen(buffer), 1, temp_file); 
        } 
    } 
 
    fclose(archive_file); 
    fclose(temp_file); 
 
    // Free the memory allocated for the file array 
    for (int i = 0; i < file_count; i++) { 
        free(files_to_delete[i]); 
    } 
    free(files_to_delete); 
 
    // Move the temporary file to the original archive file 
    remove(archive_path); // Delete the original file 
    rename(temp_filename, archive_path); // Rename the temporary file 
}

    void extract_archive(const char* archive_path, const char* output_path) { 
    FILE* archive_file = fopen(archive_path, "rb"); 
    if (archive_file == NULL) { 
        printf("Error: cannot open archive\n"); 
        return; 
    } 
 
    char buffer[1024]; 
    char filename[256]; 
    while (fgets(buffer, sizeof(buffer), archive_file) != NULL) { 
        if (strncmp(buffer, "File: ", 6) == 0) { 
            // Extract the file name from the header 
            sscanf(buffer, "File: %s\n", filename); 
 
            if (verbose) { 
                printf("Extracting file: %s\n", filename); 
            } 
 
            // Create a path to save the file if output_path is specified 
            char full_path[512] = { 0 }; 
            if (output_path != NULL) { 
                sprintf(full_path, "%s/%s", output_path, filename); 
            } 
            else { 
                strcpy(full_path, filename); 
            } 
 
            // Open a file for writing 
            FILE* extracted_file = fopen(full_path, "wb"); 
            if (extracted_file == NULL) { 
                printf("Error: cannot create file %s\n", full_path); 
                continue; // Skip this file 
            } 
 
            // Read and write data from the archive to the file 
            size_t bytes_read; 
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), archive_file)) > 0) { 
                fwrite(buffer, 1, bytes_read, extracted_file); 
            } 
 
            fclose(extracted_file); 
        } 
    } 
 
    fclose(archive_file); 
}
