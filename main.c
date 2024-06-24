#include "functions.h"

int verbose = 0;

int main(int argc, char* argv[]) {
    char* archive_path = NULL;
    char* output_path = NULL;
    char* files_to_archive = NULL;
    int verbose = 0;
    int compression_level = 0; // Compression is not used in this version of the archiver
    int operation = 0; // 0 - nothing, 1 - archive creation, 2 - archive unpacking, 3 - archive viewing, 4 - add, 5 - delete

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--create") == 0) {
            operation = 1;
        }
        else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--extract") == 0) {
            operation = 2;
        }
        else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
            operation = 3;
        }
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--add") == 0) {
            operation = 4;
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--delete") == 0) {
            operation = 5;
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            if (i + 1 < argc) {
                archive_path = argv[i + 1];
                i++;
            }
            else {
                printf("Error: Archive or file path not specified.\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) {
                output_path = argv[i + 1];
                i++;
            }
            else {
                printf("������: �� ������ ���� ��� ����������\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        }
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0) {
            if (i + 1 < argc) {
                if (strcmp(argv[i + 1], "low") == 0) {
                    compression_level = 1;
                }
                else if (strcmp(argv[i + 1], "high") == 0) {
                    compression_level = 2;
                }
                else {
                    printf("������: �������� ������� ������\n");
                    return 1;
                }
                i++;
            }
            else {
                printf("������: �� ������ ������� ������\n");
                return 1;
            }
        }
        else if (argv[i][0] == '-') {
            // Unknown parameter
            printf("������: ����������� ��������: %s\n", argv[i]);
            return 1;
        }
        else {
            // Files for archiving
            if (files_to_archive == NULL) {
                files_to_archive = argv[i];
            }
            else {
                files_to_archive = strcat(files_to_archive, " ");
                files_to_archive = strcat(files_to_archive, argv[i]);
            }
        }
    }

    if (verbose) {
        printf("���� � ������: %s\n", archive_path);
        printf("���� ��� ����������: %s\n", output_path);
        printf("����� ��� ���������: %s\n", files_to_archive);
        printf("������� ������: %d\n", compression_level);
    }

    switch (operation) {
    case 1:
        create_archive(archive_path, files_to_archive, compression_level); // compression_level is ignored
        break;
    case 2:
        extract_archive(archive_path, output_path);
        printf("���������� ������: %s\n", archive_path);
        break;
    case 3:
        list_archive(archive_path);
        break;
    case 4:
        add_files_to_archive(archive_path, files_to_archive, compression_level);
        printf("���������� ������ � �����: %s\n", archive_path);
        break;
    case 5:
        delete_files_from_archive(archive_path, files_to_archive);
        printf("�������� ������ �� ������: %s\n", archive_path);
        break;
    default:
        printf("������: �� ������� ��������\n");
        return 1;
    }

    return 0;
}
