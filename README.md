# aprsfmt

Command-line utility for producing and parsing APRS packets.

## Overview

`aprsfmt` is a lightweight tool for constructing and formatting APRS (Automatic Packet Reporting System) packets. It provides an easy interface to generate valid AX.25 APRS packets with support for:

- **Position Reports**: Generate uncompressed position reports with latitude/longitude in decimal degrees
- **Flexible Addressing**: Specify source, destination, and path addresses in TNC2 format
- **Custom Symbols**: Choose from APRS symbol tables and symbols to represent different station types
- **Messaging Capability**: Mark stations as messaging-capable
- **Timestamps**: Include or exclude timestamps in position reports
- **Custom Info**: Append additional information or remarks to position reports

This makes it useful for testing APRS systems, generating position packets for automated stations, or integrating APRS packet generation into larger applications.

```bash
# Remove build directory
make clean

# Pull latest changes including submodules
make update

# Compile with debugging symbols (default)
make build

# Compile a clean binary for installation
make release

# Compile & run with sample arguments
make 
make run

# Compile & run tests
make test

# Compile in Release mode and install to system
make install
```
