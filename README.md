# LSB-steganography
LSB Steganography is indeed a technique for concealing sensitive data within an ordinary-looking object, making it  appear unremarkable to anyone who comes across it. Unlike cryptology, which focuses on encrypting messages to  protect their content, steganography aims to prevent detection of the information itself.By embedding data in 
a way that goes unnoticed, steganography effectively reduces the likelihood of unauthorized access or decryption,
as there is no indication that any hidden information exists. This subtlety is what distinguishes it from
traditional methods of securing data.Here we are encoding in .bmp file.

In this project we pass '-e' as command line argument for encoding and '-d' for decoding along with the other file.
For encoding the command line argument should be in './a.out -e [source image] [secret data file] [destination file]'.
For decoding the command line argument should be in './a.out -e [source image] [secret data file]
