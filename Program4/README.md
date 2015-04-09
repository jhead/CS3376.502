# SE3376-Program4
Tokenizes and parses full mailing addresses, then outputs information in XML.

## Usage
```
  ./scanner < input-file
  
  Runs in scanner mode. This mode simply tokenizes input from stdin and displays token info using Flex.
  
  
  ./parser < input-file
  
  Runs in parser mode. This mode uses Bison and Flex to parse input from stdin, display parsing
  information via stdout, and output relevant XML via stderr.
```

## Installation
Run `make`. Done.

**Dependencies**
- make
- bison
- flex
- g++

## Why?
I have no idea. Maybe there's some hidden value here. All I know is that my hatred for C++ continues to grow.
