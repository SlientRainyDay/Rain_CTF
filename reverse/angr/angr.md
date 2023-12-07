# angr

angr是一个多架构的二进制分析平台，具备对二进制文件的动态符号执行能力和多种静态分析能力。在近几年的 CTF 中也大有用途。

## 安装

在 Ubuntu 上，首先我们应该安装所有的编译所需要的依赖环境：

```mipsasm
$ sudo apt install python-dev libffi-dev build-essential virtualenvwrapper
```

强烈建议在虚拟环境中安装 angr，因为有几个 angr 的依赖（比如z3）是从他们的原始库中 fork 而来，如果你已经安装了 z3,那么你肯定不希望angr 的依赖覆盖掉官方的共享库，开一个隔离的环境就好了：

```crystal
$ mkvirtualenv angr
$ sudo pip install angr
```

如果这样安装失败的话，那么你可以按照下面的顺序从 angr 的官方仓库安装：

```markdown
1. claripy
2. archinfo
3. pyvex
4. cle
5. angr
```

如：

```shell
$ git clone https://github.com/angr/claripy
$ cd claripy
$ sudo pip install -r requirements.txt
$ sudo python setup.py build
$ sudo python setup.py install
```

其他几个库也是一样的。

安装过程中可能会有一些奇怪的错误，可以到官方文档中查看。

## 使用 angr

#### 入门

使用 angr 的第一步是新建一个工程，几乎所有的操作都是围绕这个工程展开的：

```python
>>> import angr
>>> proj = angr.Project('/bin/true')
WARNING | 2017-12-08 10:46:58,836 | cle.loader | The main binary is a position-independent executable. It is being loaded with a base address of 0x400000.
```

这样就得到了二进制文件的各种信息，如：

```python-repl
>>> proj.filename     # 文件名
'/bin/true'
>>> proj.arch         # 一个 archinfo.Arch 对象
<Arch AMD64 (LE)>
>>> hex(proj.entry)   # 入口点
'0x401370'
```

程序加载时会将二进制文件和共享库映射到虚拟地址中，CLE 模块就是用来处理这些东西的。

```dns
>>> proj.loader
<Loaded true, maps [0x400000:0x5008000]>
```

所有对象文件如下，其中二进制文件是 main object：

```stylus
>>> for obj in proj.loader.all_objects:
...     print obj
...
<ELF Object true, maps [0x400000:0x60721f]>
<ELF Object libc-2.27.so, maps [0x1000000:0x13bb98f]>
<ELF Object ld-2.27.so, maps [0x2000000:0x22260f7]>
<ELFTLSObject Object cle##tls, maps [0x3000000:0x300d010]>
<ExternObject Object cle##externs, maps [0x4000000:0x4008000]>
<KernelObject Object cle##kernel, maps [0x5000000:0x5008000]>
>>> proj.loader.main_object
<ELF Object true, maps [0x400000:0x60721f]>
>>> hex(proj.loader.main_object.min_addr)
'0x400000'
>>> hex(proj.loader.main_object.max_addr)
'0x60721f'
>>> proj.loader.main_object.execstack
False
```

通常我们在创建工程时选择关闭 `auto_load_libs` 以避免 angr 加载共享库：

```pgsql
>>> p = angr.Project('/bin/true', auto_load_libs=False)
WARNING | 2017-12-08 11:09:28,629 | cle.loader | The main binary is a position-independent executable. It is being loaded with a base address of 0x400000.
>>> p.loader.all_objects
[<ELF Object true, maps [0x400000:0x60721f]>, <ExternObject Object cle##externs, maps [0x1000000:0x1008000]>, <KernelObject Object cle##kernel, maps [0x2000000:0x2008000]>, <ELFTLSObject Object cle##tls, maps [0x3000000:0x300d010]>]
```

`project.factory` 提供了很多类对二进制文件进行分析，它提供了几个方便的构造函数。

`project.factory.block()` 用于从给定地址解析一个 basic block：

```x86asm
>>> block = proj.factory.block(proj.entry)    # 从程序头开始解析一个 basic block
>>> block
<Block for 0x401370, 42 bytes>
>>> block.pp()                  # 打印
0x401370:       xor     ebp, ebp
0x401372:       mov     r9, rdx
0x401375:       pop     rsi
0x401376:       mov     rdx, rsp
0x401379:       and     rsp, 0xfffffffffffffff0
0x40137d:       push    rax
0x40137e:       push    rsp
0x40137f:       lea     r8, qword ptr [rip + 0x32da]
0x401386:       lea     rcx, qword ptr [rip + 0x3263]
0x40138d:       lea     rdi, qword ptr [rip - 0xe4]
0x401394:       call    qword ptr [rip + 0x205b76]
>>> block.instructions          # 指令数量
11
>>> block.instruction_addrs     # 指令地址
[4199280L, 4199282L, 4199285L, 4199286L, 4199289L, 4199293L, 4199294L, 4199295L, 4199302L, 4199309L, 4199316L]
```

另外，还可以将 block 对象转换成其他形式：

```python-repl
>>> block.capstone
<CapstoneBlock for 0x401370>
>>> block.capstone.pp()
>>> block.vex
IRSB <0x2a bytes, 11 ins., <Arch AMD64 (LE)>> at 0x401370
>>> block.vex.pp()
```

程序的执行需要初始化一个 `SimState` 对象：

```pf
>>> state = proj.factory.entry_state()
>>> state
<SimState @ 0x401370>
```

该对象包含了程序的内存、寄存器、文件系统数据等模拟运行时动态变化的数据，例如：

```pf
>>> state.regs                          # 寄存器名对象
<angr.state_plugins.view.SimRegNameView object at 0x7f126fdfe810>
>>> state.regs.rip                      # BV64 对象
<BV64 0x401370>
>>> state.regs.rsp
<BV64 0x7fffffffffeff98>
>>> state.regs.rsp.length               # BV 对象都有 .length 属性
64
>>> state.regs.rdi
<BV64 reg_48_0_64{UNINITIALIZED}>       # BV64 对象，符号变量
>>> state.mem[proj.entry].int.resolved  # 将入口点的内存解释为 C 语言的 int 类型
<BV32 0x8949ed31>
```

这里的 BV，即 bitvectors，用于表示 angr 里的 CPU 数据。下面是 python int 和 bitvectors 之间的转换：

```pf
>>> bv = state.solver.BVV(0x1234, 32)   # 创建值 0x1234 的 BV32 对象
>>> bv
<BV32 0x1234>
>>> hex(state.solver.eval(bv))          # 将 BV32 对象转换为 Python int
'0x1234'
>>> bv = state.solver.BVV(0x1234, 64)
>>> bv
<BV64 0x1234>
>>> hex(state.solver.eval(bv))
'0x1234L'
```

于是 bitvectors 可以进行数学运算：

```pf
>>> one = state.solver.BVV(1, 64)
>>> one_hundred = state.solver.BVV(100, 64)
>>> one_hundred + one                 # 位数相同时可以直接运算
<BV64 0x65>
>>> one_hundred + one + 0x100
<BV64 0x165>
>>> state.solver.BVV(-1, 64)          # 默认为无符号数
<BV64 0xffffffffffffffff>
>>> five = state.solver.BVV(5, 27)
>>> five
<BV27 0x5>
>>> one + five.zero_extend(64 - 27)   # 位数不同时需要进行扩展
<BV64 0x6>
>>> one + five.sign_extend(64 - 27)   # 或者有符号扩展
<BV64 0x6>
```

使用 bitvectors 来设置寄存器和内存的值，当直接传入 python int 时，angr 会自动将其转换成 bitvectors：

```pf
>>> state.regs.rsi = state.solver.BVV(3, 64)
>>> state.regs.rsi
<BV64 0x3>
>>> state.mem[0x1000].long = 4          # 在地址 0x1000 存放一个 long 类型的值 4
>>> state.mem[0x1000].long.resolved     # .resolved 获取 bitvectors
<BV64 0x4>
>>> state.mem[0x1000].long.concrete     # .concrete 获得 Python int
4L
```

初始化的 state 可以经过模拟执行得到一系列的 states，simulation 管理器的作用就是对这些 states 进行管理：

```pf
>>> simgr = proj.factory.simulation_manager(state)
>>> simgr
<SimulationManager with 1 active>
>>> simgr.active                        # 当前 state
[<SimState @ 0x401370>]
>>> simgr.step()                        # 模拟执行一个 basic block
<SimulationManager with 1 active>
>>> simgr.active                        # 当前 state 被更新
[<SimState @ 0x1022f80>]
>>> simgr.active[0].regs.rip            # active[0] 是当前 state
<BV64 0x1022f80>
>>> state.regs.rip                      # 但原始的 state 并没有改变
<BV64 0x401370>
```

angr 提供了大量函数用于程序分析，在这些函数在 `Project.analyses.`，例如：

```python-repl
>>> cfg = p.analyses.CFGFast()          # 得到 control-flow graph
>>> cfg
<CFGFast Analysis Result at 0x7f1265b62650>
>>> cfg.graph
<networkx.classes.digraph.DiGraph object at 0x7f1265e77310> # 详情请查看 networkx
>>> len(cfg.graph.nodes())
934
>>> entry_node = cfg.get_any_node(proj.entry)   # 得到给定地址的 CFGNode
>>> entry_node
<CFGNode 0x401370[42]>
>>> len(list(cfg.graph.successors(entry_node)))
2
```

如果要想画出图来，还需要安装 matplotlib，Tkinter 等。

```python-repl
>>> import networkx as nx
>>> import matplotlib
>>> matplotlib.use('Agg')
>>> import matplotlib.pyplot as plt
>>> nx.draw(cfg.graph)                  # 画图
>>> plt.savefig('temp.png')             # 保存
```

#### 二进制文件加载器

我们知道 angr 是高度模块化的，接下来我们就分别来看看这些组成模块，其中用于二进制加载模块称为 CLE。主类为 `cle.loader.Loader`，它导入所有的对象文件并导出一个进程内存的抽象。类 `cle.backends` 是加载器的后端，根据二进制文件类型区分为 `cle.backends.elf`、`cle.backends.pe`、`cle.backends.macho` 等。

首先我们来看加载器的一些常用参数：

- `auto_load_libs`：是否自动加载主对象文件所依赖的共享库
- `except_missing_libs`：当有共享库没有找到时抛出异常
- `force_load_libs`：强制加载列表指定的共享库，不论其是否被依赖
- `skip_libs`：不加载列表指定的共享库，即使其被依赖
- `custom_ld_path`：可以到列表指定的路径查找共享库

如果希望对某个对象文件单独指定加载参数，可以使用 `main_ops` 和 `lib_opts` 以字典的形式指定参数。一些通用的参数如下：

- `backend`：使用的加载器后端，如：”elf”, “pe”, “mach-o”, “ida”, “blob” 等
- `custom_arch`：使用的 archinfo.Arch 对象
- `custom_base_addr`：指定对象文件的基址
- `custom_entry_point`：指定对象文件的入口点

举个例子：

```reasonml
angr.Project(main_opts={'backend': 'ida', 'custom_arch': 'i386'}, lib_opts={'libc.so.6': {'backend': 'elf'}})
```

加载对象文件和细分类型如下：

```stylus
>>> for obj in proj.loader.all_objects:
...     print obj
...
<ELF Object true, maps [0x400000:0x60721f]>
<ELF Object libc-2.27.so, maps [0x1000000:0x13bb98f]>
<ELF Object ld-2.27.so, maps [0x2000000:0x22260f7]>
<ELFTLSObject Object cle##tls, maps [0x3000000:0x300d010]>
<ExternObject Object cle##externs, maps [0x4000000:0x4008000]>
<KernelObject Object cle##kernel, maps [0x5000000:0x5008000]>
```

- `proj.loader.main_object`：主对象文件
- `proj.loader.shared_objects`：共享对象文件
- `proj.loader.extern_object`：外部对象文件
- `proj.loader.all_elf_object`：所有 elf 对象文件
- `proj.loader.kernel_object`：内核对象文件

通过对这些对象文件进行操作，可以解析出相关信息：

```python-repl
>>> obj = proj.loader.main_object
>>> obj
<ELF Object true, maps [0x400000:0x60721f]>
>>> hex(obj.entry)                          # 入口地址
'0x401370'
>>> hex(obj.min_addr), hex(obj.max_addr)    # 起始地址和结束地址
('0x400000', '0x60721f')
>>> for seg in obj.segments:                # segments
...     print seg
...
<ELFSegment offset=0x0, flags=0x5, filesize=0x5f48, vaddr=0x400000, memsize=0x5f48>
<ELFSegment offset=0x6c30, flags=0x6, filesize=0x450, vaddr=0x606c30, memsize=0x5f0>
>>> for sec in obj.sections:                # sections
...     print sec
...
<Unnamed | offset 0x0, vaddr 0x400000, size 0x0>
<.interp | offset 0x238, vaddr 0x400238, size 0x1c>
<.note.ABI-tag | offset 0x254, vaddr 0x400254, size 0x20>
<.note.gnu.build-id | offset 0x274, vaddr 0x400274, size 0x24>
...etc
```

根据地址查找我们需要的东西：

```python
>>> proj.loader.find_object_containing(0x400000)  # 包含指定地址的 object
<ELF Object true, maps [0x400000:0x60721f]>
>>> free = proj.loader.find_symbol('free')        # 根据名字或地址在 project 中查找 symbol
>>> free
<Symbol "free" in libc.so.6 at 0x1083ab0>
>>> free.name                                     # 符号名
u'free'
>>> free.owner_obj                                # 所属 object
<ELF Object libc-2.27.so, maps [0x1000000:0x13bb98f]>
>>> hex(free.rebased_addr)                        # 全局地址空间中的地址
'0x1083ab0'
>>> hex(free.linked_addr)                         # 相对于预链接基址的地址
'0x83ab0'
>>> hex(free.relative_addr)                       # 相对于对象基址的地址
'0x83ab0'
>>> free.is_export                                # 是否为导出符号
True
>>> free.is_import                                # 是否为导入符号
False
>>> obj.find_segment_containing(obj.entry)        # 包含指定地址的 segment
<ELFSegment offset=0x0, flags=0x5, filesize=0x5f48, vaddr=0x400000, memsize=0x5f48>
>>> obj.find_section_containing(obj.entry)        # 包含指定地址的 section
<.text | offset 0x12b0, vaddr 0x4012b0, size 0x33d9>
>>> main_free = obj.get_symbol('free')            # 根据名字在当前 object 中查找 symbol
>>> main_free
<Symbol "free" in true (import)>
>>> main_free.is_export
False
>>> main_free.is_import
True
>>> main_free.resolvedby                          # 从哪个 object 获得解析
<Symbol "free" in libc.so.6 at 0x1083ab0>
>>> hex(obj.linked_base)                          # 预链接的基址
'0x0'
>>> hex(obj.mapped_base)                          # 实际映射的基址
'0x400000'
```

通过 `obj.relocs` 可以查看所有的重定位符号信息，或者通过 `obj.imports` 可以得到一个符号信息的字典：

```python-repl
>>> for imp in obj.imports:
...     print imp, obj.imports[imp]
...
strncmp <cle.backends.elf.relocation.amd64.R_X86_64_GLOB_DAT object at 0x7faf8301b110>
lseek <cle.backends.elf.relocation.amd64.R_X86_64_GLOB_DAT object at 0x7faf8301b7d0>
malloc <cle.backends.elf.relocation.amd64.R_X86_64_GLOB_DAT object at 0x7faf8301be10>
>>> obj.imports['free'].symbol                    # 从重定向信息得到导入符号
<Symbol "free" in true (import)>
>>> obj.imports['free'].owner_obj                 # 从重定向信息得到所属的 object
<ELF Object true, maps [0x400000:0x60721f]>
```

这一部分还有个 hooking 机制，用于将共享库中的代码替换为其他的操作。使用函数 `proj.hook(addr, hook)` 和 `proj.hook_symbol(name, hook)` 来做到这一点，其中 `hook` 是一个 SimProcedure 的实例。通过 `.is_hooked`、`.unhook` 和 `.hooked_by` 来进行管理：

```python-repl
>>> stub_func = angr.SIM_PROCEDURES['stubs']['ReturnUnconstrained']   # 获得一个类
>>> stub_func
<class 'angr.procedures.stubs.ReturnUnconstrained.ReturnUnconstrained'>
>>> proj.hook(0x10000, stub_func())       # 使用类的一个实例来 hook
>>> proj.is_hooked(0x10000)
True
>>> proj.hooked_by(0x10000)
<SimProcedure ReturnUnconstrained>
>>> proj.hook_symbol('free', stub_func())
17316528
>>> proj.is_symbol_hooked('free')
True
>>> proj.is_hooked(17316528)
True
```

当然也可以利用装饰器编写自己的 hook 函数：

```python-repl
>>> @proj.hook(0x20000, length=5)         # length 参数可选，表示程序执行完 hook 后跳过几个字节
... def my_hook(state):
...     state.regs.rax = 1
...
>>> proj.is_hooked(0x20000)
True
```

## angr 在 CTF 中的运用

#### re DefcampCTF2015 entry_language

这是一题标准的密码验证题，输入一个字符串，程序验证对误。

```llvm
$ file entry_language
defcamp_r100: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.24, BuildID[sha1]=0f464824cc8ee321ef9a80a799c70b1b6aec8168, stripped
$ ./entry_language
Enter the password: ABCD
Incorrect password!
```

为了与 angr 的自动化做对比，我们先使用传统的方法，逆向算法求解，`main` 函数和验证函数 `fcn.004006fd` 如下：

```gherkin
[0x00400610]> pdf @ main
/ (fcn) main 153
|   main ();
|           ; var int local_110h @ rbp-0x110
|           ; var int local_8h @ rbp-0x8
|              ; DATA XREF from 0x0040062d (entry0)
|           0x004007e8      55             push rbp
|           0x004007e9      4889e5         mov rbp, rsp
|           0x004007ec      4881ec100100.  sub rsp, 0x110
|           0x004007f3      64488b042528.  mov rax, qword fs:[0x28]    ; [0x28:8]=-1 ; '(' ; 40
|           0x004007fc      488945f8       mov qword [local_8h], rax
|           0x00400800      31c0           xor eax, eax
|           0x00400802      bf37094000     mov edi, str.Enter_the_password: ; 0x400937 ; "Enter the password: "
|           0x00400807      b800000000     mov eax, 0
|           0x0040080c      e8affdffff     call sym.imp.printf         ; int printf(const char *format)
|           0x00400811      488b15500820.  mov rdx, qword [obj.stdin]  ; [0x601068:8]=0
|           0x00400818      488d85f0feff.  lea rax, [local_110h]
|           0x0040081f      beff000000     mov esi, 0xff               ; 255
|           0x00400824      4889c7         mov rdi, rax
|           0x00400827      e8b4fdffff     call sym.imp.fgets          ; char *fgets(char *s, int size, FILE *stream)
|           0x0040082c      4885c0         test rax, rax
|       ,=< 0x0040082f      7435           je 0x400866
|       |   0x00400831      488d85f0feff.  lea rax, [local_110h]
|       |   0x00400838      4889c7         mov rdi, rax
|       |   0x0040083b      e8bdfeffff     call fcn.004006fd            ; 调用验证函数
|       |   0x00400840      85c0           test eax, eax
|      ,==< 0x00400842      7511           jne 0x400855
|      ||   0x00400844      bf4c094000     mov edi, str.Nice_          ; 0x40094c ; "Nice!"
|      ||   0x00400849      e852fdffff     call sym.imp.puts           ; int puts(const char *s)
|      ||   0x0040084e      b800000000     mov eax, 0
|     ,===< 0x00400853      eb16           jmp 0x40086b
|     |||      ; JMP XREF from 0x00400842 (main)
|     |`--> 0x00400855      bf52094000     mov edi, str.Incorrect_password_ ; 0x400952 ; "Incorrect password!"
|     | |   0x0040085a      e841fdffff     call sym.imp.puts           ; int puts(const char *s)
|     | |   0x0040085f      b801000000     mov eax, 1
|     |,==< 0x00400864      eb05           jmp 0x40086b
|     |||      ; JMP XREF from 0x0040082f (main)
|     ||`-> 0x00400866      b800000000     mov eax, 0
|     ||       ; JMP XREF from 0x00400864 (main)
|     ||       ; JMP XREF from 0x00400853 (main)
|     ``--> 0x0040086b      488b4df8       mov rcx, qword [local_8h]
|           0x0040086f      6448330c2528.  xor rcx, qword fs:[0x28]
|       ,=< 0x00400878      7405           je 0x40087f
|       |   0x0040087a      e831fdffff     call sym.imp.__stack_chk_fail ; void __stack_chk_fail(void)
|       |      ; JMP XREF from 0x00400878 (main)
|       `-> 0x0040087f      c9             leave
\           0x00400880      c3             ret
[0x00400610]> pdf @ fcn.004006fd
/ (fcn) fcn.004006fd 171
|   fcn.004006fd (int arg_bh);
|           ; var int local_38h @ rbp-0x38
|           ; var int local_24h @ rbp-0x24
|           ; var int local_20h @ rbp-0x20
|           ; var int local_18h @ rbp-0x18
|           ; var int local_10h @ rbp-0x10
|           ; arg int arg_bh @ rbp+0xb
|              ; CALL XREF from 0x0040083b (main)
|           0x004006fd      55             push rbp
|           0x004006fe      4889e5         mov rbp, rsp
|           0x00400701      48897dc8       mov qword [local_38h], rdi
|           0x00400705      c745dc000000.  mov dword [local_24h], 0
|           0x0040070c      48c745e01409.  mov qword [local_20h], str.Dufhbmf ; 0x400914 ; "Dufhbmf"
|           0x00400714      48c745e81c09.  mov qword [local_18h], str.pG_imos ; 0x40091c ; "pG`imos"
|           0x0040071c      48c745f02409.  mov qword [local_10h], str.ewUglpt ; 0x400924 ; "ewUglpt"
|           0x00400724      c745dc000000.  mov dword [local_24h], 0
|       ,=< 0x0040072b      eb6e           jmp 0x40079b
|       |      ; JMP XREF from 0x0040079f (fcn.004006fd)
|      .--> 0x0040072d      8b4ddc         mov ecx, dword [local_24h]
|      :|   0x00400730      ba56555555     mov edx, 0x55555556
|      :|   0x00400735      89c8           mov eax, ecx
|      :|   0x00400737      f7ea           imul edx
|      :|   0x00400739      89c8           mov eax, ecx
|      :|   0x0040073b      c1f81f         sar eax, 0x1f
|      :|   0x0040073e      29c2           sub edx, eax
|      :|   0x00400740      89d0           mov eax, edx
|      :|   0x00400742      01c0           add eax, eax
|      :|   0x00400744      01d0           add eax, edx
|      :|   0x00400746      29c1           sub ecx, eax
|      :|   0x00400748      89ca           mov edx, ecx
|      :|   0x0040074a      4863c2         movsxd rax, edx
|      :|   0x0040074d      488b74c5e0     mov rsi, qword [rbp + rax*8 - 0x20]
|      :|   0x00400752      8b4ddc         mov ecx, dword [local_24h]
|      :|   0x00400755      ba56555555     mov edx, 0x55555556
|      :|   0x0040075a      89c8           mov eax, ecx
|      :|   0x0040075c      f7ea           imul edx
|      :|   0x0040075e      89c8           mov eax, ecx
|      :|   0x00400760      c1f81f         sar eax, 0x1f
|      :|   0x00400763      29c2           sub edx, eax
|      :|   0x00400765      89d0           mov eax, edx
|      :|   0x00400767      01c0           add eax, eax
|      :|   0x00400769      4898           cdqe
|      :|   0x0040076b      4801f0         add rax, rsi                ; '+'
|      :|   0x0040076e      0fb600         movzx eax, byte [rax]
|      :|   0x00400771      0fbed0         movsx edx, al
|      :|   0x00400774      8b45dc         mov eax, dword [local_24h]
|      :|   0x00400777      4863c8         movsxd rcx, eax
|      :|   0x0040077a      488b45c8       mov rax, qword [local_38h]
|      :|   0x0040077e      4801c8         add rax, rcx                ; '&'
|      :|   0x00400781      0fb600         movzx eax, byte [rax]
|      :|   0x00400784      0fbec0         movsx eax, al
|      :|   0x00400787      29c2           sub edx, eax
|      :|   0x00400789      89d0           mov eax, edx
|      :|   0x0040078b      83f801         cmp eax, 1                  ; 1
|     ,===< 0x0040078e      7407           je 0x400797                  ; = 1 时跳转，验证成功
|     |:|   0x00400790      b801000000     mov eax, 1                   ; 返回 1，验证失败
|    ,====< 0x00400795      eb0f           jmp 0x4007a6
|    ||:|      ; JMP XREF from 0x0040078e (fcn.004006fd)
|    |`---> 0x00400797      8345dc01       add dword [local_24h], 1     ; i = i + 1
|    | :|      ; JMP XREF from 0x0040072b (fcn.004006fd)
|    | :`-> 0x0040079b      837ddc0b       cmp dword [local_24h], 0xb  ; [0xb:4]=-1 ; 11
|    | `==< 0x0040079f      7e8c           jle 0x40072d                 ; i <= 11 时跳转
|    |      0x004007a1      b800000000     mov eax, 0                   ; 返回 0
|    |         ; JMP XREF from 0x00400795 (fcn.004006fd)
|    `----> 0x004007a6      5d             pop rbp
\           0x004007a7      c3             ret
```

整理后可以得到下面的伪代码：

```arduino
int fcn_004006fd(int *passwd) {
    char *str_1 = "Dufhbmf";
    char *str_2 = "pG`imos";
    char *str_3 = "ewUglpt";
    for (int i = 0; i <= 11; i++) {
        if((&str_3)[i % 3][2 * (1 / 3)] - *(i + passwd) != 1) {
            return 1;
        }
    }
    return 0;
}
```

然后写出逆向脚本：

```stylus
str_list = ["Dufhbmf", "pG`imos", "ewUglpt"]
passwd = []
for i in range(12):
    passwd.append(chr(ord(str_list[i % 3][2 * (i / 3)]) - 1))
print ''.join(passwd)
```

逆向算法似乎也很简单，但如果连算法都不用逆的话，下面就是见证 angr 魔力的时刻，我们只需要指定让程序运行到`0x400844`，即验证通过时的位置，而不用管验证的逻辑是怎么样的。完整的脚本如下：

```pf
import angr
project = angr.Project("entry_language", auto_load_libs=False)
@project.hook(0x400844)
def print_flag(state):
    print "FLAG SHOULD BE:", state.posix.dump_fd(0)
    project.terminate_execution()
project.execute()
```

Bingo!!!

```shell
$ python2 solve_angr.py
FLAG SHOULD BE: Code_Talkers
$ ./entry_language
Enter the password: Code_Talkers
Nice!
```