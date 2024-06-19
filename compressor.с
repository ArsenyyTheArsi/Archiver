// Функция для сжатия файла
void compressFile(const char* inputFileName, const char* outputFileName) {
    FILE* inputFile = fopen(inputFileName, "r");
    FILE* outputFile = fopen(outputFileName, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Ошибка открытия файлов\n");
        return;
    }

    char current, next;
    int count = 1;

    while ((current = fgetc(inputFile)) != EOF) {
        next = fgetc(inputFile);
        if (current == next) {
            count++;
        }
        else {
            fprintf(outputFile, "%c%d", current, count);
            count = 1;
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}
