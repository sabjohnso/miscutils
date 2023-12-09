MiscUtils
============

## fail
Fail by returning an exit code of 1 for any input arguments.

## succeed
Succeed by returning an exit code of 0 for any input arguments.

## throttle
Transfer data from the input to the output with a limitted rate. The
rate is provided in bytes/second.

### options

--input <named-input>

--output <named-ouput>

--rate <bytes-per-second>

### examples

``` bash
source-program | throttle -rate 10 | ouput-program
```

```bash
source-program | throttle --output named-output-pipe
```

```bash
throttle --input named-input-pipe -output named-output-pipe
```
