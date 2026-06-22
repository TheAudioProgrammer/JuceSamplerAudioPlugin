# JUCE Sampler Audio Plugin

A simple but functional sampler plugin built with JUCE 8.0.8, featuring chromatic sampling across multiple octaves with decay and reverb controls.

## Features

- 8 chromatic samples (C5 through C6) mapped across the full MIDI range
- Adjustable decay envelope
- Built-in reverb effect
- Custom GUI with filmstrip knobs
- VST3, AU, and Standalone formats

## Directory Structure

- `sources/` - Source files (.h and .cpp)
- `assets/` - WAV samples, PNG images, and TTF fonts
- `build/` - Generated build files (not in git)

## Build Instructions

### Prerequisites

- CMake 3.22 or higher
- C++20 compatible compiler
- Xcode (macOS) or Visual Studio 2022 (Windows)

### macOS

```bash
# Debug build (native architecture only)
cmake -B build -G Xcode

# Release build (universal binary: Intel + Apple Silicon)
cmake -B build -G Xcode -D"CMAKE_OSX_ARCHITECTURES=arm64;x86_64"

# Build from command line
cmake --build build --config Release
```

After building, plugins will be automatically copied to:
- AU: `~/Library/Audio/Plug-Ins/Components/`
- VST3: `~/Library/Audio/Plug-Ins/VST3/`
- Standalone: `build/Sampler_artefacts/`

### Windows

```bash
# Generate Visual Studio project
cmake -B build -G "Visual Studio 17 2022"

# Build from command line
cmake --build build --config Release
```

Open `build/Sampler.sln` in Visual Studio to build and debug.

### First Build Notes

- After running CMake for the first time, generated files like `JuceHeader.h` and `BinaryData.h` may appear in red in your IDE
- These files are created during the build process
- After the first successful build, they will be accessible
- If you add/remove/rename source files, you need to re-run CMake

## Parameters

### Decay (0-100%)

Controls the envelope decay time of the sampler voices.

- **Range:** 0% to 100%
- **Default:** 0%
- **Behavior:** Uses a non-linear (squared) curve to provide finer control at lower values
  - 0% = 0.05s decay time (very short)
  - 100% = 1.0s decay time (sustained)
- **Attack:** Fixed at 0.0s (instant)
- **Release:** Fixed at 0.05s

### Reverb (0-100%)

Controls the reverb room size.

- **Range:** 0% to 100%
- **Default:** 50%
- **Behavior:**
  - Room size scales linearly with percentage
  - Damping: Fixed at 0.5
  - Wet level: Fixed at 0.33
  - Dry level: Fixed at 0.4
- **Note:** Reverb is always active in the signal chain

## MIDI Note Mapping

The plugin uses 8 chromatic samples (C5-C6) distributed across the full MIDI range. Each sample covers multiple keys to create a chromatic scale:

| Sample | Root Note | MIDI Notes Covered |
|--------|-----------|-------------------|
| **C5** | 72 (C5) | 24, 25, 36, 37, 48, 49, 60, 61, 72, 73 (C1-C#1, C2-C#2, C3-C#3, C4-C#4, C5-C#5) |
| **D5** | 74 (D5) | 26, 27, 38, 39, 50, 51, 62, 63, 74, 75, 86, 87, 98, 99 (D1-D#1, D2-D#2, D3-D#3, D4-D#4, D5-D#5, D6-D#6, D7-D#7) |
| **E5** | 76 (E5) | 28, 40, 52, 64, 76, 88, 100 (E1, E2, E3, E4, E5, E6, E7) |
| **F5** | 77 (F5) | 29, 30, 41, 42, 53, 54, 65, 66, 77, 78, 89, 90, 101, 102 (F1-F#1, F2-F#2, F3-F#3, F4-F#4, F5-F#5, F6-F#6, F7-F#7) |
| **G5** | 79 (G5) | 31, 32, 43, 44, 55, 56, 67, 68, 79, 80, 91, 92, 103, 104 (G1-G#1, G2-G#2, G3-G#3, G4-G#4, G5-G#5, G6-G#6, G7-G#7) |
| **A5** | 81 (A5) | 21, 22, 33, 34, 45, 46, 57, 58, 69, 70, 81, 82, 93, 94, 105, 106 (A0-A#0, A1-A#1, A2-A#2, A3-A#3, A4-A#4, A5-A#5, A6-A#6, A7-A#7) |
| **B5** | 83 (B5) | 23, 35, 47, 59, 71, 83, 95, 107 (B0, B1, B2, B3, B4, B5, B6, B7) |
| **C6** | 84 (C6) | 84, 85, 96, 97, 108 (C6-C#6, C7-C#7, C8) |

**Note:** JUCE's sampler engine automatically pitch-shifts each sample to match the played note, using the root note as the reference pitch.

## Technical Details

### Audio Processing

- **Polyphony:** 8 voices
- **Voice Management:** Standard JUCE voice stealing when polyphony limit is reached
- **Stereo Processing:** Stereo output only (mono not currently supported)
- **Sample Format:** 16-bit WAV files included as binary data

### CMake Configuration

This project uses CMake instead of Projucer:

- **CPM:** Downloads JUCE automatically (no submodules needed)
- **JUCE Version:** 8.0.8
- **Important flags in CMakeLists.txt:**
  - `IS_SYNTH TRUE` - Marks plugin as an instrument
  - `NEEDS_MIDI_INPUT TRUE` - Enables MIDI input
  - `CMAKE_MSVC_RUNTIME_LIBRARY` - Links with static C++ library on Windows
  - PDB/dSYM generation enabled for release builds (allows crash log symbolication)
- **Binary Data:** WAV files, PNG images, and TTF fonts are embedded in the plugin binary via `juce_add_binary_data()`

### Assets

**Samples:**
- WAV files are embedded as binary data in the plugin executable
- For projects with many or very large samples, on-demand loading would be required (more complex)
- Current samples have no silence at start/end and are normalized to similar loudness
- Single velocity layer (no velocity sensitivity)
- Note: Finder/Explorer may sort `a5.wav` before `c5.wav` (alphabetical, not musical order)

**Images and Fonts:**
- Background image and Inter fonts are embedded as binary data
- Plugin is fully self-contained with no external file dependencies at runtime
- All artwork is rendered at 2x resolution:
  - Background: 1376×1032 pixels (displayed at 688×516)
  - Knob filmstrip: 152×132 pixels per frame, 129 frames total
  - 2x assets ensure sharpness on Retina/HiDPI displays
- Inter font family is licensed under SIL Open Font License (royalty-free distribution)

### Code Structure Notes

- Simplified `PluginProcessor` implementation (not using standard JUCE/Projucer template)
- Stereo-only output (see `isBusesLayoutSupported()`)
- Mono support could be added as `juce::Synthesiser` already supports it

## Development Notes

- The `**/build` folder is in `.gitignore` and won't be committed
- When adding/removing/renaming source files, update `target_sources` in `CMakeLists.txt` and re-run CMake
- Explicit file listing (vs. automatic discovery) provides better control and clarity

## License

Check individual asset licenses:
- Inter fonts: SIL Open Font License
- JUCE: GPL/Commercial (see JUCE licensing for distribution)
