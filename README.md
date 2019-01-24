# CRC Calculator

Simple Qt application for calculating the CRC value of a text.
The interesting part of this application is actually **crcutils.h**.
It includes a templated *constexpr* implementation of the CRC algorithm
using a recoprocal table that is generated at compile time.
Two default polynomial implementation are provided trhough *typedef*, the **ANSI CRC32**, and the **ECMA CRC64**.
