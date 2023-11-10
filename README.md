# FTP SERVER
FTP, or File Transfer Protocol, is a standard network protocol used to transfer files from one host to another over a TCP-based network, such as the internet or an intranet. An FTP server is a software application or service running on a server that provides the functionality for clients to connect and perform file-related operations like uploading, downloading, renaming, and deleting files.

FTP operates on a client-server model, where the client initiates a connection to the server to perform file transfer operations. The server manages access to files and directories, and it authenticates users who connect to it. FTP uses two separate channels for communication:

* Control Channel: This channel is used for sending commands from the client to the server and receiving responses. (implemented :white_check_mark:)

* Data Channel: This channel is used for the actual transfer of files. (implemented :white_check_mark:)

This project is a basic FTP server implemented in C++ that utilizes a JSON configuration file to configure both the server and client. Note that certain sections of this file have been omitted in my implementation, but customization can be easily performed if desired.

### Prerequisites

To compile this project, you require the jsonCpp library. You can obtain jsonCpp by using the command ```sudo apt install libjson-cpp-dev```. Please note that this command specifically applies to Debian-based Linux distributions. For other distributions, alternative commands can be found easily through online search.

For experienced users, you may refer to the instructions provided on the official jsonCpp GitHub repository [Link](https://github.com/open-source-parsers/jsoncpp).


## Getting Started
After installing the prerequisites, you can compile the code using the following commands.

To compile the server, use the command:
```
g++ -std=c++17 FTPServer.cpp -o server -ljsoncpp
```
To compile the client, use the command:
```
g++ -std=c++17 client.cpp -o client
```
Once the code is compiled, open two separate terminals. In one terminal, type ./server, and in the other, type ./client.

Upon execution, the program prompts you to enter your username (format: User user_name) and password (format: Pass your_pass). If the password is correct, you gain access to send commands to the server. All commands follow a similar format.

```
==========================================HELP==================================================
                you can enter following commands:
                    PWD  : prints working directory.
                    LS   : list all files in the current path.
                    MKD directory_name : creates directory_name in current path.
                    DELE [-d] [-f] dir/file_name : deletes directory or file in current path.
                    RENAME oldfile newfile : renames oldfile to newfile.
                    RETR file_name : downloads file_name from FTP server if the file exists in the current path. 
                    quit : for closing the program.

```


<!-- 
### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo -->

<!-- ## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project. -->

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/Szpilman2/FTPSERVER/blob/main/LICENSE) file for details

## Acknowledgments

* I employed ChatGPT (version 3.5) to rephrase this README file.
