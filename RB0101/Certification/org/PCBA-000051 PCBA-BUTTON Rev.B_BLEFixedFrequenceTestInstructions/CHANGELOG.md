# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-11-21

### Added
- Initial documentation (README.md, QUICKSTART.md).
- Changelog file in `doc/` directory.

### Changed
- Switched console output from UART to SEGGER RTT.
- Disabled UART initialization in `prj.conf` and `app.overlay`.
- Updated project configuration to support RTT shell backend.
