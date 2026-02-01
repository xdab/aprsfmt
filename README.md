# aprsfmt
Command-line utility for producing and parsing APRS packets

## Building

This project uses CMake for building.

### Build Instructions

```bash
# Create and enter build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .
```

### Installation

```bash
# Install to system (default prefix: /usr/local)
sudo cmake --install .

# Or install to a specific prefix
cmake --install . --prefix /usr
```
