# gemini

### Project doc files
 
        1. https://docs.google.com/document/d/15Z3HEfb52j08BZjDZbre4yTWI0OxDA_hIu4TIBL6bL8/edit
        2. https://docs.google.com/document/d/1-qPuVC6oiqT9DOCM4KgcPJmV6gPHqqHyjM2e_G4T89A/edit

#### Installcation of protobuf and grpc and other packages.

        1. Installation grpc C++ library of simple. Just follow the readme of grpc C++ installation.
          https://github.com/grpc/grpc/blob/master/BUILDING.md
        2. sudo apt-get install libssl-dev
        3. sudo apt-get install cmake

#### Build Instruction
        1. cd protocol
        2. make clean && make
        3. cd ..
        4. cmake .
        5. make
        6. ./build/bin/chord --local-ip 127.0.0.1 --local-port 6767
        (6th command is to run a single node only)
