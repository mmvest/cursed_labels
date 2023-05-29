## Cursed Labels

The label functionality you didn't know you don't need -- use labels as signal handlers or functions whether inside or outside the context in which the label is defined. 

Reasons to use "Cursed Labels":
* Cleanup of local variables on interrupt by using a label as a signal handler. No more need to define all those variables globally or create a seperate cleanup function -- just use a cursed label!
* Ever wanted to call a label like a function? Well now you can!
* Don't

Cursed Labels works by using C voodoo to cast the label to a function. This wizardry is aided by our `CURSED_LABEL` macro that will handle creating the label, finding the correct stack context for the label, and setting the context appropriately so that the label can run without your application blowing up.

## How to Use

The macro sits inside of a single header, `cursed_label.h`. Simply include that header in whatever project you want to use the labels in. Wherever you would normally create a label, you instead will use the macro as follows:
```C++
int main()
{
    int ret_val = 0;
    ...; // Do stuff here

    CURSED_LABEL(main, label_name)
        ... // Put all your label code here
        exit(ret_val);
}
```
As you can see, you use the macro in place of where you would put your label but you also give it the name of the function in which the label is located and the name of the label.

To use the cursed label as a function call or signal handler, use the `LABEL_TO_FUNCTION` macro like so:
```c++
signal(SIGINT, LABEL_TO_FUNCTION(label_name));
```
This macro takes care of the cursed function casting that is used to make this work.

Note that `CURSED_LABEL` relies on frame pointers existing on the stack, so `fno-omit-frame-pointer` option may need to be passed to `gcc` to get this to work... but no guarantee.

## Building
Building? Why would you do that? Its just a header.

## PoC Usage

To run the proof of concept executable, first build it:
```sh
gcc full_poc.c -o full_poc
```

Then, run it like any other executable:
```sh
./full_poc
```
Note that testing and building was done using `gcc` on an Ubuntu distro.

## License

TBD