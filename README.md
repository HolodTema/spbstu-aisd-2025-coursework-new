### СПБПУ, Программная Инженерия, 1 курс, Алгоритмы и структуры данных, курсовой проект.

### Shannon-Fano text encoding
This console application allows you encode or decode any text file on any language. (text file must be encoded via UTF-8)

You can choose one of two modes of encoding/decoding:
1. binary mode: text will be encoded as binary file, where every 0 or 1 character of code are bits inside binary file. This mode can compress text file size in 50-60%.

2. debug mode: encoded text (string of 0 and 1) will be written to the ordinary text file. This mode shows you how Shannon-Fano encoding works. There is no text file size compression in the debug mode.

### This program creates some files:
1. encodedText.txt - text file with encoded text (string of 0 and 1) in debug mode.
2. encodedText.bin - binary file with encoded text in binary mode.
3. key.txt - text file with encryption key with Shannon-Fano codes for certain encoded text/binary file. This file describes codes for every character.
4. decodedText.txt - text file with decoded text.

### The structure of the program:
1. ConsoleHelper.h - class with static methods to communicate with user via console. This class handles user way and calls methods of CodeHelper.h and FileHelper.h
2. FileHelper.h - class with static methods to read or write text/binary files. Also this class can parse encryption-key file key.txt
3. CodeHelper.h - class with static methods to encrypt or decrypt strings of text.
4. exceptions.h - some exceptions, which can be thrown while CodeHelper or FileHelper are working.
5. config.h - class with some constants, like program version or file names. 
