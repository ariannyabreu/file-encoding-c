# File Encoder/Decoder AAG

## Summary

This project is a C program that provides functionality to encode and decode text files into a custom AAG file format. 
The AAG file format is designed with a specific structure that includes metadata about the text file, such as a checksum and text size, to ensure data integrity.

## Features

- **Encoding**: Converts a plain text file into an AAG file format.
  - The AAG file header includes:
    - The first two bytes: the characters 'A' and 'G'.
    - The next 4 bytes: an unsigned integer representing the text size.
    - The next 8 bytes: an unsigned long integer representing the checksum (sum of ASCII values of all characters).
    - The remaining bytes: the actual text data.
- **Decoding**: Converts an AAG file back into a plain text file.
  - Validates the file format by checking the header.
  - Verifies data integrity using the checksum.

## How to Use

1. **Compile the Program**
   - Open your terminal and navigate to the project directory.
   - Compile the code using `clang` or `make`:
     ```sh
     clang -o aag_encoder_decoder aag_encoder_decoder.c
     ```
     or
     ```sh
     make aag_encoder_decoder
     ```

2. **Run the Program**
   - Execute the compiled program:
     ```sh
     ./aag_encoder_decoder
     ```

3. **Follow the On-Screen Menu**
   - Select an option from the menu to encode or decode files.

## Acknowledgments

This project was initially developed as part of the CIS154 coursework by Professor Michael Penta at NECC, Haverhill, MA. It was further enhanced to improve memory allocation and deallocation best practices.

## Contact

For any questions or issues, please contact [ariannyag.dev@gmail.com](mailto:ariannyag.dev@gmail.com).
