~~# 16-bit Virtual Machine — Architecture Specification (v0.1)

---

## 1. Core Philosophy

- Register-based architecture
- Fixed-width instructions (16-bit)
- Little-endian memory
- Flat memory model (no segmentation or paging)
- Load/Store design:
	- Memory is accessed **only** via `LOAD` / `STORE`
- Instruction Pointer (`RIP`) always points to the **next instruction**
- Stack grows **downward** (high → low addresses)
- Stack is 16-bit aligned
- CALL pushes the return address (next instruction)
- RET pops the return address into `RIP`

---

## 2. Registers

### General Purpose Registers

```
RV0–RV7 : 8 general-purpose 16-bit registers
```

### Special Registers

```
RIP : Instruction Pointer (16-bit)
RSP : Stack Pointer (16-bit)
RSB : Stack Base / Frame Pointer (16-bit)
```

### FLAGS Register

```
RZ : Zero flag        (result == 0)
RN : Negative flag    (MSB set)
RC : Carry flag       (unsigned overflow)
```

---

## 3. Memory Model

### Address Space

```
16-bit address space → 64 KB
Range: 0x0000 – 0xFFFF
Byte-addressable
```

### Memory Layout

```
0x0000 – 0x7FFF   Program / Data        32 KB
0x8000 – 0xEFFF   Heap (RAM)            28 KB
0xF000 – 0xFEFF   Stack (RAM)           3.75 KB
0xFF00 – 0xFFFF   MMIO                  256 B
```

### Stack

* Stack grows downward
* `RSP` always points to the **top of the stack**

#### Push

```
RSP = RSP - 2
memory[RSP] = value
```

#### Pop

```
value = memory[RSP]
RSP = RSP + 2
```

### Frame Pointer (RSB)

```
RSB is used to:
- Mark the start of a stack frame
- Access function arguments and local variables
```

---

## 4. Instruction Format

### Format A — Register / Immediate

```
[ OPCODE (4) | REG (3) | MODE (1) | OPERAND (8) ]
```

```
MODE = 0 → OPERAND = IMM8
MODE = 1 → OPERAND = RVS (lower 3 bits used)
```

---

### Format B — Memory (Load/Store)

```
[ OPCODE (4) | REG (3) | MODE (1) | OPERAND (8) ]
```

```
MODE = 0 → OPERAND = ADDRESS
MODE = 1 → OPERAND = RADDR (lower 3 bits used)
```

---

### Format C — Control Flow

```
[ OPCODE (4) | ADDR (12) ]
```

---

### Field Definitions

```
OPCODE (4 bits) : operation selector (16 possible instructions)

REG (3 bits):
    - ALU / LOAD :  RVD (destination)
    - STORE      :  RVS (value source)

OPERAND (8 bits) : Interpreted based on MODE:
   MODE = 0: OPERAND = IMM8 (8-bit immediate)
   MODE = 1: OPERAND
		- RVS (3 bits) for ALU ops
		- Raddr (address register) for memory ops
```

---

### Notes

* Not all instructions use all fields
* Unused fields are ignored
* Instructions are aligned to **2 bytes**
* `RIP += 2` after each instruction

---

## 5. Instruction Set Architecture (ISA)

### 5.1 Data Movement

```asm
MOV  RVD, IMM8
MOV  RVD, RVS
```

### 5.2 Memory Access

```asm
; addr8 -> OPERAND, Raddr -> OPERAND & 0x7 
LOAD  RVD, [addr8] 	; MODE=0 → direct address, load memory[addr8] → RVD
LOAD  RVD, [RVS]	; MODE=1 → indirect, load memory[reg[RVS]] → RVD

STORE [addr8], RVS	; MODE=0 → direct address, store RVS → memory[addr8]
STORE [Raddr], RVS	; MODE=1 → indirect, store RVS → memory[reg[Raddr]]

```

---

### Memory Semantics

* Operates on **16-bit words**
* Little-endian layout
* Address alignment (multiple of 2) is recommended

---

### 5.3 Arithmetic / Logic

```asm
ADD RVD, IMM8     ; RVD = RVD + IMM8
ADD RVD, RVS      ; RVD = RVD + RVS

SUB RVD, IMM8     ; RVD = RVD - IMM8
SUB RVD, RVS      ; RVD = RVD - RVS

MUL RVD, IMM8     ; RVD = RVD * IMM8
MUL RVD, RVS      ; RVD = RVD * RVS (lower 16 bits kept)

DIV RVD, IMM8     ; RVD = RVD / IMM8
DIV RVD, RVS      ; RVD = RVD / RVS (quotient)

MOD RVD, IMM8     ; RVD = RVD % IMM8
MOD RVD, RVS      ; RVD = RVD % RVS (remainder)

SHL RVD, IMM8     ; RVD = RVD << IMM8
SHL RVD, RVS      ; RVD = RVD << RVS

SHR RVD, IMM8     ; RVD = RVD >> IMM8 (logical)
SHR RVD, RVS      ; RVD = RVD >> RVS (logical)
```

---

### FLAGS Behavior

| Instruction | RZ | RN | RC | RO |
|-------------|----|----|----|----|
| ADD         | ✓  | ✓  | ✓  | ✓  |
| SUB         | ✓  | ✓  | ✓  | ✓  |
| MUL         | ✓  | ✓  | ✓  | ✓  |
| DIV         | ✓  | ✓  | ❌  | ❌  |
| MOD         | ✓  | ✓  | ❌  | ❌  |
| SHL         | ✓  | ✓  | ✓  | ✓  |
| SHR         | ✓  | ✓  | ✓  | ❌  |
| CMP         | ✓  | ✓  | ✓  | ✓  |
| LOAD        | ❌  | ❌  | ❌  | ❌  |
| STORE       | ❌  | ❌  | ❌  | ❌  |
| JMP         | ❌  | ❌  | ❌  | ❌  |
| JZ/JNZ      | ❌  | ❌  | ❌  | ❌  |
| CALL        | ❌  | ❌  | ❌  | ❌  |
| RET         | ❌  | ❌  | ❌  | ❌  |
| PUSH        | ❌  | ❌  | ❌  | ❌  |
| POP         | ❌  | ❌  | ❌  | ❌  |

---

### 5.4 Comparison

```asm
CMP RVD, IMM8     ; compare RVD with IMM8
CMP RVD, RVS      ; compare RVD with RVS (sets FLAGS, no writeback)
```

### 5.5 Control Flow

```asm
JMP ADDR12		; unconditional jump to address
JZ  ADDR12		; jump if RZ == 1
JNZ ADDR12		; jump if RZ == 0
```

### 5.6 Stack

```asm
; PUSH RVS
SUB RSP, 2			; make space
STORE [RSP], RVS	; write value

; POP
LOAD RVD, [RSP]		; read value
ADD RSP, 2			; move stack up
```

---

### 5.7 Call

```asm
CALL RVS
RET
```

#### CALL RVS

```
; Example
MOVI RV1, 0x10
SHL  RV1, 4
CALL RV1

; push return address
RSP = RSP - 2
memory[RSP] = RIP

; jump to address in register
RIP = RV[RVS]
```

#### 5.8 RET

```
; pop return address
RIP = memory[RSP]
RSP = RSP + 2
```

---

#### Semantics

```
; ADD
; RVD = RVD + operand
; RZ = (RVD == 0)
; RN = MSB of RVD
; RC = carry out
; RO = signed overflow

; SUB
; RVD = RVD - operand
; RZ = (RVD == 0)
; RN = MSB of RVD
; RC = borrow
; RO = signed overflow

; MUL
; RVD = (RVD * operand) & 0xFFFF
; RZ = (RVD == 0)
; RN = MSB of RVD
; RC = overflow
; RO = overflow

; DIV
; if operand == 0 → HALT
; RVD = RVD / operand
; RZ = (RVD == 0)
; RN = MSB of RVD

; MOD
; if operand == 0 → HALT
; RVD = RVD % operand
; RZ = (RVD == 0)
; RN = MSB of RVD

; SHL
; shift = IMM8 & 0xF
; RVD = (RVD << shift) & 0xFFFF
; RZ = (RVD == 0)
; RN = MSB of RVD
; RC = last bit shifted out (bit (16 - shift))

; SHR
; shift = IMM8 & 0xF
; RVD = RVD >> shift
; RZ = (RVD == 0)
; RN = MSB of RVD
; RC = last bit shifted out Right (LSB)

; LOAD
; address =
;   if MODE == 0 → ADDR8
;   if MODE == 1 → R[RVS]
; RVD = memory16[address]

; STORE
; address =
;   if MODE == 0 → ADDR8
;   if MODE == 1 → R[RVS]
; memory16[address] = R[RVS]

; CMP
; temp = RVD - operand
; RZ = (temp == 0)
; RN = MSB of temp
; RC = borrow
; RO = signed overflow
; RVD unchanged

; JMP
; RIP = ADDR12

; JZ
; if RZ == 1 → RIP = ADDR12

; JNZ
; if RZ == 0 → RIP = ADDR12

; CALL
; RSP -= 2
; memory16[RSP] = RIP
; RIP = R[RVS]

; RET
; RIP = memory16[RSP]
; RSP += 2

; PUSH RVS
; SUB RSP, 2			; make space
; STORE [RSP], RVS    	; write value

; POP
; LOAD RVD, [RSP]     	; read value
; ADD RSP, 2          	; move stack up
```

---

## 6. Execution Model

Each instruction follows:

```
1. Fetch instruction at RIP
2. Decode (OPCODE, RVD, RVS, IMM8)
3. Execute operation

4. Update RIP:
   - If the instruction modified RIP → do nothing
   - Else → RIP = RIP + 2
```

---

## 7. Design Constraints

- No memory-to-memory operations
- Fixed 16-bit instruction width
- Immediate = 8-bit
- Larger values require multiple instructions~~
