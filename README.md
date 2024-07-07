# LibQtLDAP: A Qt-based LDAP Library

## Overview

libqtldap is a C++ library that provides a Qt-based interface for managing LDAP objects using libdomain. 
It allows developers to easily interact with LDAP directories and perform various operations such as searching,
adding, modifying, and deleting LDAP objects.
Features
   - LDAP object management: Create, read, update, and delete LDAP objects using a simple and intuitive API.
   - Qt integration: Leverages the power of Qt to provide a robust implementation.
   - libdomain support: Utilizes libdomain to handle LDAP connections and operations.
   - Error handling: Provides detailed error messages and error codes for easy debugging.

## Getting Started

### Building and Installing

To build and install libqtldap, follow these steps:
   - Clone the repository: git clone https://github.com/august-alt/libqtldap.git
   - Build the library: ```mkdir build && cd build && cmake .. && make -j `nproc` ```
   - Install the library: `make install`

### Using libqtldap in Your Project

### API Documentation

The libqtldap API documentation is available in the doc directory.

### License

libqtldap is licensed under the GNU v2 license.

### Contributing

Contributions to libqtldap are welcome! If you'd like to report a bug, request a feature, or submit a patch, please use the GitHub issue tracker or pull requests.

### Acknowledgments

libqtldap is built on top of the excellent libdomain library, which provides a robust and efficient LDAP implementation.
We would like to thank the libdomain developers for their hard work and dedication.

