# mbhttpd

HTTP server implementing a subset of HTTP 1.0

## Build

    $ make

## Invoke

    $ ./mbhttpd -p 8080
    $ ./mbhttpd # defaults: bind on lowest-numbered interface (INADDR_ANY) on 8080

Exit status:

- returns 1 if invocation error
- returns 2 if connection error
- never returns else

## Misc

Defaults MIME types:

- binaries : application/octet-stream
- ascii : text/plain

## Known limitations

- Request parser doesn't exist
- Can't choose the interface to bind on. (gethostbyname leaks on my libc)
- All request headers are ignored
- URL decoding not understood ( example: s/%7B/{/ )
- Can't open big files ( > 2 Gb )
- No logging
- Must be killed to stop serving

## Needs rewriting

- Request and headers parser
- clients table -> better structure (pre-allocated list or hashtable)
- error handling is a mess
- request_str and headers_str reallocation not efficient

## References

RFC:

- HTTP 1.0: http://www.w3.org/Protocols/rfc1945/rfc1945

MIME Types:

- http://www.iana.org/assignments/media-types/

## Tested with

- `valgrind -v --leak-check=full --show-reachable=yes ./mbhttpd -p 8080`
- `libefence`

# LICENSE

MIT

Copyright (c) 2010 Maxime Biais <firstname.lastname@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
