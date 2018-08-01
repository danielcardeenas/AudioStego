AudioStego
==========

Audio file steganography. Hides text or files inside audio files and retrieve them automatically.
It actually supports only .mp3 and .wav files as inputs, but you can hide whatever kind of file or text on them.

[Here](https://danielcardeenas.github.io/audiostego.html) is a post about this software.

Build
==========
This shit needs [CMake](http://www.cmake.org/install/) to build. So download it before compiling. 

```sh
> sudo apt-get install libboost-all-dev
> git clone https://github.com/danielcardeenas/AudioStego.git
> cd AudioStego
> mkdir build
> cd build
> cmake ..
> make
```
And it should create be a file named `hideme`

**Note: Boost libraries need to be installed to compile**

How to use
==========

### Hiding data
In order to hide a file inside another file

```sh
> ./hideme file_used_to_hide_data file_to_hide
```

Or if you want to hide a string on it

```sh
> ./hideme file_used_to_hide_data "'Message to hide'"
``` 
**(Notice the simple quotation inside double quotes)**

Both will output a clone of the file with the hidden data as `output.(file extension)`

### Retrieving hidden data
In order to retrieve a hidden message just set the "-f" or "--find" flag:

```sh
> ./hideme [file_with_hidden_data] -f
```

It will automatically detect wether it's a string or a file and the type of the file itself.
+ If it's a file, it will create it as `output.(file extension)`
+ If it's a string message, it will print it on the console/terminal
