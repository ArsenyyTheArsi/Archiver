#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int verbose;

void add_files_to_archive(const char* archive_path, const char* files_to_archive, int compression_level);
void create_archive(const char* archive_path, const char* files_to_archive, int compression_level);
void delete_files_from_archive(const char* archive_path, const char* files_to_archive);
void extract_archive(const char* archive_path, const char* output_path);
void list_archive(const char* archive_path);
