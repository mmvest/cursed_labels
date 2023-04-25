## About The Project

This project was more of a curiosity than anything substantial. Cursed Labels was born from a passing thought -- "can I pass a label as a signal handler function for a call to signal()?" Turns out you can!

Reasons to use "Cursed Labels":
* You really want to screw up your code
* Don't

This will wreck havoc on a real body of code and in it's current state has no real use. When your label is called by the signal handler, your stack and register context will likely be entirely different than what it needs to be to execute properly. If you try to access any local variables (essentially any variables that rely on offsets from rsp/rbp or any other register) then the application will likely crash or produce unexpected results. End goal will be to implement a way to set the context of the label properly so that it can access local variables without crashing.

## Building
Built on Ubuntu for WSL on Windows 11 using gcc. To build the proof of concept code, from the root of the project run the following:
```sh
mkdir build
gcc ./example/poc.c -o ./build/poc
```

## Usage

Using Cursed Labels is simple -- just include the header in your code as shown in the proof of concept and wrap any label you want to call as a function in the `LABLE_TO_FUNCTION()` macro.

To run the proof of concept executable, just run it like any other executable:
```sh
./build/poc
```

## Roadmap

- [X] Create Proof of concept
- [O] Add context setting (in progress)

## License

TBD