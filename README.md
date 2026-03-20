<p align="center">
  <img src="Resources/Branding/BannerSmall.png" alt="Logo" width=300 height=180>
</p>

<h3 align="center">rocketvm</h3>

<p align="center">
  A custom 16-bit virtual machine and assembler written in C
  <br>
  <a href="https://github.com/krnl32/rocketvm/issues/new?labels=bug">Report bug</a>
  ·
  <a href="https://github.com/krnl32/rocketvm/issues/new?labels=feature">Request feature</a>
</p>

## About
**rocketvm** is a custom 16-bit virtual machine with:

* A bytecode-based VM (LIB) (rocketvm)
* A custom assembler (rocketasm)
* A runtime executor (rocketrun)

## Architecture Overview
```
.rsm (assembly)
   ↓
rocketasm (lexer → parser → encoder)
   ↓
.rvm (bytecode)
   ↓
rocketrun (VM runtime)
   ↓
execution on rocketvm
```

## Features
### Virtual Machine
* 16-bit architecture
* 8 general-purpose registers (RV0–RV7)
* Instruction pointer (RIP) and stack pointer (RSP)
* Flag register (Zero, Negative, Carry, Overflow)
* Fixed 2-byte instruction encoding
* Memory with bounds + alignment checks

### Instruction Set
* Data: `MOV`, `LOAD`, `STORE`
* Arithmetic: `ADD`, `SUB`, `MUL`, `DIV`, `MOD`
* Bitwise: `SHL`, `SHR`
* Control flow: `JMP`, `JZ`, `JNZ`, `CALL`, `RET`
* Comparison: `CMP`

### Assembler (rocketasm)
* Custom lexer and parser
* Label support (forward + backward references)
* Multiple operand types:

	* registers
	* immediates
	* memory (`[addr]`, `[reg]`, `[label]`)

### Runtime (rocketrun)
* Loads & Executes `.rvm` binaries

## Example
```asm
start:
    mov rv0, 5
    mov rv1, 10
    add rv0, rv1

    cmp rv0, 15
    jz equal

    jmp end

equal:
    mov rv2, 1

end:
    ret
```
## Build
cmake -B build
cmake --build build

## Usage
### Assemble
./rocketasm program.rsm -o program.rvm

### Run
./rocketrun program.rvm

## Design Notes
* Instructions are **fixed-width (2 bytes)** for simplicity
* Control flow supports both **relative addressing -> IMM11** and **absolute addressing -> REG**
* Memory accesses are **aligned (16-bit)**
* The VM uses a **register-based design** (not stack-based)

## ISA
See [Architecture.md](https://github.com/krnl32/rocketvm/blob/master/specification/Architecture.md)

## Contributing
See [CONTRIBUTING.md](https://github.com/krnl32/rocketvm/blob/master/CONTRIBUTING.md)

## License
Licensed under the [GPL-3 License](https://github.com/krnl32/rocketvm/blob/master/COPYING.txt)


## Author
* **krnl32** — https://github.com/krnl32
