AudioStego
==========

Audio file steganography. Hides text or files inside audio files and retrieve them automatically.
It actually supports only .mp3 and .wav files as inputs, but you can hide whatever kind of file or text on them.

Build
==========
I recommend using [Cmake](http://www.cmake.org/install/) to build since its easy and quick

1. `[AudioStego]$ mkdir build`
2. `[AudioStego]$ cd build`
3. `[AudioStego/build]$ cmake ..`

How to use
==========
In order to hide a file inside another file
`$ ./HideMeIn [file_used_to_hide_data] [file_to_hide]`

Or if you want to hide a string on it
`$ ./HideMeIn [file_used_to_hide_data] "'Message to hide'"` (Note the simple quotation inside double quotes)

In order to retrieve a hidden message just specified the "-f" or "--find" flag:
`$ ./HideMeIn [file_with_hidden_data]`

It will automatically detect wether it's a string or a file and the type of the file itself.
