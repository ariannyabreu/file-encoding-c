/************************
Arianny Abreu Gonzalez

C Programming Project - File Encoding

Description: This program is called File Encoding. It provides an option to encode a text file into a custom AAG file format
and to decode an AAG file back into a text format. The AAG files have the extension of .aag. The AAG file header
is organized as follows: The first two bytes (two char) of the file are the letters A and G. The next 4 bytes
(one unsigned int) represent the number of characters (including white spaces) that is in the file. The next 8 bytes
(one unsigned long int) represent the checksum. This is the sum of the ASCII integer values for all characters in the
file (including white spaces). This number can be used to check if the file was read correctly. The remaining
file data is the actual text of the file.

Date: April 20, 2023
**************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold the AAG file data.
struct aagFile {
    char first;
    char second;
    unsigned int textSize;
    unsigned long int checksum;
    char* text;
};

// Function prototypes.
void encode(void);
void decode(void);
char* getFileName(void);

int main(void)
{
    int choice;
    do
    {
        // Menu options.
        printf("Select an option:\n");
        printf("1. Encode a text file to AAG format\n");
        printf("2. Decode an AAG file to text format\n");
        printf("3. Exit\n");

        // Get user's choice.
        scanf("%d", &choice);
        // Consume the newline character left by scanf.
        getchar();

        // Perform an action based on user's choice.
        switch (choice)
        {
            case 1:
                encode();
                break;
            case 2:
                decode();
                break;
            case 3:
                printf("Thank you. Goodbye.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    while (choice != 3); // Repeat until user chooses to exit.

    return 0;
}

// Function to get the file name from user.
char* getFileName(void) {
    char temp [256]; // Temporary buffer to hold the file name.
    printf("Enter file name: ");
    fgets(temp, sizeof(temp), stdin);

    // Remove the newline character if present.
    temp[strcspn(temp, "\n")] = '\0';

    // Allocate memory for the file name.
    char* fileName = (char*) malloc(strlen(temp) + 1);
    if (fileName == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(fileName, temp); // Copy the input file name to allocated memory.
    return fileName;
}

// This function encodes text file into an AAG file.
void encode(void)
{
    char* fileAri = getFileName(); // Get the output file name from the user.

    printf("Enter text to encode: ");
    char text[1000]; // Buffer to hold the input text.
    fgets(text, sizeof(text), stdin);

    unsigned int textSize = strlen(text); // Calculate the size of the text.
    if (text[textSize - 1] == '\n') {
        text[textSize - 1] = '\0'; // Remove newline character from text input.
        textSize--;
    }

    // Calculate the checksum of the text.
    unsigned long int checksum = 0;
    for (int i = 0; i < textSize; i++)
    {
        checksum += text[i];
    }

    // Initialize the aagFile structure.
    struct aagFile file;
    file.first = 'A';
    file.second = 'G';
    file.textSize = textSize;
    file.checksum = checksum;
    file.text = (char*)malloc(sizeof(char) * (textSize + 1));

    if (file.text == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(fileAri);
        exit(EXIT_FAILURE);
    }
    strcpy(file.text, text); // Copy the text to the structure.

    FILE* fp = fopen(fileAri, "wb");  // Open file for writing in binary mode.

    if(fp == NULL) {
        fprintf(stderr, "Cannot open file %s\n", fileAri);
        free(file.text);
        free(fileAri);
        return;
    }

    // Write the structure data to the file.
    fwrite(&file.first, sizeof(char), 1, fp);
    fwrite(&file.second, sizeof(char), 1, fp);
    fwrite(&file.textSize, sizeof(unsigned int), 1, fp);
    fwrite(&file.checksum, sizeof(unsigned long int), 1, fp);
    fwrite(file.text, sizeof(char), textSize, fp);

    fclose(fp); // Close the file.
    free(file.text); // Free the allocated memory for text.
    free(fileAri); // Free the allocated memory for file name.

    printf("File encoded successfully.\n");
}

// This function decodes AAG file into a text file.
void decode(void)
{
    int attempts = 3;  // Limits the number of attempts to enter a valid file name to prevent segmentation fault.

    while (attempts > 0)
    {
        // Get the input file name from the user.
        char* fileAri = getFileName();

        // Open file for reading in binary mode.
        FILE* fp = fopen(fileAri, "rb");
        if (fp == NULL)
        {
            fprintf(stderr, "Cannot open file %s. Please try again.\n", fileAri);
            free(fileAri); // Free the allocated memory for file name.
            attempts--;
            continue;
        }

        // Read the structure data from the file.
        struct aagFile file;
        fread(&file.first, sizeof(char), 1, fp);
        fread(&file.second, sizeof(char), 1, fp);
        fread(&file.textSize, sizeof(unsigned int), 1, fp);
        fread(&file.checksum, sizeof(unsigned long int), 1, fp);

        // Check if the file format is correct.
        if (file.first != 'A' || file.second != 'G')
        {
            printf("Invalid file format. The file is not an AAG file.\n");
            fclose(fp);
            free(fileAri); // Free the allocated memory for file name.
            attempts--;
            continue;
        }

        // Allocate memory for the text.
        file.text = (char*)malloc(sizeof(char) * (file.textSize + 1));
        if (file.text == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(fp);
            free(fileAri);
            exit(EXIT_FAILURE);
        }
        fread(file.text, sizeof(char), file.textSize, fp);
        file.text[file.textSize] = '\0'; // Null-terminate the string.

        // Print the decoded text.
        printf("Decoded text: %s\n", file.text);

        free(file.text); // Free the allocated memory for text.
        fclose(fp); // Close the file.
        free(fileAri); // Free the allocated memory for file name.
        return;
    }

    printf("Failed to decode file after %d attempts.\n", 3);
}
