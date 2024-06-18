void decompressFile(const char* inputFileName, const char* outputFileName) {
    FILE* inputFile = fopen(inputFileName, "r");
    FILE* outputFile = fopen(outputFileName, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Ошибка открытия файлов\n");
        return;
    }

    char current;
    int count;

    while (fscanf(inputFile, "%c%d", &current, &count) != EOF) {
        for (int i = 0; i < count; i++) {
            fprintf(outputFile, "%c", current);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}
