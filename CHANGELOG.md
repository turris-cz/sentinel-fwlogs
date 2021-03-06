# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.0] - 2022-03-10
### Added
- support for welcome message required for newer versions of sentinel-proxy
- dependency on libczmq\_logc that integrates CZMQ logging with LogC
- configuration parsing for `nflog_group`, `socket` and `topic`

### Changed
- minimal required version of LogC is now 0.2.0

### Fixed
- CI: release description for automatic release creation

## [0.2.0] - 2021-05-14
### Added
- support for parsing UDP and TCP protocols for IPv6 (for now without extensions
  headers)

### Changed
- source files were reorganized in the repository

## [0.1.0] - 2020-10-06
### Added
- Initial version of fwlogs
- IPv4 TCP and UCP support
- Minimal IPv6 support
