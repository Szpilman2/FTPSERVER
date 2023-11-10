


jsoncpp installation for Linux (Debian Based): ```sudo apt install libjson-cpp-dev```

how to compile?
``` g++ -std=c++17 FTPServer.cpp -o server -ljsoncpp ```
``` g++ -std=c++17 client.cpp -o client ```

# FTP SERVER

Simple FTP Server Implemented in C++

### Prerequisites

You need jsonCpp library for compiling this project. you can download jsonCpp with ```sudo apt install libjson-cpp-dev``` command. Actually this command only works in Debian based linux. I found similar commands for another distros.(it is easy to find if you search on the web.)

but if you are veteran you can go with instructions provided with jsonCpp official github repository.


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

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
