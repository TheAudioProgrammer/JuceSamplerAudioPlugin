## README

- Directory structure:
    - `sources` has the source files
    - `sounds` has the samples as WAV files

- Using CMake instead of Projucer
    - Using CPM rather than submodules to bring in JUCE.  It's much easier for beginners to use.
    - `juce_add_plugin` section: name and properties of the plug-in. 
        - `IS_SYNTH` must be TRUE
        - `NEEDS_MIDI_INPUT` must be TRUE
    - `target_sources` has the .h and .cpp files
        - It is possible to let CMake find your source files automatically but I like being explicit. It does mean you need to run `cmake` again whenever you add/remove/rename source files (but that's true for Projucer as well).
    - `juce_add_binary_data` has the sound files and images
    - Mac: `cmake -B build -G Xcode` (if you use Xcode)
        - For release builds, do `cmake -B build -G Xcode -D"CMAKE_OSX_ARCHITECTURES=arm64;x86_64"` to enable both Intel and ARM builds. Without this flag, it only builds for your own type of machine (which is faster for debug builds, which is why we don't put the `CMAKE_OSX_ARCHITECTURES` flag inside CMakeLists.txt).
    - Windows: `cmake -B build -G "Visual Studio 17 2022"`
    - Added the `**/build` folder to the `.gitignore` file so it doesn't get committed.
    - A few notable things inside the CMakeLists.txt file:
        - `set(CMAKE_MSVC_RUNTIME_LIBRARY ...`: Will link with the static C++ library rather than the dynamic one (on Windows).
        - `# generate PDB/dSYM files for the Release build`: This section enables the creation of PDB (on Windows) and dSYM (on Mac) files that allow us to symbolicate release build crash logs.
        - `set_property(GLOBAL PROPERTY USE_FOLDERS YES)` and `option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)`: These are optional and only change how the project will look in your IDE.
        - The `juce_add_binary_data()` section in the CMake file adds the WAV files to the plug-in.

- Samples:
    - We're adding the WAV files as binary data to the plug-in itself.
    - If you have many samples or really large ones, they cannot be added to binary data but must be loaded on-demand. This is a lot more complicated.
        - Note that Finder/Explorer shows `a5.wav` and before `c5.wav`, since it doesn't understand that these should be musical notes.
    - There is only one velocity per note.
    - When you make your own samples, make sure there is no silence at the beginning or end and that all samples are roughly equally loud (although we could fix uneven loudness in code).

- After running `cmake -B build ...` for the first time, if you open the project in your IDE there may be source files in red, notably `JuceHeader.h`, `BinaryData.h`, and `BinaryData1.cpp` and so on. That's because these files are automatically generated during the build process. After the first build, they will appear as normal and you can open them.

- Some notes about the `PluginProcessor` code:
    - This doesn't use the typical code from the JUCE templates like what you get with Projucer (since we're not using Projucer). I've simplified it a little.
    - Currently this is stereo only (see `isBusesLayoutSupported`). Mono wouldn't be too hard to add, since `juce::Synthesiser` supports it already.
