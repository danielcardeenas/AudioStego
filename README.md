AudioStego
==========

Audio file steganography. Hides text or files inside audio files and retrieve them automatically.
It actually supports only .mp3 and .wav files as inputs, but you can hide whatever kind of file or text on them.

Build
==========
I recommend using [Cmake](http://www.cmake.org/install/) to build since it's easy and quick

1. `[AudioStego]$ mkdir build`
2. `[AudioStego]$ cd build`
3. `[AudioStego/build]$ cmake ..`
4. `[AudioStego/build]$ make ..`

And there's should be a file named "HideMeIn"

How to use
==========

### Hiding data
In order to hide a file inside another file

`$ ./HideMeIn [file_used_to_hide_data] [file_to_hide]`

Or if you want to hide a string on it

`$ ./HideMeIn [file_used_to_hide_data] "'Message to hide'"` (Note the simple quotation inside double quotes)

Both will output a clone of the file used to hide data in the same directory

### Retrieve hidden data
In order to retrieve a hidden message just set the "-f" or "--find" flag:

`$ ./HideMeIn [file_with_hidden_data] -f`

It will automatically detect wether it's a string or a file and the type of the file itself.
Will output the file / text hidden
