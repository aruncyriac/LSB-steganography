# LSB Steganography

LSB Steganography is a technique for concealing sensitive data within an ordinary-looking object, making it appear unremarkable. Unlike cryptology, which encrypts messages to protect content, steganography aims to prevent detection of hidden information. By embedding data in a way that goes unnoticed, it effectively reduces the likelihood of unauthorized access or decryption.

This project implements LSB steganography for encoding and decoding data in `.bmp` image files.

## Usage

### Encoding
To encode secret data into an image, use the following command:
```sh
./a.out -e [source image] [secret data file] [destination file]
```
- `source image`: The BMP image in which data will be hidden.
- `secret data file`: The file containing the information to be embedded.
- `destination file`: The output BMP file with the hidden data.

### Decoding
To extract hidden data from an image, use:
```sh
./a.out -d [encoded image] [output file]
```
- `encoded image`: The BMP file containing hidden data.
- `output file`: The extracted secret data.

## Features
- Implements Least Significant Bit (LSB) encoding and decoding.
- Works with `.bmp` image files to hide and retrieve secret messages.
- Ensures minimal distortion to the cover image, making hidden data undetectable.
- Command-line interface for efficient processing.

## Applications
- Secure communication
- Digital watermarking
- Confidential data storage

