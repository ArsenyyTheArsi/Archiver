#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct for Huffman nodes
typedef struct HuffmanNode {
    unsigned char symbol;
    int count;
    float frequency;
    char code[257];
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

// Define a union to represent Huffman codes in bytes
typedef union HuffmanCode {
    unsigned char byteValue;
    struct {
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
        unsigned bit8:1;
    } bits;
} HuffmanCode;

// Build the Huffman Tree using an array of nodes
HuffmanNode *buildHuffmanTree(HuffmanNode *nodes[], int nodeCount) {
    // Create a new node and calculate the frequency
    HuffmanNode *temp = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    temp->frequency = nodes[nodeCount - 1]->frequency + nodes[nodeCount - 2]->frequency;
    temp->code[0] = 0;
    temp->left = nodes[nodeCount - 1];
    temp->right = nodes[nodeCount - 2];

    // If only two nodes left, return the new node as root
    if (nodeCount == 2) {
        return temp;
    } else {
        // Insert the new node in the correct position
        for (int i = 0; i < nodeCount; i++) {
            if (temp->frequency > nodes[i]->frequency) {
                for (int j = nodeCount - 1; j > i; j--) {
                    nodes[j] = nodes[j - 1];
                }
                nodes[i] = temp;
                break;
            }
        }
    }
    // Recursive call to continue building the tree
    return buildHuffmanTree(nodes, nodeCount - 1);
}

// Generate Huffman codes for each symbol in the tree
void generateHuffmanCodes(HuffmanNode *root) {
    if (root->left) {
        strcpy(root->left->code, root->code);
        strcat(root->left->code, "0");
        generateHuffmanCodes(root->left);
    }
    if (root->right) {
        strcpy(root->right->code, root->code);
        strcat(root->right->code, "1");
        generateHuffmanCodes(root->right);
    }
}

// Encode the input file using the Huffman codes and write to the compressed file
int encodeHuffman(FILE *inputFile, FILE *compressedFile) {
    FILE *tempFile;
    int symbol;
    int uniqueSymbolsCount = 0;
    int totalSymbolsCount = 0;
    int tempFileSize = 0;
    int tailSize;
    int symbolFrequency[257] = {0};
    HuffmanNode symbols[257] = {0};
    HuffmanNode *symbolPointers[257];
    int bitArray[8];
    int i, j = 0, byteCount = 0;
    unsigned char *buffer = NULL;
    HuffmanNode *root;

    // Initialize the symbols array
    for (i = 0; i < 257; i++) {
        symbols[i].symbol = 0;
        symbols[i].count = 0;
    }

    // Count the frequency of each symbol in the input file
    while ((symbol = fgetc(inputFile)) != EOF) {
        for (i = 0; i < 257; i++) {
            if ((unsigned char)symbol == symbols[i].symbol) {
                if (((unsigned char)symbol == 0) && (uniqueSymbolsCount == 0)) {
                    symbols[i].count = 1;
                    uniqueSymbolsCount++;
                }
                symbolFrequency[i]++;
                totalSymbolsCount++;
                break;
            }
            if (symbols[i].count == 0) {
                symbols[i].symbol = (unsigned char)symbol;
                symbols[i].count = 1;
                symbolFrequency[i] = 1;
                uniqueSymbolsCount++;
                totalSymbolsCount++;
                break;
            }
        }
    }

    // Calculate the frequency of each symbol
    for (i = 0; i <= uniqueSymbolsCount; i++) {
        symbols[i].frequency = (float)symbolFrequency[i] / totalSymbolsCount;
    }

    // Create an array of pointers to the symbols
    for (i = 0; i <= uniqueSymbolsCount; i++) {
        symbolPointers[i] = &symbols[i];
    }

    // Sort the symbols by frequency in descending order
    HuffmanNode temp;
    for (i = 0; i <= uniqueSymbolsCount; i++) {
        for (j = 0; j < uniqueSymbolsCount - 1; j++) {
            if (symbols[j].frequency < symbols[j + 1].frequency) {
                temp = symbols[j];
                symbols[j] = symbols[j + 1];
                symbols[j + 1] = temp;
            }
        }
    }

    // Build the Huffman tree
    root = buildHuffmanTree(symbolPointers, uniqueSymbolsCount + 1);
    generateHuffmanCodes(root);

    // Rewind the input file to the beginning
    rewind(inputFile);

    // Create a temporary file to store the encoded data
    tempFile = fopen("temp.txt", "wb");
    while ((symbol = fgetc(inputFile)) != EOF) {
        for (i = 0; i <= uniqueSymbolsCount; i++) {
            if ((unsigned char)symbol == symbols[i].symbol) {
                fputs(symbols[i].code, tempFile);
            }
        }
    }
    fclose(tempFile);

    // Calculate the size of the temporary file
    tempFile = fopen("temp.txt", "rb");
    while ((symbol = fgetc(tempFile)) != EOF) {
        tempFileSize++;
    }

    // Calculate the tail size (remaining bits after full bytes)
    tailSize = tempFileSize % 8;

    // Write the header to the compressed file
    fwrite(&uniqueSymbolsCount, sizeof(int), 1, compressedFile);
    fwrite(&tailSize, sizeof(int), 1, compressedFile);

    // Write the symbol table to the compressed file
    for (i = 0; i <= uniqueSymbolsCount; i++) {
        fwrite(&symbols[i].symbol, sizeof(unsigned char), 1, compressedFile);
        fwrite(&symbols[i].frequency, sizeof(float), 1, compressedFile);
    }

    // Rewind the temporary file to the beginning
    rewind(tempFile);
    HuffmanCode huffmanCode;

    // Read the bits from the temporary file and convert to bytes
    j = 0;
    for (i = 0; i < tempFileSize - tailSize; i++) {
        bitArray[j] = fgetc(tempFile);
        if (j == 7) {
            huffmanCode.bits.bit1 = bitArray[0] - '0';
            huffmanCode.bits.bit2 = bitArray[1] - '0';
            huffmanCode.bits.bit3 = bitArray[2] - '0';
            huffmanCode.bits.bit4 = bitArray[3] - '0';
            huffmanCode.bits.bit5 = bitArray[4] - '0';
            huffmanCode.bits.bit6 = bitArray[5] - '0';
            huffmanCode.bits.bit7 = bitArray[6] - '0';
            huffmanCode.bits.bit8 = bitArray[7] - '0';

            byteCount++;
            buffer = (unsigned char *)realloc(buffer, byteCount * sizeof(unsigned char));
            buffer[byteCount - 1] = huffmanCode.byteValue;

            j = 0;
            continue;
        }
        j++;
    }

    // Process the remaining bits (tail)
    for (i = 0; i < 8; i++) {
        bitArray[i] = 0;
    }

    j = 0;
    for (i = 0; i <= tailSize; i++) {
        bitArray[j] = fgetc(tempFile);
        if (j == tailSize) {
            huffmanCode.bits.bit1 = bitArray[0] - '0';
            huffmanCode.bits.bit2 = bitArray[1] - '0';
            huffmanCode.bits.bit3 = bitArray[2] - '0';
            huffmanCode.bits.bit4 = bitArray[3] - '0';
            huffmanCode.bits.bit5 = bitArray[4] - '0';
            huffmanCode.bits.bit6 = bitArray[5] - '0';
            huffmanCode.bits.bit7 = bitArray[6] - '0';
            huffmanCode.bits.bit8 = bitArray[7] - '0';

            byteCount++;
            buffer = (unsigned char *)realloc(buffer, byteCount * sizeof(unsigned char));
            buffer[byteCount - 1] = huffmanCode.byteValue;

            j = 0;
            continue;
        }
        j++;
    }
    fclose(tempFile);
    remove("temp.txt"); // Remove the temporary file

    // Write the encoded data to the compressed file
    fwrite(&byteCount, sizeof(int), 1, compressedFile);
    fwrite(buffer, sizeof(unsigned char), byteCount, compressedFile);
    free(buffer);

    return byteCount;
}

// Decode the compressed file and write the output to the output file
int decodeHuffman(FILE *compressedFile, FILE *outputFile, long long int originalFileSize) {
    int uniqueSymbolsCount;
    int tailSize;
    int compressedSize;
    int i, j;
    int byteRead;
    unsigned char currentBit;
    unsigned char decodedSymbol;
    char bitArray[8];
    char bitBuffer[257] = {0};
    FILE *tempFile;
    HuffmanNode symbols[257] = {0};
    HuffmanCode huffmanCode;

    // Read the header from the compressed file
    fread(&uniqueSymbolsCount, sizeof(int), 1, compressedFile);
    fread(&tailSize, sizeof(int), 1, compressedFile);

    // Read the symbol table from the compressed file
    for (i = 0; i <= uniqueSymbolsCount; i++) {
        fread(&symbols[i].symbol, sizeof(unsigned char), 1, compressedFile);
        fread(&symbols[i].frequency, sizeof(float), 1, compressedFile);
    }

    // Read the size of the compressed data
    fread(&compressedSize, sizeof(int), 1, compressedFile);

    // Create a temporary file to store the decoded bit stream
    tempFile = fopen("temp.txt", "wb");

    j = 0;
    while (j != compressedSize) {
        byteRead = fgetc(compressedFile);
        currentBit = (unsigned char)byteRead;

        huffmanCode.byteValue = currentBit;

        // Extract individual bits from the byte and store in the bit array
        bitArray[0] = huffmanCode.bits.bit1 + '0';
        bitArray[1] = huffmanCode.bits.bit2 + '0';
        bitArray[2] = huffmanCode.bits.bit3 + '0';
        bitArray[3] = huffmanCode.bits.bit4 + '0';
        bitArray[4] = huffmanCode.bits.bit5 + '0';
        bitArray[5] = huffmanCode.bits.bit6 + '0';
        bitArray[6] = huffmanCode.bits.bit7 + '0';
        bitArray[7] = huffmanCode.bits.bit8 + '0';

        // Write the bit array to the temporary file
        fwrite(bitArray, sizeof(unsigned char), 8, tempFile);
        for (i = 0; i < 8; i++) {
            bitArray[i] = 0;
        }
        j++;
    }

    fclose(tempFile);

    // Reset the bit buffer
    for (i = 0; i < 257; i++) {
        bitBuffer[i] = 0;
    }

    compressedSize = 0;
    tempFile = fopen("temp.txt", "rb");

    // Read the bits from the temporary file and decode the symbols
    while (compressedSize != originalFileSize) {
        byteRead = fgetc(tempFile);
        currentBit = byteRead;
        strncat(bitBuffer, &currentBit, 1);

        // Match the bit buffer with the Huffman codes to decode symbols
        for (i = 0; i <= uniqueSymbolsCount; i++) {
            if (!strcmp(bitBuffer, symbols[i].code)) {
                decodedSymbol = (unsigned char)symbols[i].symbol;
                fwrite(&decodedSymbol, sizeof(unsigned char), 1, outputFile);
                compressedSize++;
                memset(bitBuffer, 0, strlen(bitBuffer));
                break;
            }
        }
    }

    fclose(tempFile);
    fclose(outputFile);
    remove("temp.txt"); // Remove the temporary file

    return 0;
}


